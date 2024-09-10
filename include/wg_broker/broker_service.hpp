#pragma once

#include <simppl/interface.h>
#include <simppl/struct.h>

#include <wg_broker/service_state.hpp>


namespace wg_broker {


using simppl::dbus::in;
using simppl::dbus::out;

INTERFACE(BrokerService) 
{
    Method<
        out<std::vector<ServiceState>>
    > getAvailableServices;

    Method< in<std::string> > enableServiceOnStartup;
    Method< in<std::string> > disableServiceOnStartup;
    Method< in<std::string> > startService;
    Method< in<std::string> > stopService;

    Method< in<std::string> > addServiceFromFile;

    inline BrokerService() : 
        INIT(getAvailableServices),
        INIT(enableServiceOnStartup),
        INIT(disableServiceOnStartup),
        INIT(startService),
        INIT(stopService),
        INIT(addServiceFromFile)
    {}
};


}   // namespace wg_broker