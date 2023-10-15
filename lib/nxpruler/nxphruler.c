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
   Modified by Gustavo Carneiro <gustavo@users.sourceforge.net>
   Original file: gtkhruler.c, version 1.25
*/

#include <math.h>
#include <stdio.h>
#include <string.h>
#include "nxphruler.h"


#define RULER_HEIGHT          14
#define MINIMUM_INCR          5
#define MAXIMUM_SUBDIVIDE     5
#define MAXIMUM_SCALES        10

#define ROUND(x) ((int) ((x) + 0.5))


static void nxp_hruler_class_init     (NxpHRulerClass *klass);
static void nxp_hruler_init           (NxpHRuler      *hruler);
static gint nxp_hruler_motion_notify  (GtkWidget      *widget,
				       GdkEventMotion *event);
static void nxp_hruler_draw_ticks     (NxpRuler       *ruler);
static void nxp_hruler_draw_pos       (NxpRuler       *ruler);
static gint nxp_hruler_button_press   (GtkWidget      *widget,
				       GdkEventButton *event);
static gint nxp_hruler_button_release (GtkWidget      *widget,
				       GdkEventButton *event);




GtkType
nxp_hruler_get_type (void)
{
  static GtkType hruler_type = 0;

  if (!hruler_type)
    {
      static const GtkTypeInfo hruler_info =
      {
	"NxpHRuler",
	sizeof (NxpHRuler),
	sizeof (NxpHRulerClass),
	(GtkClassInitFunc) nxp_hruler_class_init,
	(GtkObjectInitFunc) nxp_hruler_init,
	/* reserved_1 */ NULL,
        /* reserved_2 */ NULL,
        (GtkClassInitFunc) NULL,
      };

      hruler_type = gtk_type_unique (NXP_TYPE_RULER, &hruler_info);
    }

  return hruler_type;
}

static void
nxp_hruler_class_init (NxpHRulerClass *klass)
{
  GtkWidgetClass *widget_class;
  NxpRulerClass *ruler_class;

  widget_class = (GtkWidgetClass*) klass;
  ruler_class = (NxpRulerClass*) klass;

  widget_class->motion_notify_event = nxp_hruler_motion_notify;
  widget_class->button_press_event = nxp_hruler_button_press;
  widget_class->button_release_event = nxp_hruler_button_release;

  ruler_class->draw_ticks = nxp_hruler_draw_ticks;
  ruler_class->draw_pos = nxp_hruler_draw_pos;
}

static void
nxp_hruler_init (NxpHRuler *hruler)
{
  GtkWidget *widget;

  widget = GTK_WIDGET (hruler);
  widget->requisition.width = widget->style->xthickness * 2 + 1;
  widget->requisition.height = widget->style->ythickness * 2 + RULER_HEIGHT;
  hruler->facing = NXP_HRULER_FACING_DOWN;
}


/**
 * nxp_hruler_new:
 * @adjustment: a #NxpRulerAdjustment; the ruler will *not* take ownership of this object.
 * 
 * Create new horizontal ruler widget
 * 
 * Return value: the new widget
 **/
GtkWidget*
nxp_hruler_new (NxpRulerAdjustment *adjustment)
{
  GtkWidget *widget = GTK_WIDGET (gtk_type_new (NXP_TYPE_HRULER));
  nxp_ruler_set_adjustment(NXP_RULER(widget), adjustment);
  return widget;
}



