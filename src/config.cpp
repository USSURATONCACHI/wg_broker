#include <wg_broker/config.hpp>

#include <iostream>
#include <optional>
#include <toml++/toml.hpp>

namespace ussur {
namespace wg {


template<typename T> 
static T value_or_exception(toml::table& tbl, const std::string& field_name) {
    std::optional<T> maybe = tbl[field_name].value<T>();

    if (maybe.has_value()) {
        return *maybe;
    } else {
        throw std::runtime_error("Missing field " + field_name + " in the config");
    }
}



Config parse_config(std::filesystem::path config_file) {
    std::string path_string = config_file.string();
    toml::table tbl = toml::parse_file(path_string);
    Config conf;

    conf.is_system_bus   = value_or_exception<bool>(tbl, "is_system_bus");
    conf.bus_name        = value_or_exception<std::string>(tbl, "bus_name");
    conf.wg_profiles_dir = value_or_exception<std::string>(tbl, "wg_profiles_dir");
    conf.required_group  = value_or_exception<std::string>(tbl, "required_group");

    return conf;
}

void print_config(const Config& config, std::ostream& os) {
    os << "ussur::wg::Config {" << std::endl;
    os << "\t is_system_bus   =  " << config.is_system_bus << " ," << std::endl;
    os << "\t bus_name        = \"" << config.bus_name << "\"," << std::endl;
    os << "\t wg_profiles_dir = \"" << config.wg_profiles_dir << "\"," << std::endl;
    os << "\t required_group  = \"" << config.required_group << "\"," << std::endl;
    os << "}" << std::endl;
}

void print_config_fields(std::ostream& os) {
    os << "\t is_system_bus   - type = bool,   recommended = true" << std::endl;
    os << "\t bus_name        - type = string, recommended = 'ussur.wg.Broker'" << std::endl;
    os << "\t wg_profiles_dir - type = string, recommended = '/etc/wireguard'" << std::endl;
    os << "\t required_group  - type = string, recommended = 'wireguarders'" << std::endl;
}

} // namespace wg
} // namespace ussur