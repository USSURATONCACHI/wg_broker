#include "wg_broker/owned_ptr.hpp"
#include <dbus/dbus.h>

#include <wg_broker/dbus_connection.hpp>
#include <wg_broker/exceptions.hpp>


namespace ussur {
namespace wg {


OwnedPtr<DBusConnection> establish_connection(DBusBusType type) {
    DBusError error;
    dbus_error_init(&error);

    // Connect to the session bus
    DBusConnection* connection = dbus_bus_get(type, &error);
    if (dbus_error_is_set(&error)) {
        std::string error_msg = std::string("Connection error: ") + error.message;
        dbus_error_free(&error);
        throw ussur::wg::ConnectionException(error_msg);
    }
    if (!connection) {
        throw ussur::wg::ConnectionException("Failed to connect to the D-Bus session bus");
    }

    return OwnedPtr<DBusConnection>(std::move(connection), dbus_connection_unref);
}

OwnedPtr<DBusMessage>  connection_pop_message(DBusConnection* connection) {
    DBusMessage* message = dbus_connection_pop_message(connection);
    return OwnedPtr<DBusMessage>(message, dbus_message_unref);
}


} // namespace wg
} // namespace ussur
