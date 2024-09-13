#include <wg_broker/services/echo_service_impl.hpp>

#include <wg_broker/gen/echo_skeleton.h>

#include <exception>
#include <string>

#include <gio/gio.h>
#include <glib.h>


namespace ussur {
namespace wg {

// Handles (declarations)
static gboolean handle_echo(EchoSkeleton *skeleton, GDBusMethodInvocation *invocation, const gchar* arg_input, EchoServiceImpl* self);


// Interface methods:
std::string EchoServiceImpl::echo(EchoSkeleton *skeleton, GDBusMethodInvocation *invocation, std::string arg_input) {
    return "Echoing: " + arg_input;
}


// Helpers
CreateSkeletonInfo<EchoSkeleton> EchoServiceImpl::get_create_skeleton_info() {
    CreateSkeletonInfo<EchoSkeleton> info;
    info.fn_create_skeleton = echo_service_skeleton_new;
    info.fn_connect_signals = std::bind(&EchoServiceImpl::connect_skeleton_signals, this, std::placeholders::_1);
    return info;
}

void EchoServiceImpl::connect_skeleton_signals(EchoSkeleton* skeleton) {
    g_signal_connect(skeleton, "handle-echo", G_CALLBACK(handle_echo), this);
}


// Handlers (impls)
static gboolean handle_echo(EchoSkeleton *skeleton, GDBusMethodInvocation *invocation, const gchar* arg_input, EchoServiceImpl* self) {
    std::string result;
    try {
        result = self->echo(skeleton, invocation, std::string(arg_input));
    } catch (std::exception& e) {
        g_dbus_method_invocation_return_error(invocation, g_quark_from_string("ussur.wg.EchoError"), 1, "%s", e.what());
        return FALSE;
    }

    echo_service_complete_echo(skeleton, invocation, g_strdup(result.c_str()));
    return TRUE;
}

} // namespace wg
} // namespace ussur

