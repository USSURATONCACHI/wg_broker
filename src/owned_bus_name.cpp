#include <wg_broker/owned_bus_name.hpp>
#include <wg_broker/exceptions.hpp>

#include <iostream>

namespace ussur {
namespace wg {


    // DBusConnection* connection; // To be able to free it later
    // std::string name;
OwnedBusName OwnedBusName::acquire(GDBusConnection* conn, const std::string& name, unsigned int flags) {
    GError *error = NULL;
    GVariant *result = g_dbus_connection_call_sync(
        conn,
        "org.freedesktop.DBus",  // Bus name to call
        "/org/freedesktop/DBus", // Object path
        "org.freedesktop.DBus",  // Interface name
        "RequestName",           // Method name
        g_variant_new("(su)", name.c_str(), flags),  // Parameters
        NULL,                    // Expected reply type
        G_DBUS_CALL_FLAGS_NONE,   // Call flags
        -1,                       // Timeout (use default)
        NULL,                     // Cancellable
        &error                    // Error
    );

    if (error) {
        std::string error_msg = std::string("Failed to request bus name: ") + error->message;
        g_error_free(error);
        throw ussur::wg::BusNameException(error_msg);
    } else {
        return OwnedBusName(conn, std::string(name));
    }
}

OwnedBusName::~OwnedBusName() {
    GError *error = NULL;

    // Call the ReleaseName method on org.freedesktop.DBus interface
    GVariant *result = g_dbus_connection_call_sync(
        connection,
        "org.freedesktop.DBus",            // Bus name to call
        "/org/freedesktop/DBus",           // Object path
        "org.freedesktop.DBus",            // Interface name
        "ReleaseName",                     // Method name
        g_variant_new("(s)", name.c_str()),// Parameters
        NULL,                              // Expected reply type
        G_DBUS_CALL_FLAGS_NONE,            // Call flags
        -1,                                // Timeout (use default)
        NULL,                              // Cancellable
        &error                             // Error
    );

    if (error) {
        std::cerr << "Release Name Error: " << error->message << std::endl;
        g_error_free(error);
    } else {
        std::cout << "Bus name released successfully: " << name << std::endl;
    }

    // Cleanup the result if necessary
    if (result) {
        g_variant_unref(result);
    }
}

OwnedBusName::OwnedBusName(OwnedBusName&& move_from) :
    connection(std::move(move_from.connection)),
    name(std::move(move_from.name))
{
    new (&move_from) OwnedBusName();
}

OwnedBusName& OwnedBusName::operator=(OwnedBusName&& move_from) {
    if (this != &move_from) {
        this->~OwnedBusName();
        new (this) OwnedBusName(std::move(move_from));
    }
    return *this;
}

OwnedBusName::OwnedBusName() noexcept :
    connection(nullptr),
    name()
{}

OwnedBusName::OwnedBusName(GDBusConnection* conn, std::string&& name) noexcept :
    connection(conn),
    name(std::move(name))
{}


} // namespace wg
} // namespace ussur