static void
nxp_hruler_draw_ticks(NxpRuler *ruler)
{
  NxpRulerClass *klass;
  GtkWidget *widget;
  GdkGC *gc, *bg_gc;
  gint width, height, xthickness, ythickness;
  gchar unit_str[32];
  gint digit_height, length, digit_offset;
  gdouble rx0, rxl, rx, rxf, rdiv;
  gfloat x0, x, x1, xf, subx, div, subdiv;
  gint wx, t;
  gboolean facing_down;
  NxpHRuler *hruler;
  PangoLayout *layout;
  PangoRectangle logical_rect, ink_rect;

  g_return_if_fail(ruler != NULL);
  hruler = NXP_HRULER(ruler);

  if (!ruler->backing_store) return;

  klass = NXP_RULER_GET_CLASS(ruler);
  nxp_ruler_compute_ticks(ruler);
  widget = GTK_WIDGET(ruler);

  gc = widget->style->fg_gc[widget->state];
  bg_gc = widget->style->bg_gc[widget->state];

  facing_down = (hruler->facing != NXP_HRULER_FACING_UP);

  xthickness = widget->style->xthickness;
  ythickness = widget->style->ythickness;
  layout = gtk_widget_create_pango_layout(widget, "012456789E.+-");
  pango_layout_get_extents(layout, &ink_rect, &logical_rect);
  digit_height = PANGO_PIXELS(ink_rect.height) + 1;
  digit_offset = ink_rect.y;

  width = widget->allocation.width;
  height = widget->allocation.height - ythickness * 2;

  gdk_draw_rectangle(ruler->backing_store,
		     widget->style->bg_gc[widget->state],
		     TRUE, 0, 0, widget->allocation.width,
		     widget->allocation.height);
  gtk_paint_box(widget->style, ruler->backing_store,
		widget->state, GTK_SHADOW_OUT, 
		NULL, widget, "hruler",
		0, 0, 
		widget->allocation.width, widget->allocation.height);

  if (facing_down)
    gdk_draw_line(ruler->backing_store, gc,
		  xthickness,
		  height + ythickness,
		  widget->allocation.width - xthickness,
		  height + ythickness);
  else
    gdk_draw_line(ruler->backing_store, gc,
		  xthickness, 0,
		  widget->allocation.width - xthickness, 0);

  x0 = ruler->tick_start;
  div = ruler->tick_div;
  subdiv = ruler->tick_subdiv;
  xf = ruler->tick_limit;

  rx0 = ruler->rtick_start;
  rdiv = ruler->rtick_div;
  rxf = ruler->tick_limit;

  switch (ruler->mode)
    {
    case NXP_RULER_LINEAR:
      for (x = x0, rx = rx0, rxl = rx0; x < xf; x += div, rx += rdiv) {
	wx = rint(x);
	length = 10;
	if (wx >= 0) {
	  if (facing_down) {
	    gdk_draw_line(ruler->backing_store, gc,
			  wx, height + ythickness, 
			  wx, height - length + ythickness);
	  } else {
	    gdk_draw_line(ruler->backing_store, gc,
			  wx, 1, 
			  wx, length - 1);
	  }
	}
	if (fabs(rx / rxl) <= 1e-10) // is this number very small compared to the previous
	  sprintf(unit_str, "%G", 0.0); // number? If so, it's probably just 0.0
	else
	  sprintf(unit_str, "%G", rx);
	rxl = rx;

	pango_layout_set_text(layout, unit_str, -1);
	pango_layout_get_extents(layout, &logical_rect, NULL);

	if (facing_down) {
	  gtk_paint_layout(widget->style, 
			   ruler->backing_store, 
			   GTK_WIDGET_STATE(widget), 
			   FALSE, 
			   NULL, 
			   widget, 
			   "hruler", 
			   wx + 2, ythickness + PANGO_PIXELS(logical_rect.y - digit_offset), 
			   layout);
	} else {
	  gtk_paint_layout(widget->style, 
			   ruler->backing_store, 
			   GTK_WIDGET_STATE(widget), 
			   FALSE, 
			   NULL, 
			   widget, 
			   "hruler", 
			   wx + 2, ythickness, 
			   layout);
	}

	for (subx = x; subx < x + div; subx += subdiv) {
	  length = 5;
	  wx = rint(subx);
	  if (wx >= 0) {
	    if (facing_down) {
	      gdk_draw_line(ruler->backing_store, gc,
			    wx, height + ythickness, 
			    wx, height - length + ythickness);
	    } else {
	      gdk_draw_line(ruler->backing_store, gc,
			    wx, 1, 
			    wx, length - 1);
	    }
	  }
	  if (subdiv >= 6) {
	    wx += subdiv / 2;
	    length = 3;
	    if (wx >= 0) {
	      if (facing_down) {
		gdk_draw_line(ruler->backing_store, gc,
			      wx, height + ythickness, 
			      wx, height - length + ythickness);
	      } else {
		gdk_draw_line(ruler->backing_store, gc,
			      wx, 1, 
			      wx, length - 1);
	      }
	    }
	  }
	}
      }
      break;
    case NXP_RULER_LOG:
      if (div < 10) break;

      for (x = x0, rx = rx0; x < xf; x += div, rx++) {
	wx = rint(x);
	length = 10;
	if (wx >= 0) {
	  if (facing_down) {
	    gdk_draw_line(ruler->backing_store, gc,
			  wx, height + ythickness, 
			  wx, height - length + ythickness);
	  } else {
	    gdk_draw_line(ruler->backing_store, gc,
			  wx, 1,
			  wx, length - 1);
	  }
	}
	if (rx <= 3 && rx >= -2)
	  sprintf(unit_str, "%G", pow(10, rx));
	else
	  sprintf(unit_str, "1E%i", (int)rx);
	pango_layout_set_text(layout, unit_str, -1);
	pango_layout_get_extents(layout, &logical_rect, NULL);
	if (facing_down)
	  gtk_paint_layout(widget->style, 
			   ruler->backing_store, 
			   GTK_WIDGET_STATE(widget), 
			   FALSE, 
			   NULL, 
			   widget, 
			   "hruler", 
			   wx + 2, ythickness + PANGO_PIXELS(logical_rect.y - digit_offset), 
			   layout);
	else
	  gtk_paint_layout(widget->style, 
			   ruler->backing_store, 
			   GTK_WIDGET_STATE(widget), 
			   FALSE, 
			   NULL, 
			   widget, 
			   "hruler", 
			   wx + 2, ythickness, 
			   layout);

	for (t = 2 ; t <= 9 ; t++) {
	  x1 = x + div*klass->log29[t - 2];
	  if (x1 > xf) break;
	  if (x1 >= 0) {
	    length = 3;
	    wx = rint(x1);
	    if (facing_down)
	      gdk_draw_line(ruler->backing_store, gc,
			    wx, height + ythickness, 
			    wx, height - length + ythickness);
	    else
	      gdk_draw_line(ruler->backing_store, gc,
			    wx, 1, 
			    wx, length - 1);
	  }
	}
      }
      break;
    }
  g_object_unref(layout);
}



