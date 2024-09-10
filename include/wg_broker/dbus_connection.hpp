#pragma once

#include <dbus/dbus.h>
#include <wg_broker/owned_ptr.hpp>

namespace ussur {
namespace wg {


OwnedPtr<DBusConnection> establish_connection(DBusBusType type);
OwnedPtr<DBusMessage>    connection_pop_message(DBusConnection* connection);

inline OwnedPtr<DBusMessage>  connection_pop_message(OwnedPtr<DBusConnection>& connection) {
    return connection_pop_message(connection.get());
}


} // namespace wg
} // namespace ussur