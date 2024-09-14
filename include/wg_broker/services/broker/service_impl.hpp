#pragma once

#include <filesystem>

#include <gio/gio.h>

#include <wg_broker/base_service.hpp>
#include <wg_broker/services/broker/profile_status.hpp>

extern "C" {
    #include <wg_broker/gen/broker_skeleton.h>
}


namespace ussur {
namespace wg {
namespace broker {

using BrokerSkeleton = Broker;

class BrokerServiceImpl : public BaseService<BrokerSkeleton> {
public:
    BrokerServiceImpl(std::filesystem::path wg_profiles_dir);

    virtual CreateSkeletonInfo<BrokerSkeleton> get_create_skeleton_info();
    virtual void connect_skeleton_signals(BrokerSkeleton* skeleton);

    std::vector<ProfileStatus> get_profiles(BrokerSkeleton* skeleton, GDBusMethodInvocation *invocation);

private:
    std::filesystem::path wg_profiles_dir;
};


} // namespace broker
} // namespace wg
} // namespace ussur