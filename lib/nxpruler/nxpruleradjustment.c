/* -*- Mode: C; c-file-style: "stroustrup"; -*- */
#include "nxpruleradjustment.h"
#include "nxpruler-marshalers.h"

enum {
    RANGE_CHANGED_SIGNAL,
    BEGIN_INTERACTION_SIGNAL,
    INTERACTIVE_TRANSLATE_SIGNAL,
    INTERACTIVE_SCALE_SIGNAL,
    END_INTERACTION_SIGNAL,
    LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 }; 
static gpointer parent_class = NULL;


static void nxp_ruler_adjustment_init            (NxpRulerAdjustment      *instance,
						  gpointer                 g_class);
static void nxp_ruler_adjustment_class_base_init (NxpRulerAdjustmentClass *class);
static void nxp_ruler_adjustment_class_init      (NxpRulerAdjustmentClass *class,
						  gpointer                 data);
/* static void nxp_ruler_adjustment_finalize        (GObject                 *object); */




static GTypeInfo typeinfo = 
{
    /* interface types, classed types, instantiated types */
    sizeof(NxpRulerAdjustmentClass), 

    (GBaseInitFunc)nxp_ruler_adjustment_class_base_init, 
    NULL, // GBaseFinalizeFunc      base_finalize;

    /* classed types, instantiated types */
    (GClassInitFunc)nxp_ruler_adjustment_class_init, 
    NULL, // GClassFinalizeFunc     class_finalize;
    NULL, // gconstpointer          class_data;

    /* instantiated types */
    sizeof(NxpRulerAdjustment),		//guint16                instance_size;
    0,					//guint16                n_preallocs;
    (GInstanceInitFunc)nxp_ruler_adjustment_init, //GInstanceInitFunc      instance_init;

    /* value handling */
    NULL,				//const GTypeValueTable	*value_table;
};


GType nxp_ruler_adjustment_get_type(void)
{
    static GType type = 0;
    if (!type)
	type = g_type_register_static(G_TYPE_OBJECT, "NxpRulerAdjustment",
				      &typeinfo, 0);
    return type;
}


static void nxp_ruler_adjustment_init(NxpRulerAdjustment *self, gpointer g_class)
{
    self->lower                   = 0;
    self->upper                   = 1;
    self->interaction_in_progress = FALSE;
}


static void nxp_ruler_adjustment_class_base_init(NxpRulerAdjustmentClass *class)
{
    /* empty */
}


static void nxp_ruler_adjustment_class_init(NxpRulerAdjustmentClass *class, gpointer data)
{
    GObjectClass *object_class = G_OBJECT_CLASS(class);

/*     object_class->finalize     = nxp_ruler_adjustment_finalize; */
    parent_class = g_type_class_peek_parent (class);
    

    signals[RANGE_CHANGED_SIGNAL] = 
	g_signal_new("range_changed",
		     G_OBJECT_CLASS_TYPE(object_class),
		     G_SIGNAL_RUN_LAST,
		     G_STRUCT_OFFSET(NxpRulerAdjustmentClass, range_changed),
		     NULL, NULL,
		     _nxpruler_marshal_VOID__VOID,
		     G_TYPE_NONE, 0);
    signals[BEGIN_INTERACTION_SIGNAL] = 
	g_signal_new("begin_interaction",
		     G_OBJECT_CLASS_TYPE(object_class),
		     G_SIGNAL_RUN_LAST,
		     G_STRUCT_OFFSET(NxpRulerAdjustmentClass, begin_interaction),
		     NULL, NULL,
		     _nxpruler_marshal_VOID__VOID,
		     G_TYPE_NONE, 0);
    signals[END_INTERACTION_SIGNAL] = 
	g_signal_new("end_interaction",
		     G_OBJECT_CLASS_TYPE(object_class),
		     G_SIGNAL_RUN_LAST,
		     G_STRUCT_OFFSET(NxpRulerAdjustmentClass, end_interaction),
		     NULL, NULL,
		     _nxpruler_marshal_VOID__BOOLEAN,
		     G_TYPE_NONE, 1, G_TYPE_BOOLEAN);
    signals[INTERACTIVE_TRANSLATE_SIGNAL] = 
	g_signal_new("interactive_translate",
		     G_OBJECT_CLASS_TYPE(object_class),
		     G_SIGNAL_RUN_LAST,
		     G_STRUCT_OFFSET(NxpRulerAdjustmentClass, interactive_translate),
		     NULL, NULL,
		     _nxpruler_marshal_VOID__DOUBLE,
		     G_TYPE_NONE, 1, G_TYPE_DOUBLE);
    signals[INTERACTIVE_SCALE_SIGNAL] = 
	g_signal_new("interactive_scale",
		     G_OBJECT_CLASS_TYPE(object_class),
		     G_SIGNAL_RUN_LAST,
		     G_STRUCT_OFFSET(NxpRulerAdjustmentClass, interactive_scale),
		     NULL, NULL,
		     _nxpruler_marshal_VOID__DOUBLE_DOUBLE,
		     G_TYPE_NONE, 2, G_TYPE_DOUBLE, G_TYPE_DOUBLE);
}


