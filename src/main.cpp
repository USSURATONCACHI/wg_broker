#include "echo_skeleton.h"
#include "glib.h"
#include <iostream>
#include <string>

#include <gio/gio.h>

#include <wg_broker/dbus_connection.hpp>
#include <wg_broker/owned_bus_name.hpp>
#include <wg_broker/owned_ptr.hpp>
#include <wg_broker/skeleton.hpp>
#include <wg_broker/services/echo_service_impl.hpp>
#include <wg_broker/services/broker_service_impl.hpp>

// extern "C" {
//     #include <wg_broker/gen/echo_skeleton.h>
// }

using ussur::wg::OwnedPtr;
using ussur::wg::OwnedBusName;
using ussur::wg::EchoServiceImpl;
using ussur::wg::BrokerServiceImpl;

using ussur::wg::create_skeleton;

static OwnedPtr<GMainLoop> create_main_loop();

int main() {
    const std::string BUS_NAME_STRING = "ussur.wg.Broker";

    std::cout << "Program starts." << std::endl;

    OwnedPtr<GDBusConnection> connection = ussur::wg::establish_connection(G_BUS_TYPE_SESSION);
    std::cout << "Connection established: " << connection.get() << std::endl;

    OwnedBusName bus_name = OwnedBusName::acquire(connection.get(), BUS_NAME_STRING,  G_BUS_NAME_OWNER_FLAGS_DO_NOT_QUEUE);
    std::cout << "Bus name acquired: " << bus_name.name << std::endl;

    OwnedPtr<GMainLoop> loop = create_main_loop();
    std::cout << "Main loop created: " << loop.get() << std::endl;
    
    EchoServiceImpl echo_service;
    BrokerServiceImpl broker_service;

    OwnedPtr<ussur::wg::EchoSkeleton> echo_skeleton = 
        ussur::wg::create_skeleton<ussur::wg::EchoSkeleton>(
            connection.get(), 
            "/ussur/wg/Echo", 
            echo_service.get_create_skeleton_info()
        );
    std::cout << "Echo service created: " << echo_skeleton.get() << std::endl;

    OwnedPtr<ussur::wg::BrokerSkeleton> broker_skeleton = 
        ussur::wg::create_skeleton<ussur::wg::BrokerSkeleton>(
            connection.get(), 
            "/ussur/wg/Broker", 
            broker_service.get_create_skeleton_info()
        );
    std::cout << "Broker service created: " << broker_skeleton.get() << std::endl;

    g_main_loop_run(loop.get());

    return 0;
}

static OwnedPtr<GMainLoop> create_main_loop() {
    return OwnedPtr<GMainLoop>(
        g_main_loop_new(NULL, FALSE), 
        g_main_loop_unref
    );
}