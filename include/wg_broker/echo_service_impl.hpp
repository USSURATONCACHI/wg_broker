#pragma once

#include <string>

#include <wg_broker/base_service.hpp>
#include <wg_broker/gen/echo_skeleton.h>
#include <gio/gio.h>

namespace ussur {
namespace wg {


class EchoServiceImpl : BaseService<EchoService> {
public:

    virtual CreateSkeletonInfo<EchoService> get_create_skeleton_info();
    virtual void connect_skeleton_signals(EchoService* skeleton);

    std::string echo(EchoService* skeleton, GDBusMethodInvocation *invocation, std::string arg_input);
};


} // namespace wg
} // namespace ussur