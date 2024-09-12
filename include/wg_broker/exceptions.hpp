#pragma once

#include <stdexcept>

namespace ussur {
namespace wg {


struct ConnectionException : std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct BusNameException : std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct SkeletonException : std::runtime_error {
    using std::runtime_error::runtime_error;
};


} // namespace wg
} // namespace ussur