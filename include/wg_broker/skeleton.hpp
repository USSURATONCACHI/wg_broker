#pragma once

#include <string>
#include <functional>
#include <optional>
#include <iostream>

#include <gio/gio.h>

#include <wg_broker/exceptions.hpp>
#include <wg_broker/owned_ptr.hpp>

namespace ussur {
namespace wg {


void destruct_skeleton(void* skeleton);

template<typename T>
using OptFn = std::optional<std::function<T>>;

template<typename T>
struct CreateSkeletonInfo {
    std::function<T*()> fn_create_skeleton;
    OptFn<void(T*)> fn_connect_signals;
};


template<typename T>
OwnedPtr<T> create_skeleton(
    GDBusConnection* connection, 
    const std::string& object_path,
    CreateSkeletonInfo<T> create_info
) {
    T* skeleton = create_info.fn_create_skeleton();

    if (create_info.fn_connect_signals.has_value())
        (*create_info.fn_connect_signals)(skeleton);
    
    GError *error = NULL;
    gboolean success = g_dbus_interface_skeleton_export(
        G_DBUS_INTERFACE_SKELETON(skeleton),
        connection,
        object_path.c_str(),
        &error
    );
    
    if (!success) {
        std::string msg = "Error exporting broker skeleton: " + std::string(error->message);
        g_error_free(error);
        throw SkeletonException(msg);
        // exit(1);
    }
    
    // Print some info for debugging purposes
    std::cout << "Exported an echo skeleton to object path: " << object_path <<  std::endl;

    return OwnedPtr<T>(skeleton, destruct_skeleton);
}



} // namespace wg
} // namespace ussur