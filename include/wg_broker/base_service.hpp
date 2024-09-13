#pragma once

#include <wg_broker/skeleton.hpp>

namespace ussur {
namespace wg {


template<typename C_Service>
class BaseService {
public:
    using ServiceType = C_Service;

    virtual CreateSkeletonInfo<ServiceType> get_create_skeleton_info() = 0;
    virtual void connect_skeleton_signals(ServiceType* skeleton) = 0;
};


} // namespace wg
} // namespace ussur