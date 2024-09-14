#pragma once

#include <string>
#include <filesystem>

namespace ussur {
namespace wg {
namespace broker { 

struct ProfileStatus {
    std::filesystem::path filepath;
    std::string name;
    std::string content;
    std::string log;
    bool is_loaded;
    bool is_startup;
    bool has_error;

    ProfileStatus();
};

} // namespace broker
} // namespace wg
} // namespace ussur