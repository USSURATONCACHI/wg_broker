#include <wg_broker/services/broker/profile_status.hpp>

namespace ussur {
namespace wg {
namespace broker { 

ProfileStatus::ProfileStatus() : 
    filepath(),
    name(),
    content(),
    log(),
    is_loaded(false),
    is_startup(false),
    has_error(false)
{}

} // namespace broker
} // namespace wg
} // namespace ussur