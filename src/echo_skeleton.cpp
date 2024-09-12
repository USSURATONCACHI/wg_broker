#include <wg_broker/echo_skeleton.hpp>

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>


namespace ussur {
namespace wg {

OwnedPtr<ExampleEchoService> create_skeleton(DBusConnection* connection, const std::string& object_path) {
    ExampleEchoService* skeleton = example_echo_service_skeleton_new();

    // g_signal_connect(skeleton, "skibidi-sigma", G_CALLBACK(), this);
    
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
        throw new std::runtime_error(msg);
        // exit(1);
    }
    
    // Print some info for debugging purposes
    std::cout << "Broker D-Bus service running at object path: " << m_object_path <<  std::endl;
}

} // namespace wg
} // namespace ussur