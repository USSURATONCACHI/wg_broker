#include <wg_broker/broker_impl.hpp>

#include <simppl/dispatcher.h>
#include <simppl/skeleton.h>

using simppl::dbus::Skeleton;
using simppl::dbus::Dispatcher;

namespace wg_broker {


BrokerImpl::BrokerImpl(Dispatcher& disp, const std::string& busname, const std::string& object_path)
    : Skeleton<BrokerService>(disp, busname, object_path)
{
    // echo >> [this](const std::string& echo_string) {
    //     std::cout << "Client says '" << echo_string << "'." << std::endl;
    //     respond_with(echo(echo_string));
    // };
}


} // namespace wg_broker