#pragma once

#include <string>
#include <filesystem>

namespace ussur {
namespace wg {


struct Config {
    bool is_system_bus;
    std::string bus_name;
    std::string wg_profiles_dir;
    std::string required_group;
};

Config parse_config(std::filesystem::path config_file);
void print_config(const Config& config, std::ostream& os);
void print_config_fields(std::ostream& os);


} // namespace wg
} // namespace ussur