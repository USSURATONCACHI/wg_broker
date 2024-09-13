#pragma once

#include <gio/gio.h>

#include <wg_broker/owned_ptr.hpp>
#include <wg_broker/owned_bus_name.hpp>
#include <wg_broker/services/broker_service_impl.hpp>
#include <wg_broker/services/echo_service_impl.hpp>

namespace ussur {
namespace wg {

struct WholeApp {
    OwnedPtr<GDBusConnection> connection;
    OwnedBusName bus_name;
    OwnedPtr<GMainLoop> loop;

    BrokerServiceImpl broker_service;
    EchoServiceImpl echo_service;

    OwnedPtr<ussur::wg::BrokerSkeleton> broker_skeleton;
    OwnedPtr<ussur::wg::EchoSkeleton> echo_skeleton;
};

WholeApp create_app();

OwnedPtr<GMainLoop> create_main_loop();


} // namespace wg
} // namespace ussur