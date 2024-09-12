#pragma once

#include <string>
#include <functional>
#include <optional>

#include <gio/gio.h>
#include <wg_broker/owned_ptr.hpp>

extern "C" {
    #include <wg_broker/gen/echo_skeleton.h>
}

namespace ussur {
namespace wg {


OwnedPtr<ExampleEchoService> create_skeleton(
    GDBusConnection* connection, 
    const std::string& object_path,
    std::function<void(ExampleEchoService*)> callback_signal_connect
);


} // namespace wg
} // namespace ussur