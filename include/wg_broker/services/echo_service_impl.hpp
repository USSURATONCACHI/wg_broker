#pragma once

#include <string>

#include <wg_broker/base_service.hpp>
#include <gio/gio.h>

extern "C" {
    #include <wg_broker/gen/echo_skeleton.h>
}

namespace ussur {
namespace wg {

using EchoSkeleton = EchoService;

class EchoServiceImpl : BaseService<EchoSkeleton> {
public:

    virtual CreateSkeletonInfo<EchoSkeleton> get_create_skeleton_info();
    virtual void connect_skeleton_signals(EchoSkeleton* skeleton);

    std::string echo(EchoSkeleton* skeleton, GDBusMethodInvocation *invocation, std::string arg_input);
};


} // namespace wg
} // namespace ussur