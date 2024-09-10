#include "dbus/dbus-shared.h"
#include <iostream>
#include <string>

#include <dbus/dbus.h>

#include <wg_broker/exceptions.hpp>
#include <wg_broker/dbus_connection.hpp>
#include <wg_broker/owned_bus_name.hpp>
#include <wg_broker/owned_ptr.hpp>

using ussur::wg::OwnedPtr;
using ussur::wg::OwnedBusName;


int main() {
    std::cout << "Program starts." << std::endl;

    DBusError error;
    dbus_error_init(&error);

    OwnedPtr<DBusConnection> connection = ussur::wg::establish_connection(DBUS_BUS_SESSION);
    std::cout << "Connection established: " << connection.get() << std::endl;

    OwnedBusName bus_name = OwnedBusName::acquire(connection.get(), "ussur.wg.Broker", DBUS_NAME_FLAG_DO_NOT_QUEUE);
    std::cout << "Bus name acquired: " << bus_name.name << std::endl;

    // Main loop
    while (true) {
        // Non-blocking read of the next message
        dbus_connection_read_write(connection.get(), 0);
        OwnedPtr<DBusMessage> message = ussur::wg::connection_pop_message(connection);
        
        if (message.get() == nullptr) {
            continue; // No message, loop
        }

        // Process the message if it's a method call
        // process_message(connection, message);
    }

    return 0;
}