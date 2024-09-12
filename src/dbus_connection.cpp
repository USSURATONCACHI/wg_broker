#include "wg_broker/owned_ptr.hpp"

#include <wg_broker/dbus_connection.hpp>
#include <wg_broker/exceptions.hpp>


namespace ussur {
namespace wg {


OwnedPtr<GDBusConnection> establish_connection(GBusType type) {
    GError *error = NULL;
    GDBusConnection *connection = g_bus_get_sync(type, NULL, &error);

    if (error) {
        std::string error_msg = std::string("Connection error: ") + error->message;
        g_error_free(error);
        throw ussur::wg::ConnectionException(error_msg);
    }

    return OwnedPtr<GDBusConnection>(std::move(connection), g_object_unref);
}


} // namespace wg
} // namespace ussur
