#include <iostream>

#include <optional>
#include <stdexcept>
#include <toml++/toml.hpp>
#include <wg_broker/whole_app.hpp>

struct Config {
    bool is_system_bus;
    std::string bus_name;
    std::string wg_profiles_dir;
    std::string required_group;
};

static void print_usage(const char* program_name);
static Config parse_config(const char* config_file);

template<typename T> 
T value_or_exception(toml::table& tbl, const std::string& field_name);

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "No config file specified." << std::endl;
        print_usage(argv[0]);
        return 1;
    }
    const char* config_file = argv[1];

    try {
        Config config = parse_config(config_file);
        
        ussur::wg::WholeApp app = ussur::wg::create_app();
        g_main_loop_run(app.loop.get());

    } catch (const toml::parse_error& err) {
        std::cerr << "Parsing failed:\n" << err << "\n";
        return 1;
    } catch (const std::runtime_error& err) {
        std::cerr << "Runtime error:\n" << err.what() << "\n";
        return 2; 
    }


    return 0;
}

template<typename T> 
T value_or_exception(toml::table& tbl, const std::string& field_name) {
    std::optional<T> maybe = tbl[field_name].value<T>();

    if (maybe.has_value()) {
        return *maybe;
    } else {
        throw std::runtime_error("Missing field " + field_name + " in the config");
    }
}

static Config parse_config(const char* config_file) {
    toml::table tbl = toml::parse_file(config_file);
    Config conf;

    conf.is_system_bus   = value_or_exception<bool>(tbl, "is_system_bus");
    conf.bus_name        = value_or_exception<std::string>(tbl, "bus_name");
    conf.wg_profiles_dir = value_or_exception<std::string>(tbl, "wg_profiles_dir");
    conf.required_group  = value_or_exception<std::string>(tbl, "required_group");

    return conf;
}

static void print_usage(const char* program_name) {
    std::cout << "Usage: " << program_name << " <config_file.toml>" << std::endl;
    std::cout << std::endl;
    std::cout << "Config must contain fields:" << std::endl;
    std::cout << "\t is_system_bus   - type = bool,   recommended = true" << std::endl;
    std::cout << "\t bus_name        - type = string, recommended = 'ussur.wg.Broker'" << std::endl;
    std::cout << "\t wg_profiles_dir - type = string, recommended = '/etc/wireguard'" << std::endl;
    std::cout << "\t required_group  - type = string, recommended = 'wireguarders'" << std::endl;
}
