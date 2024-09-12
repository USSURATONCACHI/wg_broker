#include "echo_skeleton.h"
#include "gio/gio.h"
#include <string>
#include <wg_broker/echo_service.hpp>

namespace ussur {
namespace wg {


static gboolean handle_echo(ExampleEchoService *skeleton, GDBusMethodInvocation *invocation, const gchar* arg_input, EchoService* self) {
    return self->echo(skeleton, invocation, arg_input);
}


void EchoService::connect_skeleton_signals(ExampleEchoService* skeleton) {
    g_signal_connect(skeleton, "handle-echo", G_CALLBACK(handle_echo), this);
}



gboolean EchoService::echo(ExampleEchoService *skeleton, GDBusMethodInvocation *invocation, const gchar* arg_input) {
    std::string output = "Echoing: " + std::string(arg_input);

    example_echo_service_complete_echo(skeleton, invocation, g_strdup(output.c_str()));
    return TRUE;
}


} // namespace wg
} // namespace ussur