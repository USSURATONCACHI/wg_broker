#include <wg_broker/skeleton.hpp>
#include <wg_broker/owned_ptr.hpp>

#include <functional>

#include <glib.h>
#include <gio/gio.h>

namespace ussur {
namespace wg {

void destruct_skeleton(void* skeleton) {
    if (skeleton) {
        g_dbus_interface_skeleton_unexport(G_DBUS_INTERFACE_SKELETON(skeleton));
        g_object_unref(skeleton);
    }
}

} // namespace wg
} // namespace ussur