/* static void nxp_ruler_adjustment_shutdown(GObject *object) */
/* { */
/*     NxpRulerAdjustment *self; */
/*     self = NXP_RULER_ADJUSTMENT(object); */
/*     if (G_OBJECT_CLASS(parent_class)->finalize) */
/* 	G_OBJECT_CLASS(parent_class)->finalize(object); */
/* } */



void nxp_ruler_adjustment_range_changed(NxpRulerAdjustment *self)
{
    g_return_if_fail(self);
    g_return_if_fail(NXP_IS_RULER_ADJUSTMENT(self));
    g_signal_emit(G_OBJECT(self), signals[RANGE_CHANGED_SIGNAL], 0);
}


void nxp_ruler_adjustment_begin_interaction(NxpRulerAdjustment *self)
{
    g_return_if_fail(self);
    g_return_if_fail(NXP_IS_RULER_ADJUSTMENT(self));
    g_return_if_fail(!self->interaction_in_progress);
    self->interaction_in_progress = TRUE;
    g_signal_emit(G_OBJECT(self), signals[BEGIN_INTERACTION_SIGNAL], 0);
}


void nxp_ruler_adjustment_interactive_translate(NxpRulerAdjustment *self, gdouble delta)
{
    g_return_if_fail(self);
    g_return_if_fail(NXP_IS_RULER_ADJUSTMENT(self));
    g_return_if_fail(self->interaction_in_progress);
    g_signal_emit(G_OBJECT(self), signals[INTERACTIVE_TRANSLATE_SIGNAL], 0, delta);
}


void nxp_ruler_adjustment_interactive_scale(NxpRulerAdjustment *self, gdouble origin, gdouble scale)
{
    g_return_if_fail(self);
    g_return_if_fail(NXP_IS_RULER_ADJUSTMENT(self));
    g_return_if_fail(self->interaction_in_progress);
    g_signal_emit(G_OBJECT(self), signals[INTERACTIVE_SCALE_SIGNAL], 0, origin, scale);
}


void nxp_ruler_adjustment_end_interaction(NxpRulerAdjustment *self, gboolean commit)
{
    g_return_if_fail(self);
    g_return_if_fail(NXP_IS_RULER_ADJUSTMENT(self));
    g_return_if_fail(self->interaction_in_progress);
    self->interaction_in_progress = FALSE;
    g_signal_emit(G_OBJECT(self), signals[END_INTERACTION_SIGNAL], 0, commit);
}


void nxp_ruler_adjustment_set_range(NxpRulerAdjustment *self, gdouble lower, gdouble upper)
{
    g_return_if_fail(self);
    g_return_if_fail(NXP_IS_RULER_ADJUSTMENT(self));
    g_return_if_fail(!self->interaction_in_progress);
    self->lower = lower;
    self->upper = upper;
    nxp_ruler_adjustment_range_changed(self);
}


NxpRulerAdjustment*
nxp_ruler_adjustment_new(gdouble lower, gdouble upper)
{
    GTypeInstance *instance;
    NxpRulerAdjustment *self;

    instance = g_type_create_instance(NXP_TYPE_RULER_ADJUSTMENT);
    self = NXP_RULER_ADJUSTMENT(instance);
    self->lower = lower;
    self->upper = upper;

    return self;

}

