/* -*- Mode: C; c-file-style: "stroustrup"; -*- */
#ifndef __NXP_RULER_ADJUSTMENT_H__
#define __NXP_RULER_ADJUSTMENT_H__

#include <glib-object.h>

#define NXP_RULER_ADJUSTMENT(object)            (G_TYPE_CHECK_INSTANCE_CAST ((object),\
				                 nxp_ruler_adjustment_get_type(), NxpRulerAdjustment))
#define NXP_RULER_ADJUSTMENT_CLASS(klass)       (G_TYPE_CHECK_CLASS_CAST ((klass),\
				                 nxp_ruler_adjustment_get_type(), NxpRulerAdjustmentClass))
#define NXP_IS_RULER_ADJUSTMENT(object)         (G_TYPE_CHECK_INSTANCE_TYPE ((object),\
				                 nxp_ruler_adjustment_get_type()))
#define NXP_IS_RULER_ADJUSTMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_TYPE ((klass),\
				                 nxp_ruler_adjustment_get_type()))
#define NXP_RULER_ADJUSTMENT_GET_CLASS(object)  (G_TYPE_INSTANCE_GET_CLASS ((object),\
				                 nxp_ruler_adjustment_get_type(), NxpRulerAdjustmentClass))
#define NXP_RULER_ADJUSTMENT_TYPE(object)       (G_TYPE_FROM_INSTANCE (object))
#define NXP_RULER_ADJUSTMENT_TYPE_NAME(object)  (g_type_name (NXP_RULER_ADJUSTMENT_TYPE (object)))
#define NXP_RULER_ADJUSTMENT_CLASS_TYPE(klass)  (G_TYPE_FROM_CLASS (klass))
#define NXP_RULER_ADJUSTMENT_CLASS_NAME(klass)  (g_type_name (NXP_RULER_ADJUSTMENT_CLASS_TYPE (klass)))
#define NXP_TYPE_RULER_ADJUSTMENT               (nxp_ruler_adjustment_get_type())


typedef struct _NxpRulerAdjustment      NxpRulerAdjustment;
typedef struct _NxpRulerAdjustmentClass NxpRulerAdjustmentClass;



struct _NxpRulerAdjustment
{
    GObject base;
    gdouble lower, upper;
    gboolean interaction_in_progress:1;
};


struct _NxpRulerAdjustmentClass
{
    GObjectClass base;

    void (*range_changed)         (NxpRulerAdjustment *self);
    void (*begin_interaction)	  (NxpRulerAdjustment *self);
    void (*interactive_translate) (NxpRulerAdjustment *self, gdouble delta);
    void (*interactive_scale)	  (NxpRulerAdjustment *self, gdouble origin, gdouble scale);
    void (*end_interaction)	  (NxpRulerAdjustment *self, gboolean commit);
};


GType               nxp_ruler_adjustment_get_type              (void) G_GNUC_CONST;
NxpRulerAdjustment* nxp_ruler_adjustment_new                   (gdouble             lower,
								gdouble             upper);
void                nxp_ruler_adjustment_set_range             (NxpRulerAdjustment *self,
								gdouble             lower,
								gdouble             upper);
void                nxp_ruler_adjustment_range_changed         (NxpRulerAdjustment *self);
void                nxp_ruler_adjustment_begin_interaction     (NxpRulerAdjustment *self);
void                nxp_ruler_adjustment_interactive_translate (NxpRulerAdjustment *self,
								gdouble             delta);
void                nxp_ruler_adjustment_interactive_scale     (NxpRulerAdjustment *self,
								gdouble             origin,
								gdouble             scale);
void                nxp_ruler_adjustment_end_interaction       (NxpRulerAdjustment *self,
								gboolean            commit);



#endif /* __NXP_RULER_ADJUSTMENT_H__ */
