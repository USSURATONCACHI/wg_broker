#include "glib.h"
#include <iostream>
#include <string>

#include <gio/gio.h>

#include <wg_broker/dbus_connection.hpp>
#include <wg_broker/owned_bus_name.hpp>
#include <wg_broker/owned_ptr.hpp>

// extern "C" {
//     #include <wg_broker/gen/echo_skeleton.h>
// }

using ussur::wg::OwnedPtr;
using ussur::wg::OwnedBusName;

static OwnedPtr<GMainLoop> create_main_loop();

int main() {
    std::cout << "Program starts." << std::endl;

    OwnedPtr<GDBusConnection> connection = ussur::wg::establish_connection(G_BUS_TYPE_SESSION);
    std::cout << "Connection established: " << connection.get() << std::endl;

    OwnedBusName bus_name = OwnedBusName::acquire(connection.get(), "ussur.wg.Broker",  G_BUS_NAME_OWNER_FLAGS_DO_NOT_QUEUE);
    std::cout << "Bus name acquired: " << bus_name.name << std::endl;

    OwnedPtr<GMainLoop> loop = create_main_loop();
    std::cout << "Main loop created: " << loop.get() << std::endl;

    g_main_loop_run(loop.get());

    return 0;
}

static OwnedPtr<GMainLoop> create_main_loop() {
    return OwnedPtr<GMainLoop>(
        g_main_loop_new(NULL, FALSE), 
        g_main_loop_unref
    );
}