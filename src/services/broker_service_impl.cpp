#include "glib.h"
#include <wg_broker/gen/broker_skeleton.h>
#include <wg_broker/services/broker_service_impl.hpp>

namespace ussur {
namespace wg {

using BrokerSkeleton = Broker;


// Handles (declarations)
static gboolean handle_get_profiles(BrokerSkeleton *skeleton, GDBusMethodInvocation *invocation, BrokerServiceImpl* self);


// Interface methods:

// struct ProfileStatus {
//     std::string name;
//     std::string content;
//     std::string log;
//     bool is_loaded;
//     bool is_startup;
//     bool has_error;
//     std::string error;
// };
using ProfileStatus = BrokerServiceImpl::ProfileStatus;
std::vector<ProfileStatus> 
    BrokerServiceImpl::get_profiles(BrokerSkeleton* skeleton, GDBusMethodInvocation *invocation) 
{
    std::vector<ProfileStatus> profiles;

    {   
        ProfileStatus p;
        p.name    = "Profile 1";
        p.content = "Sigma";
        p.log     = "Sigma log";
        p.error   = "";
        profiles.push_back(p);
    }
    {   
        ProfileStatus p;
        p.name    = "Profile 2";
        p.content = "Sigma";
        p.log     = "Sigma log";
        p.error   = "";
        profiles.push_back(p);
    }
    {   
        ProfileStatus p;
        p.name    = "Profile 3";
        p.content = "Sigma";
        p.log     = "Sigma log";
        p.error   = "";
        profiles.push_back(p);
    }

    return profiles;
}


// Helpers
CreateSkeletonInfo<BrokerSkeleton> BrokerServiceImpl::get_create_skeleton_info() {
    CreateSkeletonInfo<BrokerSkeleton> info;
    info.fn_create_skeleton = broker_skeleton_new;
    info.fn_connect_signals = std::bind(&BrokerServiceImpl::connect_skeleton_signals, this, std::placeholders::_1);
    return info;
}
void BrokerServiceImpl::connect_skeleton_signals(BrokerSkeleton* skeleton) {
    g_signal_connect(skeleton, "handle-get-profiles", G_CALLBACK(handle_get_profiles), this);

}


// Handlers (impls)
static GVariant* convert_profiles_to_gvariant(const std::vector<ProfileStatus>& profiles) {
    GVariantBuilder builder;
    g_variant_builder_init(&builder, G_VARIANT_TYPE_ARRAY);

    // Iterate over each ProfileStatus in the vector
    for (const auto& profile : profiles) {
        g_variant_builder_add(&builder, "(sssbbbs)",
            profile.name.c_str(),           // String name
            profile.content.c_str(),        // String content
            profile.log.c_str(),            // String log
            profile.is_loaded,              // Boolean is_loaded
            profile.is_startup,             // Boolean is_startup
            profile.has_error,              // Boolean has_error
            profile.error.c_str()           // String error
        );
    }

    // Return the array variant
    return g_variant_builder_end(&builder);
}

static gboolean handle_get_profiles(BrokerSkeleton *skeleton, GDBusMethodInvocation *invocation, BrokerServiceImpl* self) {
    GVariant* result = NULL;
    try {
        std::vector<ProfileStatus> profiles = self->get_profiles(skeleton, invocation);
        result = convert_profiles_to_gvariant(profiles);

    } catch (std::exception& e) {
        if (result) {
            g_variant_unref(result);
        }

        g_dbus_method_invocation_return_error(invocation, g_quark_from_string("ussur.wg.BrokerError"), 1, "%s", e.what());
        return FALSE;
    }

    broker_complete_get_profiles(skeleton, invocation, result);
    
    // Free the GVariant
    // if (result)
    //     g_variant_unref(result);

    return TRUE;
}



} // namespace wg
} // namespace ussur