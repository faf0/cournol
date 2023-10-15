/* -*- Mode: C; c-file-style: "gnu"; -*- */
/* GTK - The GIMP Toolkit
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 * Modified by the GTK+ Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/. 
 */

/* 
 *  Modified by Gustavo Carneiro <gustavo@users.sourceforge.net>
 *  Original file: gtkruler.c, version 1.26
 */
#define _GNU_SOURCE

#include "nxpruler.h"
#include "nxphruler.h"
#include "nxpvruler.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
/* #include "gtkintl.h" */
#define  _(s) (s)
#define N_(s) (s)



static void nxp_ruler_class_init        (NxpRulerClass    *klass);
static void nxp_ruler_init              (NxpRuler         *ruler);
static void nxp_ruler_realize           (GtkWidget        *widget);
static void nxp_ruler_unrealize         (GtkWidget        *widget);
static void nxp_ruler_size_allocate     (GtkWidget        *widget,
					 GtkAllocation    *allocation);
static gint nxp_ruler_expose            (GtkWidget        *widget,
					 GdkEventExpose   *event);
static void nxp_ruler_make_pixmap       (NxpRuler         *ruler);
static gint nxp_ruler_enter_notify      (GtkWidget        *widget,
					 GdkEventCrossing *event);
static gint nxp_ruler_leave_notify      (GtkWidget        *widget,
					 GdkEventCrossing *event);
static void nxp_ruler_remove_adjustment (NxpRuler         *ruler);
static void nxp_ruler_destroy           (GtkObject        *object);






static GtkWidgetClass *parent_class;


GtkType
nxp_ruler_get_type (void)
{
  static GtkType ruler_type = 0;

  if (!ruler_type)
    {
      static const GtkTypeInfo ruler_info =
      {
	"NxpRuler",
	sizeof (NxpRuler),
	sizeof (NxpRulerClass),
	(GtkClassInitFunc) nxp_ruler_class_init,
	(GtkObjectInitFunc) nxp_ruler_init,
	/* reserved_1 */ NULL,
        /* reserved_2 */ NULL,
        (GtkClassInitFunc) NULL,
      };

      ruler_type = gtk_type_unique (GTK_TYPE_WIDGET, &ruler_info);
    }

  return ruler_type;
}

static void
nxp_ruler_class_init (NxpRulerClass *class)
{
  GObjectClass   *gobject_class;
  GtkObjectClass *object_class;
  GtkWidgetClass *widget_class;
  int i;

  gobject_class = G_OBJECT_CLASS (class);
  object_class = (GtkObjectClass*) class;
  widget_class = (GtkWidgetClass*) class;

  parent_class = gtk_type_class (GTK_TYPE_WIDGET);
  
  widget_class->realize = nxp_ruler_realize;
  widget_class->unrealize = nxp_ruler_unrealize;
  widget_class->size_allocate = nxp_ruler_size_allocate;
  widget_class->expose_event = nxp_ruler_expose;
  widget_class->enter_notify_event = nxp_ruler_enter_notify;
  widget_class->leave_notify_event = nxp_ruler_leave_notify;

  class->draw_ticks = NULL;
  class->draw_pos = NULL;

  object_class->destroy = nxp_ruler_destroy;

  for (i = 2; i < 10; i++)
    class->log29[i - 2] = log10((gdouble) i);

}

static void
nxp_ruler_init (NxpRuler *ruler)
{
  ruler->backing_store = NULL;
  ruler->non_gr_exp_gc = NULL;
  ruler->xsrc = 0;
  ruler->ysrc = 0;
  ruler->slider_size = 0;
  ruler->lower = 0;
  ruler->upper = 0;
  ruler->position = 0;
/*   ruler->max_size = 0; */
  ruler->adjustment = NULL;
}


