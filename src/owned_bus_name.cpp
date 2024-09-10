#include <wg_broker/owned_bus_name.hpp>
#include <wg_broker/exceptions.hpp>

#include <iostream>

namespace ussur {
namespace wg {


    // DBusConnection* connection; // To be able to free it later
    // std::string name;
OwnedBusName OwnedBusName::acquire(DBusConnection* conn, const std::string& name, unsigned int flags) {
    DBusError error;
    dbus_error_init(&error);

    int ret = dbus_bus_request_name(conn, name.c_str(), flags, &error);
    if (dbus_error_is_set(&error)) {
        std::string error_msg = std::string("Name error: ") + error.message;

        dbus_error_free(&error);
        throw ussur::wg::BusNameException(error_msg);
    }
    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
        throw ussur::wg::BusNameException("Failed to request name (unknown reason)");
    }

    return OwnedBusName(conn, std::string(name));
}

OwnedBusName::~OwnedBusName() {
    DBusError error;
    dbus_error_init(&error);

    dbus_bus_release_name(connection, name.c_str(), &error);

    if (dbus_error_is_set(&error)) {
        std::cerr << "Release Name Error: " << error.message << std::endl;
        dbus_error_free(&error);
    }
}

OwnedBusName::OwnedBusName(OwnedBusName&& move_from) :
    connection(std::move(move_from.connection)),
    name(std::move(move_from.name))
{
    new (&move_from) OwnedBusName();
}

OwnedBusName& OwnedBusName::operator=(OwnedBusName&& move_from) {
    if (this != &move_from) {
        this->~OwnedBusName();
        new (this) OwnedBusName(std::move(move_from));
    }
    return *this;
}

OwnedBusName::OwnedBusName() noexcept :
    connection(nullptr),
    name()
{}

OwnedBusName::OwnedBusName(DBusConnection* conn, std::string&& name) noexcept :
    connection(conn),
    name(std::move(name))
{}


} // namespace wg
} // namespace ussur