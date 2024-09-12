#include <wg_broker/echo_service_impl.hpp>

#include <wg_broker/gen/echo_skeleton.h>

#include <stdexcept>
#include <exception>
#include <string>

#include <gio/gio.h>
#include <glib.h>


namespace ussur {
namespace wg {

static gboolean handle_echo(EchoService *skeleton, GDBusMethodInvocation *invocation, const gchar* arg_input, EchoServiceImpl* self);

void EchoServiceImpl::connect_skeleton_signals(EchoService* skeleton) {
    g_signal_connect(skeleton, "handle-echo", G_CALLBACK(handle_echo), this);
}




std::string EchoServiceImpl::echo(EchoService *skeleton, GDBusMethodInvocation *invocation, std::string arg_input) {
    return "Echoing: " + arg_input;
}




static gboolean handle_echo(EchoService *skeleton, GDBusMethodInvocation *invocation, const gchar* arg_input, EchoServiceImpl* self) {
    std::string result;
    try {
        result = self->echo(skeleton, invocation, std::string(arg_input));
    } catch (std::exception& e) {
        g_dbus_method_invocation_return_error(
            invocation, 
            g_quark_from_string("ussur.wg.EchoError"), 
            1, 
            "%s", e.what()
        );
        return FALSE;
    }

    echo_service_complete_echo(skeleton, invocation, g_strdup(result.c_str()));
    return TRUE;
}

} // namespace wg
} // namespace ussur

