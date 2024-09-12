#include "echo_skeleton.h"
#include "glib.h"
#include <iostream>
#include <string>

#include <gio/gio.h>

#include <wg_broker/dbus_connection.hpp>
#include <wg_broker/owned_bus_name.hpp>
#include <wg_broker/owned_ptr.hpp>
#include <wg_broker/echo_skeleton.hpp>
#include <wg_broker/echo_service_impl.hpp>

// extern "C" {
//     #include <wg_broker/gen/echo_skeleton.h>
// }

using ussur::wg::OwnedPtr;
using ussur::wg::OwnedBusName;
using ussur::wg::EchoServiceImpl;

static OwnedPtr<GMainLoop> create_main_loop();

int main() {
    const std::string BUS_NAME_STRING = "ussur.wg.Broker";
    const std::string OBJ_NAME_STRING = "/ussur/wg/Broker";

    std::cout << "Program starts." << std::endl;

    OwnedPtr<GDBusConnection> connection = ussur::wg::establish_connection(G_BUS_TYPE_SESSION);
    std::cout << "Connection established: " << connection.get() << std::endl;

    OwnedBusName bus_name = OwnedBusName::acquire(connection.get(), BUS_NAME_STRING,  G_BUS_NAME_OWNER_FLAGS_DO_NOT_QUEUE);
    std::cout << "Bus name acquired: " << bus_name.name << std::endl;

    OwnedPtr<GMainLoop> loop = create_main_loop();
    std::cout << "Main loop created: " << loop.get() << std::endl;
    
    EchoServiceImpl service;
    std::function<void(EchoService*)> connector = 
        [&service](EchoService* s) { 
            service.connect_skeleton_signals(s); 
        };

    OwnedPtr<EchoService> skeleton = ussur::wg::create_skeleton(connection.get(), OBJ_NAME_STRING, connector);
    std::cout << "Skeleton created: " << skeleton.get() << std::endl;

    g_main_loop_run(loop.get());

    return 0;
}

static OwnedPtr<GMainLoop> create_main_loop() {
    return OwnedPtr<GMainLoop>(
        g_main_loop_new(NULL, FALSE), 
        g_main_loop_unref
    );
}