void
nxp_ruler_draw_ticks (NxpRuler *ruler)
{
  g_return_if_fail (NXP_IS_RULER (ruler));
  if (ruler->backing_store == NULL) return;
  if (NXP_RULER_GET_CLASS (ruler)->draw_ticks)
    NXP_RULER_GET_CLASS (ruler)->draw_ticks (ruler);
}

void
nxp_ruler_draw_pos (NxpRuler *ruler)
{
  g_return_if_fail (NXP_IS_RULER (ruler));

  if (NXP_RULER_GET_CLASS (ruler)->draw_pos)
     NXP_RULER_GET_CLASS (ruler)->draw_pos (ruler);
}


static void
nxp_ruler_realize (GtkWidget *widget)
{
  NxpRuler *ruler;
  GdkWindowAttr attributes;
  gint attributes_mask;

  ruler = NXP_RULER (widget);
  GTK_WIDGET_UNSET_FLAGS(widget, GTK_DOUBLE_BUFFERED);
  GTK_WIDGET_SET_FLAGS (ruler, GTK_REALIZED);

  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.x = widget->allocation.x;
  attributes.y = widget->allocation.y;
  attributes.width = widget->allocation.width;
  attributes.height = widget->allocation.height;
  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes.visual = gtk_widget_get_visual (widget);
  attributes.colormap = gtk_widget_get_colormap (widget);
  attributes.event_mask = gtk_widget_get_events (widget);
  attributes.event_mask |= ( GDK_EXPOSURE_MASK |
			     GDK_POINTER_MOTION_MASK |
			     GDK_POINTER_MOTION_HINT_MASK |
			     GDK_BUTTON_PRESS_MASK |
			     GDK_BUTTON_RELEASE_MASK |
			     GDK_ENTER_NOTIFY_MASK |
			     GDK_LEAVE_NOTIFY_MASK );

  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;

  widget->window = gdk_window_new (gtk_widget_get_parent_window (widget), &attributes, attributes_mask);
  gdk_window_set_user_data (widget->window, ruler);

  widget->style = gtk_style_attach (widget->style, widget->window);
  gtk_style_set_background (widget->style, widget->window, GTK_STATE_ACTIVE);

  nxp_ruler_make_pixmap (ruler);
}

static void
nxp_ruler_unrealize (GtkWidget *widget)
{
  NxpRuler *ruler = NXP_RULER (widget);

  if (ruler->backing_store)
    gdk_pixmap_unref (ruler->backing_store);
  if (ruler->non_gr_exp_gc)
    gdk_gc_destroy (ruler->non_gr_exp_gc);

  ruler->backing_store = NULL;
  ruler->non_gr_exp_gc = NULL;

  if (GTK_WIDGET_CLASS (parent_class)->unrealize)
    (* GTK_WIDGET_CLASS (parent_class)->unrealize) (widget);
}

static void
nxp_ruler_size_allocate (GtkWidget     *widget,
			 GtkAllocation *allocation)
{
  NxpRuler *ruler = NXP_RULER (widget);

  widget->allocation = *allocation;

  if (GTK_WIDGET_REALIZED (widget))
    {
      gdk_window_move_resize (widget->window,
			      allocation->x, allocation->y,
			      allocation->width, allocation->height);

      nxp_ruler_make_pixmap (ruler);
    }
}

static gint
nxp_ruler_expose (GtkWidget      *widget,
		  GdkEventExpose *event)
{
  NxpRuler *ruler;

  if (GTK_WIDGET_DRAWABLE (widget))
    {
      ruler = NXP_RULER (widget);

      if (ruler->needs_redraw)
          nxp_ruler_draw_ticks(ruler);
      gdk_draw_pixmap (widget->window,
		       ruler->non_gr_exp_gc,
		       ruler->backing_store,
		       event->area.x, event->area.y,
		       event->area.x, event->area.y, 
		       event->area.width,
		       event->area.height);
      if (!ruler->dragging)
        nxp_ruler_draw_pos (ruler);
      ruler->needs_redraw = FALSE;
    }

  return FALSE;
}

