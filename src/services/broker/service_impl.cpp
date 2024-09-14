#include <wg_broker/services/broker/service_impl.hpp>

#include <exception>

#include <glib.h>
#include <gio/gio.h>

#include <wg_broker/gen/broker_skeleton.h>
#include <wg_broker/util.hpp>
#include <wg_broker/services/broker/service_util.hpp>

namespace fs = std::filesystem;


namespace ussur {
namespace wg {
namespace broker { 

using BrokerSkeleton = Broker;
using ProfileStatus = broker::ProfileStatus;

// Local helpers

// Handles (declarations)
static gboolean handle_get_profiles(BrokerSkeleton *skeleton, GDBusMethodInvocation *invocation, BrokerServiceImpl* self);


// Interface methods:
BrokerServiceImpl::BrokerServiceImpl(std::filesystem::path wg_profiles_dir)
    : wg_profiles_dir(wg_profiles_dir)
{}

std::vector<ProfileStatus> 
    BrokerServiceImpl::get_profiles(BrokerSkeleton* skeleton, GDBusMethodInvocation *invocation) 
{
    std::vector<ProfileStatus> profiles;
    GDBusConnection *connection = g_dbus_method_invocation_get_connection(invocation);

    for (const fs::directory_entry& entry : fs::directory_iterator(wg_profiles_dir)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename();
            const std::string FILE_TYPE = ".conf";
            if (!filename.ends_with(FILE_TYPE)) 
                continue;
            
            std::string profile_name = filename.substr(0, filename.length() - FILE_TYPE.length());
            profiles.push_back(
                broker::get_profile_status(connection, entry.path(), profile_name)
            );
        }
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
        g_variant_builder_add(&builder, "(sssbbb)",
            profile.name.c_str(),           // String name
            profile.content.c_str(),        // String content
            profile.log.c_str(),            // String log
            profile.is_loaded,              // Boolean is_loaded
            profile.is_startup,             // Boolean is_startup
            profile.has_error               // Boolean has_error
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

} // namespace broker
} // namespace wg
} // namespace ussur