#pragma once

#include <wg_broker/broker_service.hpp>

#include <simppl/dispatcher.h>
#include <simppl/skeleton.h>


namespace wg_broker {

class BrokerImpl : simppl::dbus::Skeleton<BrokerService> {

public:
    BrokerImpl(simppl::dbus::Dispatcher& disp, const std::string& busname, const std::string& object_path);

};


} // namespace wg_broker