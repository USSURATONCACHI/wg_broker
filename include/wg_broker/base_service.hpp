#pragma once

#include <string>
#include <gio/gio.h>
#include <wg_broker/skeleton.hpp>

namespace ussur {
namespace wg {


template<typename C_Service>
class BaseService {
public:
    using ServiceType = C_Service;

    virtual CreateSkeletonInfo<ServiceType> get_create_skeleton_info() = 0;
    virtual void connect_skeleton_signals(ServiceType* skeleton) = 0;

    virtual OwnedPtr<ServiceType> create_skeleton(GDBusConnection* conn, const std::string& object_name) {
        return ussur::wg::create_skeleton<ServiceType>(conn, object_name.c_str(), get_create_skeleton_info());
    }

    // non-movable and non-copyable, since we pass `this` pointer to glib callbacks
    BaseService() {}

    BaseService(const BaseService&) = delete;
    BaseService(BaseService&&) = delete;

    BaseService& operator=(const BaseService&) = delete;
    BaseService& operator=(BaseService&&) = delete;
};


} // namespace wg
} // namespace ussur