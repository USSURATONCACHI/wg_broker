#pragma once

#include <filesystem>
#include <string>
#include <optional>

#include <gio/gio.h>

#include <wg_broker/services/broker/profile_status.hpp>

namespace ussur {
namespace wg {
namespace broker {

std::optional<std::string> systemd_get_unit(GDBusConnection* conn, std::string name);
std::string systemd_get_unit_property_string(GDBusConnection* conn, std::string object_path, std::string property_path);
bool systemd_check_if_unit_on_startup(GDBusConnection* conn, std::string unit_name);
ProfileStatus get_profile_status(GDBusConnection *connection, std::filesystem::path filepath, std::string profile_name);

} // namespace broker
} // namespace wg
} // namespace ussur