static void
nxp_ruler_make_pixmap (NxpRuler *ruler)
{
  GtkWidget *widget;
  gint width;
  gint height;

  widget = GTK_WIDGET (ruler);

  if (ruler->backing_store)
    {
      gdk_window_get_size (ruler->backing_store, &width, &height);
      if ((width == widget->allocation.width) &&
	  (height == widget->allocation.height))
	return;

      gdk_pixmap_unref (ruler->backing_store);
    }

  ruler->backing_store = gdk_pixmap_new (widget->window,
					 widget->allocation.width,
					 widget->allocation.height,
					 -1);

  ruler->xsrc = 0;
  ruler->ysrc = 0;

  if (!ruler->non_gr_exp_gc)
    {
      ruler->non_gr_exp_gc = gdk_gc_new (widget->window);
      gdk_gc_set_exposures (ruler->non_gr_exp_gc, FALSE);
    }
  ruler->needs_redraw = TRUE;
  gtk_widget_queue_draw(GTK_WIDGET(ruler));
}


static gint nxp_ruler_enter_notify(GtkWidget *widget, GdkEventCrossing *event)
{
    NxpRuler *ruler;
    g_return_val_if_fail(NXP_IS_RULER(widget),0);
    ruler = (NxpRuler *) widget;
    gtk_widget_set_state(widget, GTK_STATE_PRELIGHT);
    ruler->needs_redraw = TRUE;
    gtk_widget_queue_draw(widget);
    //g_print(__FUNCTION__ "\n");
    return TRUE;
}

static gint nxp_ruler_leave_notify(GtkWidget *widget, GdkEventCrossing *event)
{
    NxpRuler *ruler;
    g_return_val_if_fail(NXP_IS_RULER(widget),0);
    ruler = (NxpRuler *) widget;
    gtk_widget_set_state(widget, GTK_STATE_NORMAL);
    ruler->needs_redraw = TRUE;
    gtk_widget_queue_draw(widget);
    //g_print(__FUNCTION__ "\n");
    return TRUE;
}