static void
nxp_hruler_draw_pos (NxpRuler *ruler)
{
  GtkWidget *widget;
  GdkGC *gc;
  int i;
  gint x, y;
  gint width, height;
  gint bs_width, bs_height;
  gint xthickness;
  gint ythickness;
  gfloat increment;
  NxpHRuler *hruler;
  gboolean facing_down;

  g_return_if_fail (ruler != NULL);
  g_return_if_fail (NXP_IS_HRULER (ruler));

  hruler = NXP_HRULER (ruler);
  facing_down = (hruler->facing != NXP_HRULER_FACING_UP);
    
  if (GTK_WIDGET_DRAWABLE (ruler))
    {
      widget = GTK_WIDGET (ruler);

      gc = widget->style->fg_gc[widget->state];
      xthickness = widget->style->xthickness;
      ythickness = widget->style->ythickness;
      width = widget->allocation.width;
      height = widget->allocation.height - ythickness * 2;

      bs_width = height / 2;
      bs_width |= 1;  /* make sure it's odd */
      bs_height = bs_width / 2 + 1;

      if ((bs_width > 0) && (bs_height > 0))
	{
	  /*  If a backing store exists, restore the ruler  */
	  if (ruler->backing_store && ruler->non_gr_exp_gc)
	    gdk_draw_pixmap (ruler->widget.window,
			     ruler->non_gr_exp_gc,
			     ruler->backing_store,
			     ruler->xsrc, ruler->ysrc,
			     ruler->xsrc, ruler->ysrc,
			     bs_width, bs_height);

	  increment = (gfloat) width / (ruler->upper - ruler->lower);

	  x = ROUND ((ruler->position - ruler->lower) * increment) + (xthickness - bs_width) / 2 - 1;
	  if (facing_down)
	    y = (height + bs_height) / 2 + ythickness;
	  else 
	    y = bs_height - 1 + ythickness;

	  for (i = 0; i < bs_height; i++)
	    gdk_draw_line (widget->window, gc,
			   x + i, y + (facing_down?i:-i),
			   x + bs_width - 1 - i, y + (facing_down?i:-i));
	  ruler->xsrc = x;
	  if (facing_down)
	    ruler->ysrc = y;
	  else 
	    ruler->ysrc = ythickness;
	}
    }
}


