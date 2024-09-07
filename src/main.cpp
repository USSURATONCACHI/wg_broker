#include <iostream>

#include <simppl/dispatcher.h>
#include <simppl/interface.h>
#include <simppl/skeleton.h>

namespace test {

INTERFACE(EchoService) 
{
    Method<simppl::dbus::in<std::string>, simppl::dbus::out<std::string>> echo;

    inline EchoService() : INIT(echo) {}
};

}   // namespace test

class MyEcho : simppl::dbus::Skeleton<test::EchoService> {
public:
    MyEcho(simppl::dbus::Dispatcher& disp)
       : simppl::dbus::Skeleton<test::EchoService>(disp, "ussur.wg.Broker", "/ussur/wg/Broker")
    {
        echo >> [this](const std::string& echo_string) {
            std::cout << "Client says '" << echo_string << "'." << std::endl;
            respond_with(echo(echo_string));
        };
    }
};

int main() {
    std::cout << "Creating dispatcher." << std::endl;
    simppl::dbus::Dispatcher disp("bus:session");

    std::cout << "Creating instance." << std::endl;
    MyEcho instance(disp);

    std::cout << "Running dispatcher." << std::endl;
    disp.run();

    return 0;
}