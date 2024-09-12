#pragma once


#include <wg_broker/gen/echo_skeleton.h>
#include <gio/gio.h>

namespace ussur {
namespace wg {


class EchoService {
public:
    void connect_skeleton_signals(ExampleEchoService* skeleton);

    gboolean echo(ExampleEchoService *skeleton, GDBusMethodInvocation *invocation, const gchar* arg_input);

};


} // namespace wg
} // namespace ussur