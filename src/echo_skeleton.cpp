#include <wg_broker/echo_skeleton.hpp>
#include <wg_broker/owned_ptr.hpp>
#include <wg_broker/echo_skeleton.hpp>

#include <functional>

#include <glib.h>
#include <gio/gio.h>
#include <wg_broker/exceptions.hpp>
#include <iostream>

namespace ussur {
namespace wg {

static void destruct_broker_skeleton(ExampleEchoService* skeleton);


OwnedPtr<ExampleEchoService> create_skeleton(
    GDBusConnection* connection, 
    const std::string& object_path,
    std::function<void(ExampleEchoService*)> callback_signal_connect
) {
    ExampleEchoService* skeleton = example_echo_service_skeleton_new();

    // if (callback_signal_connect)
    //     (*callback_signal_connect)(skeleton);
    callback_signal_connect(skeleton);
    
    GError *error = NULL;
    gboolean success = g_dbus_interface_skeleton_export(
        G_DBUS_INTERFACE_SKELETON(skeleton),
        connection,
        object_path.c_str(),
        &error
    );
    
    if (!success) {
        std::string msg = "Error exporting broker skeleton: " + std::string(error->message);
        g_error_free(error);
        throw SkeletonException(msg);
        // exit(1);
    }
    
    // Print some info for debugging purposes
    std::cout << "Exported an echo skeleton to object path: " << object_path <<  std::endl;

    return OwnedPtr<ExampleEchoService>(skeleton, destruct_broker_skeleton);
}

static void destruct_broker_skeleton(ExampleEchoService* skeleton) {
    if (skeleton) {
        g_dbus_interface_skeleton_unexport(G_DBUS_INTERFACE_SKELETON(skeleton));
        g_object_unref(skeleton);
    }
}

} // namespace wg
} // namespace ussur