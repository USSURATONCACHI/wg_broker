#pragma once

#include <string>

#include <wg_broker/gen/echo_skeleton.h>
#include <gio/gio.h>

namespace ussur {
namespace wg {


class EchoServiceImpl {
public:
    void connect_skeleton_signals(EchoService* skeleton);

    
    std::string echo(EchoService *skeleton, GDBusMethodInvocation *invocation, std::string arg_in);

};


} // namespace wg
} // namespace ussur