#pragma once

#include <memory>

#include <gio/gio.h>

#include <wg_broker/owned_ptr.hpp>
#include <wg_broker/owned_bus_name.hpp>
#include <wg_broker/services/broker/service_impl.hpp>
#include <wg_broker/services/echo_service_impl.hpp>

#include <wg_broker/config.hpp>

namespace ussur {
namespace wg {

struct WholeApp {
    OwnedPtr<GDBusConnection> connection;
    OwnedBusName bus_name;
    OwnedPtr<GMainLoop> loop;

    std::unique_ptr<broker::BrokerServiceImpl> broker_service;
    std::unique_ptr<EchoServiceImpl> echo_service;

    OwnedPtr<broker::BrokerSkeleton> broker_skeleton;
    OwnedPtr<ussur::wg::EchoSkeleton> echo_skeleton;
};

WholeApp create_app(Config config);

OwnedPtr<GMainLoop> create_main_loop();


} // namespace wg
} // namespace ussur