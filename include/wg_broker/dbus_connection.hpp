#pragma once

#include <glib.h>
#include <gio/gio.h>
#include <wg_broker/owned_ptr.hpp>

namespace ussur {
namespace wg {


OwnedPtr<GDBusConnection> establish_connection(GBusType type);


} // namespace wg
} // namespace ussur