void
nxp_ruler_compute_ticks(NxpRuler *ruler)
{
  GtkWidget *widget;
  GdkGC *gc, *bg_gc;
  gint width, height, xthickness, ythickness, size = -1;
  gchar unit_str[32];
  gint digit_height;
  gint text_width, text_width1, text_width2;
  gdouble tbe, div, x0, xi, xf, dx, k;
  gint ndiv;
  PangoLayout *layout;
  PangoRectangle logical_rect, ink_rect;
  char orientation = '-';

  //g_return_if_fail (ruler != NULL);
  //g_return_if_fail (GTK_IS_MYHRULER (ruler));

  widget = GTK_WIDGET(ruler);

  gc = widget->style->fg_gc[GTK_STATE_NORMAL];
  bg_gc = widget->style->bg_gc[GTK_STATE_NORMAL];

  xthickness = widget->style->xthickness;
  ythickness = widget->style->ythickness;

  layout = gtk_widget_create_pango_layout(widget, "012456789E.+-");
  pango_layout_get_extents(layout, &ink_rect, &logical_rect);

  digit_height = PANGO_PIXELS(ink_rect.height) + 1;

  if (g_type_is_a(G_OBJECT_TYPE(ruler), NXP_TYPE_HRULER))
    orientation = 'h';
  else if (g_type_is_a(G_OBJECT_TYPE(ruler), NXP_TYPE_VRULER))
    orientation = 'v';
  else g_assert_not_reached();

  if (orientation == 'h') {
    width = widget->allocation.width;
    height = widget->allocation.height - ythickness * 2;
    size = width;
  } else if (orientation == 'v') {
    width = widget->allocation.height;
    height = widget->allocation.width - ythickness * 2;
    size = width;
  } else g_assert_not_reached();

  xf = ruler->upper;
  xi = ruler->lower;
  dx = xf - xi;
  g_return_if_fail(dx > 0);

  switch (ruler->mode)
    {
      gint n;
    case NXP_RULER_LINEAR:
      ndiv = 10;
      for (n = 5; n; n--) { // iterate 5 times to find optimum division size
	/* div: length of each division */
	tbe = log10(dx / ndiv);   /* looking for approx. 'ndiv' divisions in a length 'dx' */
	div = pow(10, rint(tbe));	/* div: power of 10 closest to dx/ndiv */
	if (fabs(div / 2 - dx / ndiv) < fabs(div - dx / ndiv)) /* test if div/2 is closer to dx/ndiv */
	  div /= 2;
	else if (fabs(div*2 - dx / ndiv) < fabs(div - dx / ndiv))
	  div *= 2;			/* test if div*2 is closer to dx/ndiv */
	x0 = div*ceil(xi / div) - div;
	sprintf(unit_str, "  %G ", x0 + div);
	text_width1 = strlen(unit_str) * digit_height + 1;
	sprintf(unit_str, "  %G ", x0 + ndiv*div);
	text_width2 = strlen(unit_str) * digit_height + 1;
	text_width = MAX(text_width1, text_width2);
	if (n > 1) ndiv = rint(size / text_width);
      }

      ruler->rtick_start = x0;
      ruler->rtick_div = div;
      ruler->rtick_subdiv = div / 5;
      ruler->rtick_limit = ruler->upper;
      k = size / dx;
      ruler->tick_start = (x0 - xi)*k;
      ruler->tick_div = div*k;
      ruler->tick_subdiv = ruler->rtick_subdiv*k;
      ruler->tick_limit = size;
      break;
    case NXP_RULER_LOG:
      x0 = ceil(xi) - 1;
      div = 1.0;
      ruler->rtick_start = x0;
      ruler->rtick_div = div;
      ruler->rtick_subdiv = 0;
      ruler->rtick_limit = ruler->upper;
      k = size / dx;
      ruler->tick_start = (x0 - xi)*k;
      ruler->tick_div = /* div* */k;
      ruler->tick_subdiv = 0;
      ruler->tick_limit = size;
      break;
    }
  g_object_unref(G_OBJECT(layout));
}



static void nxp_ruler_remove_adjustment(NxpRuler *ruler)
{
  if (!ruler->adjustment) return;
  g_signal_handler_disconnect(ruler->adjustment, ruler->adj_signals.range_changed);
  g_signal_handler_disconnect(ruler->adjustment, ruler->adj_signals.begin_interaction);
  g_signal_handler_disconnect(ruler->adjustment, ruler->adj_signals.interactive_translate);
  g_signal_handler_disconnect(ruler->adjustment, ruler->adj_signals.interactive_scale);
  g_signal_handler_disconnect(ruler->adjustment, ruler->adj_signals.end_interaction);
  g_object_unref(ruler->adjustment);
  ruler->adjustment = NULL;
}


static void nxp_ruler_destroy(GtkObject *object)
{
  NxpRuler *ruler = NXP_RULER(object);

  nxp_ruler_remove_adjustment(ruler);

  if (GTK_OBJECT_CLASS(parent_class)->destroy)
    GTK_OBJECT_CLASS(parent_class)->destroy(object);
}


static void _set_range(NxpRuler *ruler, gdouble lower, gdouble upper)
{
  ruler->lower = lower;
  ruler->upper = upper;
  ruler->needs_redraw = TRUE;
  gtk_widget_queue_draw(GTK_WIDGET(ruler));
}



static void nxp_ruler_range_changed(NxpRulerAdjustment *adj, NxpRuler *ruler)
{
  g_assert(NXP_IS_RULER(ruler));
  g_assert(NXP_IS_RULER_ADJUSTMENT(adj));
  g_assert(adj == ruler->adjustment);
  if (adj->lower != ruler->lower || adj->upper != ruler->upper)
    _set_range(ruler, adj->lower, adj->upper);
}

