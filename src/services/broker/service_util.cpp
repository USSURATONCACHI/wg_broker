#include <wg_broker/services/broker/service_util.hpp>

#include <iostream>
#include <fstream>
#include <filesystem>
#include <iterator>
#include <string>

#include <wg_broker/util.hpp>

namespace ussur {
namespace wg {
namespace broker {


std::optional<std::string> systemd_get_unit(GDBusConnection* conn, std::string name){
    GError *error = NULL;
    GVariant *result = g_dbus_connection_call_sync(
        conn, 
        "org.freedesktop.systemd1", 
        "/org/freedesktop/systemd1", 
        "org.freedesktop.systemd1.Manager", 
        "GetUnit", 
        g_variant_new("(s)", name.c_str()), 
        G_VARIANT_TYPE("(o)"), 
        G_DBUS_CALL_FLAGS_NONE, 
        -1,
        NULL, 
        &error
    );

    std::optional<std::string> ret_value = {};
    if (!result) {
        // Check if it's a specific D-Bus error like org.freedesktop.systemd1.NoSuchUnit
        if (g_strrstr(error->message, "org.freedesktop.systemd1.NoSuchUnit")) {
            ret_value = {};
        } else {
            throw std::runtime_error("Unknown kind of dbus error returned to  " + std::string(__PRETTY_FUNCTION__));
        }
        g_clear_error(&error);
    } else {
        gchar* object_path;
        g_variant_get(result, "(o)", &object_path);
        ret_value = { std::string(object_path) };
    }
    g_variant_unref(result);

    return ret_value;
}

std::string systemd_get_unit_property_string(GDBusConnection* conn, std::string object_path, std::string property_path) {
    const std::string INTERFACE_NAME = "org.freedesktop.systemd1.Unit";

    GError *error = NULL;
    GVariant *result = g_dbus_connection_call_sync(
        conn, 
        "org.freedesktop.systemd1", 
        object_path.c_str(), 
        "org.freedesktop.DBus.Properties", 
        "Get", 
        g_variant_new("(ss)", INTERFACE_NAME.c_str(), property_path.c_str()), 
        G_VARIANT_TYPE("(v)"), 
        G_DBUS_CALL_FLAGS_NONE, 
        -1,
        NULL, 
        &error
    );

    if (!result) {
        std::string error_msg = error->message;
        g_error_free(error);
        throw std::runtime_error(error_msg);
    } else {
        GVariant* variant;
        gchar* g_ret_value;
        g_variant_get(result, "(v)", &variant);
        g_variant_get(variant, "s", &g_ret_value);
        std::string ret_value = std::string(g_ret_value);
        g_variant_unref(result);
        return ret_value;
    }
}

bool systemd_check_if_unit_on_startup(GDBusConnection* conn, std::string unit_name) {
    GError *error = NULL;
    GVariant *result = g_dbus_connection_call_sync(
        conn, 
        "org.freedesktop.systemd1", 
        "/org/freedesktop/systemd1", 
        "org.freedesktop.systemd1.Manager", 
        "GetUnitFileState", 
        g_variant_new("(s)", unit_name.c_str()), 
        G_VARIANT_TYPE("(s)"), 
        G_DBUS_CALL_FLAGS_NONE, 
        -1,
        NULL, 
        &error
    );

    bool ret_value = false;
    if (!result) {
        std::cout << __PRETTY_FUNCTION__ << " returned error: " << error->message << std::endl;
        g_clear_error(&error);
    } else {
        gchar* state;
        g_variant_get(result, "(s)", &state);
        std::cout << __PRETTY_FUNCTION__ << " returned success: " << state << std::endl;
        ret_value = std::string(state) == "enabled";
    }

    return ret_value;
}

ProfileStatus get_profile_status(GDBusConnection *connection, std::filesystem::path filepath, std::string profile_name) {
    std::string service_name = "wg-quick@" + profile_name + ".service";

    ProfileStatus p;
    p.name    = profile_name;
    p.is_loaded = false;
    p.has_error = false;
    p.is_startup = false;

    // Get log using journalctl
    {
        std::ifstream ifs(filepath);
        p.content = std::string(std::istreambuf_iterator<char>{ifs}, {});

        std::string prev_time_activated = "2024-09-13 17:00";
        std::string command = "journalctl -u " + service_name + " --since \"" + prev_time_activated + "\"";
        p.log     = ussur::wg::exec(command.c_str());
    }

    // Get ActiveState using systemd
    {
        auto object_path = systemd_get_unit(connection, service_name);

        if (object_path.has_value()) {
            std::string active_state = systemd_get_unit_property_string(connection, *object_path, "ActiveState");
            std::cout << "ActiveState: " << active_state << std::endl; 

            if (active_state == "active")
                p.is_loaded = true;
            else if (active_state == "failed")
                p.has_error = true;
        }
    }

    // Check if unit is enabled to launch on startup
    p.is_startup = systemd_check_if_unit_on_startup(connection, service_name);

    return p;
}


} // namespace broker
} // namespace wg
} // namespace ussur