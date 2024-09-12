#pragma once

#include <string>

#include <dbus/dbus.h>
#include <wg_broker/owned_ptr.hpp>

extern "C" {
    #include <wg_broker/gen/echo_skeleton.h>
}

namespace ussur {
namespace wg {


OwnedPtr<ExampleEchoService> create_skeleton(DBusConnection* connection, const std::string& object_path);


} // namespace wg
} // namespace ussur