static void nxp_ruler_begin_interaction(NxpRulerAdjustment *adj, NxpRuler *ruler)
{
  g_assert(NXP_IS_RULER_ADJUSTMENT(adj));
  g_assert(NXP_IS_RULER(ruler));
  g_assert(adj == ruler->adjustment);
  ruler->lower_save = ruler->lower;
  ruler->upper_save = ruler->upper;
}

static void nxp_ruler_interactive_translate(NxpRulerAdjustment *adj, gdouble delta, NxpRuler *ruler)
{
  g_assert(NXP_IS_RULER(ruler));
  g_assert(NXP_IS_RULER_ADJUSTMENT(adj));
  g_assert(adj == ruler->adjustment);
  _set_range(ruler, ruler->lower_save + delta, ruler->upper_save + delta);
}


static void nxp_ruler_interactive_scale(NxpRulerAdjustment *adj, gdouble xc,
					gdouble scale, NxpRuler *ruler)
{
  g_assert(NXP_IS_RULER(ruler));
  g_assert(NXP_IS_RULER_ADJUSTMENT(adj));
  g_assert(adj == ruler->adjustment);
  _set_range(ruler,
	     xc - (xc - ruler->lower_save)*scale,
	     xc + (ruler->upper_save - xc)*scale);
}


static void nxp_ruler_end_interaction(NxpRulerAdjustment *adj, gboolean commit, NxpRuler *ruler)
{
  g_assert(NXP_IS_RULER(ruler));
  g_assert(NXP_IS_RULER_ADJUSTMENT(adj));
  g_assert(adj == ruler->adjustment);
  if (!commit)
      _set_range(ruler, adj->lower, adj->upper);
}


void nxp_ruler_set_adjustment(NxpRuler *ruler, NxpRulerAdjustment *adj)
{
  g_return_if_fail(ruler);
  g_return_if_fail(NXP_IS_RULER(ruler));
  if (ruler->adjustment) nxp_ruler_remove_adjustment(ruler);
  ruler->adjustment = adj;
  g_object_ref(adj);
  _set_range(ruler, adj->lower, adj->upper);

  ruler->adj_signals.range_changed =
    g_signal_connect_data(ruler->adjustment, "range-changed",
			  G_CALLBACK(nxp_ruler_range_changed),
			  ruler, NULL, 0);
  ruler->adj_signals.begin_interaction =
    g_signal_connect_data(ruler->adjustment, "begin-interaction",
			  G_CALLBACK(nxp_ruler_begin_interaction),
			  ruler, NULL, 0);
  ruler->adj_signals.interactive_translate =
    g_signal_connect_data(ruler->adjustment, "interactive-translate",
			  G_CALLBACK(nxp_ruler_interactive_translate),
			  ruler, NULL, 0);
  ruler->adj_signals.interactive_scale =
    g_signal_connect_data(ruler->adjustment, "interactive-scale",
			  G_CALLBACK(nxp_ruler_interactive_scale),
			  ruler, NULL, 0);
  ruler->adj_signals.end_interaction =
    g_signal_connect_data(ruler->adjustment, "end-interaction",
			  G_CALLBACK(nxp_ruler_end_interaction),
			  ruler, NULL, 0);

}


void nxp_ruler_set_mode(NxpRuler *ruler, NxpRulerMode mode)
{
  g_return_if_fail(ruler);
  g_return_if_fail(NXP_IS_RULER(ruler));
  ruler->mode = mode;
  ruler->needs_redraw = TRUE;
  gtk_widget_queue_draw(GTK_WIDGET(ruler));
}


NxpRulerAdjustment* nxp_ruler_get_adjustment(NxpRuler *ruler)
{
  return ruler->adjustment;
}

