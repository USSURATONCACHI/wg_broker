#pragma once

#include <dbus/dbus.h>
#include <string>

namespace ussur {
namespace wg {


struct OwnedBusName {
public:
    DBusConnection* connection; // To be able to free it later
    std::string name;
    
public:
    static OwnedBusName acquire(DBusConnection* conn, const std::string& name, unsigned int flags);
    ~OwnedBusName();
    OwnedBusName(OwnedBusName&& move_from);
    OwnedBusName& operator=(OwnedBusName&& move_from);

private:
    OwnedBusName() noexcept;
    OwnedBusName(DBusConnection* conn, std::string&& name) noexcept;
};


} // namespace wg
} // namespace ussur