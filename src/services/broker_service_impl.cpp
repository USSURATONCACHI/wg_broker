#include <stdexcept>
#include <wg_broker/services/broker_service_impl.hpp>

#include <exception>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>

#include <glib.h>
#include <gio/gio.h>
#include <wg_broker/gen/broker_skeleton.h>
#include <wg_broker/util.hpp>

namespace fs = std::filesystem;


namespace ussur {
namespace wg {


using BrokerSkeleton = Broker;
using ProfileStatus = BrokerServiceImpl::ProfileStatus;

// Local helpers
static std::optional<std::string> systemd_get_unit(GDBusConnection* conn, std::string name);
static std::string systemd_get_unit_property_string(GDBusConnection* conn, std::string object_path, std::string property_path);
static bool systemd_check_if_unit_on_startup(GDBusConnection* conn, std::string unit_name);

// Handles (declarations)
static gboolean handle_get_profiles(BrokerSkeleton *skeleton, GDBusMethodInvocation *invocation, BrokerServiceImpl* self);


// Interface methods:
BrokerServiceImpl::BrokerServiceImpl(std::filesystem::path wg_profiles_dir)
    : wg_profiles_dir(wg_profiles_dir)
{}

std::vector<ProfileStatus> 
    BrokerServiceImpl::get_profiles(BrokerSkeleton* skeleton, GDBusMethodInvocation *invocation) 
{
    std::vector<ProfileStatus> profiles;
    GDBusConnection *connection = g_dbus_method_invocation_get_connection(invocation);

    for (const fs::directory_entry& entry : fs::directory_iterator(wg_profiles_dir)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename();
            const std::string FILE_TYPE = ".conf";
            if (!filename.ends_with(FILE_TYPE)) 
                continue;
            
            std::string profile_name = filename.substr(0, filename.length() - FILE_TYPE.length());
            std::string service_name = "wg-quick@" + profile_name + ".service";

            auto path = systemd_get_unit(connection, service_name);

            ProfileStatus p;
            p.name    = profile_name;

            std::ifstream ifs(entry.path());
            p.content = std::string(std::istreambuf_iterator<char>{ifs}, {});

            std::string command = "journalctl -u " + service_name + " --since \"2024-09-13 17:00\"";
            p.log     = ussur::wg::exec(command.c_str());

            p.is_loaded = false;
            p.has_error = false;
            p.is_startup = false;

            if (path.has_value()) {
                std::string active_state = systemd_get_unit_property_string(connection, *path, "ActiveState");
                std::cout << "ActiveState: " << active_state << std::endl; 

                if (active_state == "active")
                    p.is_loaded = true;
                else if (active_state == "failed")
                    p.has_error = true;
            }
            p.is_startup = systemd_check_if_unit_on_startup(connection, service_name);

            profiles.push_back(p);
        }
    }

    return profiles;
}


// Helpers
CreateSkeletonInfo<BrokerSkeleton> BrokerServiceImpl::get_create_skeleton_info() {
    CreateSkeletonInfo<BrokerSkeleton> info;
    info.fn_create_skeleton = broker_skeleton_new;
    info.fn_connect_signals = std::bind(&BrokerServiceImpl::connect_skeleton_signals, this, std::placeholders::_1);
    return info;
}
void BrokerServiceImpl::connect_skeleton_signals(BrokerSkeleton* skeleton) {
    g_signal_connect(skeleton, "handle-get-profiles", G_CALLBACK(handle_get_profiles), this);

}


// Handlers (impls)
static GVariant* convert_profiles_to_gvariant(const std::vector<ProfileStatus>& profiles) {
    GVariantBuilder builder;
    g_variant_builder_init(&builder, G_VARIANT_TYPE_ARRAY);

    // Iterate over each ProfileStatus in the vector
    for (const auto& profile : profiles) {
        g_variant_builder_add(&builder, "(sssbbb)",
            profile.name.c_str(),           // String name
            profile.content.c_str(),        // String content
            profile.log.c_str(),            // String log
            profile.is_loaded,              // Boolean is_loaded
            profile.is_startup,             // Boolean is_startup
            profile.has_error               // Boolean has_error
        );
    }

    // Return the array variant
    return g_variant_builder_end(&builder);
}

static gboolean handle_get_profiles(BrokerSkeleton *skeleton, GDBusMethodInvocation *invocation, BrokerServiceImpl* self) {
    GVariant* result = NULL;
    try {
        std::vector<ProfileStatus> profiles = self->get_profiles(skeleton, invocation);
        result = convert_profiles_to_gvariant(profiles);

    } catch (std::exception& e) {
        if (result) {
            g_variant_unref(result);
        }

        g_dbus_method_invocation_return_error(invocation, g_quark_from_string("ussur.wg.BrokerError"), 1, "%s", e.what());
        return FALSE;
    }

    broker_complete_get_profiles(skeleton, invocation, result);
    
    // Free the GVariant
    // if (result)
    //     g_variant_unref(result);

    return TRUE;
}


static std::optional<std::string> systemd_get_unit(GDBusConnection* conn, std::string name) {
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

static std::string systemd_get_unit_property_string(GDBusConnection* conn, std::string object_path, std::string property_name) {
    const std::string INTERFACE_NAME = "org.freedesktop.systemd1.Unit";

    GError *error = NULL;
    GVariant *result = g_dbus_connection_call_sync(
        conn, 
        "org.freedesktop.systemd1", 
        object_path.c_str(), 
        "org.freedesktop.DBus.Properties", 
        "Get", 
        g_variant_new("(ss)", INTERFACE_NAME.c_str(), property_name.c_str()), 
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

static bool systemd_check_if_unit_on_startup(GDBusConnection* conn, std::string unit_name) {
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

} // namespace wg
} // namespace ussur