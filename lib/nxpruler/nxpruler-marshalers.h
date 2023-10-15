
#ifndef ___nxpruler_marshal_MARSHAL_H__
#define ___nxpruler_marshal_MARSHAL_H__

#include	<glib-object.h>

G_BEGIN_DECLS

/* VOID:VOID (nxpruler-marshalers.list:24) */
#define _nxpruler_marshal_VOID__VOID	g_cclosure_marshal_VOID__VOID

/* VOID:BOOLEAN (nxpruler-marshalers.list:25) */
#define _nxpruler_marshal_VOID__BOOLEAN	g_cclosure_marshal_VOID__BOOLEAN

/* VOID:DOUBLE (nxpruler-marshalers.list:26) */
#define _nxpruler_marshal_VOID__DOUBLE	g_cclosure_marshal_VOID__DOUBLE

/* VOID:DOUBLE,DOUBLE (nxpruler-marshalers.list:27) */
extern void _nxpruler_marshal_VOID__DOUBLE_DOUBLE (GClosure     *closure,
                                                   GValue       *return_value,
                                                   guint         n_param_values,
                                                   const GValue *param_values,
                                                   gpointer      invocation_hint,
                                                   gpointer      marshal_data);

G_END_DECLS

#endif /* ___nxpruler_marshal_MARSHAL_H__ */

