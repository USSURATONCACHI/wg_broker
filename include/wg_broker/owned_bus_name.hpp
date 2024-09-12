#pragma once

#include <gio/gio.h>
#include <string>

namespace ussur {
namespace wg {


struct OwnedBusName {
public:
    GDBusConnection* connection; // To be able to free it later
    std::string name;
    
public:
    static OwnedBusName acquire(GDBusConnection* conn, const std::string& name, unsigned int flags);
    ~OwnedBusName();
    OwnedBusName(OwnedBusName&& move_from);
    OwnedBusName& operator=(OwnedBusName&& move_from);

private:
    OwnedBusName() noexcept;
    OwnedBusName(GDBusConnection* conn, std::string&& name) noexcept;
};


} // namespace wg
} // namespace ussur