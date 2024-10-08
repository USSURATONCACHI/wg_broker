/*
 * This file is generated by gdbus-codegen, do not modify it.
 *
 * The license of this code is the same as for the D-Bus interface description
 * it was derived from. Note that it links to GLib, so must comply with the
 * LGPL linking clauses.
 */

#ifndef __ECHO_SKELETON_H__
#define __ECHO_SKELETON_H__

#include <gio/gio.h>

G_BEGIN_DECLS


/* ------------------------------------------------------------------------ */
/* Declarations for ussur.wg.EchoService */

#define TYPE_ECHO_SERVICE (echo_service_get_type ())
#define ECHO_SERVICE(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_ECHO_SERVICE, EchoService))
#define IS_ECHO_SERVICE(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_ECHO_SERVICE))
#define ECHO_SERVICE_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), TYPE_ECHO_SERVICE, EchoServiceIface))

struct _EchoService;
typedef struct _EchoService EchoService;
typedef struct _EchoServiceIface EchoServiceIface;

struct _EchoServiceIface
{
  GTypeInterface parent_iface;

  gboolean (*handle_echo) (
    EchoService *object,
    GDBusMethodInvocation *invocation,
    const gchar *arg_input);

};

GType echo_service_get_type (void) G_GNUC_CONST;

GDBusInterfaceInfo *echo_service_interface_info (void);
guint echo_service_override_properties (GObjectClass *klass, guint property_id_begin);


/* D-Bus method call completion functions: */
void echo_service_complete_echo (
    EchoService *object,
    GDBusMethodInvocation *invocation,
    const gchar *output);



/* D-Bus method calls: */
void echo_service_call_echo (
    EchoService *proxy,
    const gchar *arg_input,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean echo_service_call_echo_finish (
    EchoService *proxy,
    gchar **out_output,
    GAsyncResult *res,
    GError **error);

gboolean echo_service_call_echo_sync (
    EchoService *proxy,
    const gchar *arg_input,
    gchar **out_output,
    GCancellable *cancellable,
    GError **error);



/* ---- */

#define TYPE_ECHO_SERVICE_PROXY (echo_service_proxy_get_type ())
#define ECHO_SERVICE_PROXY(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_ECHO_SERVICE_PROXY, EchoServiceProxy))
#define ECHO_SERVICE_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), TYPE_ECHO_SERVICE_PROXY, EchoServiceProxyClass))
#define ECHO_SERVICE_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_ECHO_SERVICE_PROXY, EchoServiceProxyClass))
#define IS_ECHO_SERVICE_PROXY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_ECHO_SERVICE_PROXY))
#define IS_ECHO_SERVICE_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_ECHO_SERVICE_PROXY))

typedef struct _EchoServiceProxy EchoServiceProxy;
typedef struct _EchoServiceProxyClass EchoServiceProxyClass;
typedef struct _EchoServiceProxyPrivate EchoServiceProxyPrivate;

struct _EchoServiceProxy
{
  /*< private >*/
  GDBusProxy parent_instance;
  EchoServiceProxyPrivate *priv;
};

struct _EchoServiceProxyClass
{
  GDBusProxyClass parent_class;
};

GType echo_service_proxy_get_type (void) G_GNUC_CONST;

#if GLIB_CHECK_VERSION(2, 44, 0)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (EchoServiceProxy, g_object_unref)
#endif

void echo_service_proxy_new (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
EchoService *echo_service_proxy_new_finish (
    GAsyncResult        *res,
    GError             **error);
EchoService *echo_service_proxy_new_sync (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);

void echo_service_proxy_new_for_bus (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
EchoService *echo_service_proxy_new_for_bus_finish (
    GAsyncResult        *res,
    GError             **error);
EchoService *echo_service_proxy_new_for_bus_sync (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);


/* ---- */

#define TYPE_ECHO_SERVICE_SKELETON (echo_service_skeleton_get_type ())
#define ECHO_SERVICE_SKELETON(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_ECHO_SERVICE_SKELETON, EchoServiceSkeleton))
#define ECHO_SERVICE_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), TYPE_ECHO_SERVICE_SKELETON, EchoServiceSkeletonClass))
#define ECHO_SERVICE_SKELETON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_ECHO_SERVICE_SKELETON, EchoServiceSkeletonClass))
#define IS_ECHO_SERVICE_SKELETON(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_ECHO_SERVICE_SKELETON))
#define IS_ECHO_SERVICE_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_ECHO_SERVICE_SKELETON))

typedef struct _EchoServiceSkeleton EchoServiceSkeleton;
typedef struct _EchoServiceSkeletonClass EchoServiceSkeletonClass;
typedef struct _EchoServiceSkeletonPrivate EchoServiceSkeletonPrivate;

struct _EchoServiceSkeleton
{
  /*< private >*/
  GDBusInterfaceSkeleton parent_instance;
  EchoServiceSkeletonPrivate *priv;
};

struct _EchoServiceSkeletonClass
{
  GDBusInterfaceSkeletonClass parent_class;
};

GType echo_service_skeleton_get_type (void) G_GNUC_CONST;

#if GLIB_CHECK_VERSION(2, 44, 0)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (EchoServiceSkeleton, g_object_unref)
#endif

EchoService *echo_service_skeleton_new (void);


G_END_DECLS

#endif /* __ECHO_SKELETON_H__ */
