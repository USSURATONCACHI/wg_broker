#include <wg_broker/whole_app.hpp>

#include <wg_broker/dbus_connection.hpp>

namespace ussur {
namespace wg {



WholeApp create_app() {
    const std::string BUS_NAME_STRING = "ussur.wg.Broker";

    std::cout << "Program starts." << std::endl;

    OwnedPtr<GDBusConnection> connection = ussur::wg::establish_connection(G_BUS_TYPE_SYSTEM);
    std::cout << "Connection established: " << connection.get() << std::endl;

    OwnedBusName bus_name = OwnedBusName::acquire(connection.get(), BUS_NAME_STRING,  G_BUS_NAME_OWNER_FLAGS_DO_NOT_QUEUE);
    std::cout << "Bus name acquired: " << bus_name.name << std::endl;

    OwnedPtr<GMainLoop> loop = create_main_loop();
    std::cout << "Main loop created: " << loop.get() << std::endl;
    
    BrokerServiceImpl broker_service;
    EchoServiceImpl echo_service;

    OwnedPtr<ussur::wg::EchoSkeleton> echo_skeleton = 
        echo_service.create_skeleton(connection.get(), "/ussur/wg/Echo");
    std::cout << "Echo service created: " << echo_skeleton.get() << std::endl;

    OwnedPtr<ussur::wg::BrokerSkeleton> broker_skeleton = 
        broker_service.create_skeleton(connection.get(), "/ussur/wg/Broker");
    std::cout << "Broker service created: " << broker_skeleton.get() << std::endl;

    WholeApp app {
        .connection     = std::move(connection),
        .bus_name       = std::move(bus_name),
        .loop           = std::move(loop),
        .broker_service = std::move(broker_service),
        .echo_service   = std::move(echo_service),
        .broker_skeleton = std::move(broker_skeleton),
        .echo_skeleton  = std::move(echo_skeleton),
    };

    return app;
}

OwnedPtr<GMainLoop> create_main_loop() {
    return OwnedPtr<GMainLoop>(
        g_main_loop_new(NULL, FALSE), 
        g_main_loop_unref
    );
}


} // namespace wg
} // namespace ussur