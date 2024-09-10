#pragma once

#include <dbus/dbus.h>
#include <wg_broker/owned_ptr.hpp>

namespace ussur {
namespace wg {


OwnedPtr<DBusConnection> establish_connection(DBusBusType type); 


} // namespace wg
} // namespace ussur