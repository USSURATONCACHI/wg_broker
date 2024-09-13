#pragma once

#include <string>
#include <vector>

#include <gio/gio.h>

#include <wg_broker/base_service.hpp>

extern "C" {
    #include <wg_broker/gen/broker_skeleton.h>
}


namespace ussur {
namespace wg {

using BrokerSkeleton = Broker;

class BrokerServiceImpl : public BaseService<BrokerSkeleton> {
public:

    virtual CreateSkeletonInfo<BrokerSkeleton> get_create_skeleton_info();
    virtual void connect_skeleton_signals(BrokerSkeleton* skeleton);

    struct ProfileStatus {
        std::string name;
        std::string content;
        std::string log;
        bool is_loaded;
        bool is_startup;
        bool has_error;
        std::string error;
    };
    std::vector<ProfileStatus> get_profiles(BrokerSkeleton* skeleton, GDBusMethodInvocation *invocation);
};


} // namespace wg
} // namespace ussur