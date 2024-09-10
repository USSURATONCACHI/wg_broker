#include <iostream>

#include <wg_broker/broker_service.hpp>
#include <wg_broker/broker_impl.hpp>

int main() {
    simppl::dbus::Dispatcher disp("bus:system");
    wg_broker::BrokerImpl instance(disp, "ussur.wg.broker", "/ussur/wg/broker");
    disp.run();

    return 0;
}