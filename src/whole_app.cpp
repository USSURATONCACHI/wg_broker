#include <wg_broker/whole_app.hpp>

#include <memory>
#include <gio/gio.h>

#include <wg_broker/dbus_connection.hpp>
#include <wg_broker/services/echo_service_impl.hpp>
#include <wg_broker/services/broker/service_impl.hpp>

namespace ussur {
namespace wg {



WholeApp create_app(Config config) {
    std::cout << "Program starts." << std::endl;

    OwnedPtr<GDBusConnection> connection = ussur::wg::establish_connection(config.is_system_bus ? G_BUS_TYPE_SYSTEM : G_BUS_TYPE_SESSION);
    std::cout << "Connection established: " << connection.get() << std::endl;

    OwnedBusName bus_name = OwnedBusName::acquire(connection.get(), config.bus_name,  G_BUS_NAME_OWNER_FLAGS_DO_NOT_QUEUE);
    std::cout << "Bus name acquired: " << bus_name.name << std::endl;

    OwnedPtr<GMainLoop> loop = create_main_loop();
    std::cout << "Main loop created: " << loop.get() << std::endl;
    
    std::cout << "Directory: " << config.wg_profiles_dir << std::endl;
    std::cout << "When converting: " << std::filesystem::path(config.wg_profiles_dir) << std::endl;

    std::unique_ptr<broker::BrokerServiceImpl> broker_service = std::make_unique<broker::BrokerServiceImpl>(config.wg_profiles_dir);
    std::unique_ptr<EchoServiceImpl> echo_service = std::make_unique<EchoServiceImpl>();

    OwnedPtr<ussur::wg::EchoSkeleton> echo_skeleton = 
        echo_service->create_skeleton(connection.get(), "/ussur/wg/Echo");
    std::cout << "Echo service created: " << echo_skeleton.get() << std::endl;

    OwnedPtr<broker::BrokerSkeleton> broker_skeleton = 
        broker_service->create_skeleton(connection.get(), "/ussur/wg/Broker");
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