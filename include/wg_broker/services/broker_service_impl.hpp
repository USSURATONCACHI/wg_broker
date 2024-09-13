#pragma once

#include <filesystem>
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
    BrokerServiceImpl(std::filesystem::path wg_profiles_dir);

    virtual CreateSkeletonInfo<BrokerSkeleton> get_create_skeleton_info();
    virtual void connect_skeleton_signals(BrokerSkeleton* skeleton);

    struct ProfileStatus {
        std::string name;
        std::string content;
        std::string log;
        bool is_loaded;
        bool is_startup;
        bool has_error;
    };
    std::vector<ProfileStatus> get_profiles(BrokerSkeleton* skeleton, GDBusMethodInvocation *invocation);

private:
    std::filesystem::path wg_profiles_dir;
};


} // namespace wg
} // namespace ussur