void nxp_hruler_set_facing(NxpHRuler *hruler, NxpHRulerFacing facing)
{
  g_return_if_fail(NXP_IS_HRULER(hruler));
  hruler->facing = facing;
}


/* Horizontal ruler drag event handlers */
static gint nxp_hruler_button_press(GtkWidget *widget, GdkEventButton *event)
{
    NxpRuler *ruler;
    gint width;

    g_return_val_if_fail(NXP_IS_HRULER(widget), 0);
    ruler = NXP_RULER(widget);
/*     if (!ruler->interactive) return FALSE; */
    width = widget->allocation.width;
    if (ruler->dragging)
	return FALSE;
    ruler->dragging = TRUE;
    ruler->drag_start = event->x;
    ruler->drag_button = event->button;
    gdk_pointer_grab(widget->window, FALSE,
		     GDK_POINTER_MOTION_MASK|GDK_BUTTON_RELEASE_MASK,
		     NULL, NULL, event->time);
    nxp_ruler_adjustment_begin_interaction(ruler->adjustment);
    return TRUE;
}


static gint nxp_hruler_button_release(GtkWidget *widget, GdkEventButton *event)
{
    NxpRuler *ruler;
    g_return_val_if_fail(NXP_IS_HRULER(widget), 0);
    ruler = NXP_RULER(widget);
    if (!ruler->dragging) return FALSE;
    ruler->dragging = FALSE;
    gdk_pointer_ungrab(event->time);
    nxp_ruler_adjustment_end_interaction(ruler->adjustment, TRUE);
    nxp_ruler_adjustment_set_range(ruler->adjustment, ruler->lower, ruler->upper);
    return FALSE;
}


/*   */
static void nxp_hruler_drag_motion(GtkWidget *widget, gint event_x)
{
    gdouble dx, view_w, xinc;
    NxpRuler *ruler;
    gint width;
    g_return_if_fail(NXP_IS_HRULER(widget));
    ruler = NXP_RULER(widget);
    width = widget->allocation.width;

    if (!ruler->dragging)
	return;
    dx = event_x - ruler->drag_start;
    view_w = ruler->upper_save - ruler->lower_save;
    switch (ruler->drag_button)
    {
    case 1: /* Button 1 -> Shift mode */
	xinc = dx / width*view_w;
	nxp_ruler_adjustment_interactive_translate(ruler->adjustment, -xinc);
	break;
    case 3: /* Button 3 -> Scale mode */
    {
	gdouble xc, scale;
	xc = ruler->lower_save + ruler->drag_start / width*view_w;
	scale = exp( -(event_x - ruler->drag_start) / 100 );
	nxp_ruler_adjustment_interactive_scale(ruler->adjustment, xc, scale);
    }
    break;
    }
}


static gint
nxp_hruler_motion_notify(GtkWidget      *widget, 
			    GdkEventMotion *event)
{
    NxpRuler *ruler;
    gint x;

    g_return_val_if_fail(widget != NULL, FALSE);
    g_return_val_if_fail(NXP_IS_HRULER(widget), FALSE);
    g_return_val_if_fail(event != NULL, FALSE);

    ruler = NXP_RULER(widget);

    if (event->is_hint)
	gdk_window_get_pointer(widget->window, &x, NULL, NULL);
    else
	x = event->x;

    ruler->position = ruler->lower + ((ruler->upper - ruler->lower) * x) / 
	widget->allocation.width;


    if (ruler->backing_store != NULL && !ruler->dragging)
	nxp_ruler_draw_pos(ruler);
    nxp_hruler_drag_motion(widget, x);

    return FALSE;
}




