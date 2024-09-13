#include <iostream>

#include <ostream>
#include <stdexcept>
#include <toml++/toml.hpp>

#include <wg_broker/config.hpp>
#include <wg_broker/whole_app.hpp>

static void print_usage(const char* program_name, std::ostream& os);
static void inner_main(const char* config_file);

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "No config file specified." << std::endl;
        print_usage(argv[0], std::cerr);
        return 1;
    }
    const char* config_file = argv[1];

    try {
        inner_main(config_file);
    } catch (const toml::parse_error& err) {
        std::cerr << "Parsing failed:\n" << err << "\n";
        print_usage(argv[0], std::cerr);
        return 1;
    } catch (const std::runtime_error& err) {
        std::cerr << "Runtime error:\n" << err.what() << "\n";
        print_usage(argv[0], std::cerr);
        return 2; 
    }


    return 0;
}

static void inner_main(const char* config_file) {
    ussur::wg::Config config = ussur::wg::parse_config(config_file);
    std::cout << "Using config: ";
    ussur::wg::print_config(config, std::cout);
    
    ussur::wg::WholeApp app = ussur::wg::create_app();
    g_main_loop_run(app.loop.get());
}

static void print_usage(const char* program_name, std::ostream& os) {
    os << "Usage: " << program_name << " <config_file.toml>" << std::endl;
    os << std::endl;
    os << "Config must contain fields:" << std::endl;
    ussur::wg::print_config_fields(os);
}
