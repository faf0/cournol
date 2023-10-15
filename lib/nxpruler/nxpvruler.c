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
 * Modified by Gustavo Carneiro <gustavo@users.sourceforge.net>
 * Original file: gtkvruler.c, version 1.24
 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include "nxpvruler.h"


#define RULER_WIDTH           14
#define MINIMUM_INCR          5
#define MAXIMUM_SUBDIVIDE     5
#define MAXIMUM_SCALES        10

#define ROUND(x) ((int) ((x) + 0.5))


static void nxp_vruler_class_init     (NxpVRulerClass *klass);
static void nxp_vruler_init           (NxpVRuler      *vruler);
static gint nxp_vruler_motion_notify  (GtkWidget      *widget,
				       GdkEventMotion *event);
static void nxp_vruler_draw_ticks     (NxpRuler       *ruler);
static void nxp_vruler_draw_pos       (NxpRuler       *ruler);
static gint nxp_vruler_button_press   (GtkWidget      *widget,
				       GdkEventButton *event);
static gint nxp_vruler_button_release (GtkWidget      *widget,
				       GdkEventButton *event);
static void nxp_vruler_drag_motion    (GtkWidget      *widget,
				       gint            event_y);


GtkType
nxp_vruler_get_type (void)
{
  static GtkType vruler_type = 0;

  if (!vruler_type)
    {
      static const GtkTypeInfo vruler_info =
      {
	"NxpVRuler",
	sizeof (NxpVRuler),
	sizeof (NxpVRulerClass),
	(GtkClassInitFunc) nxp_vruler_class_init,
	(GtkObjectInitFunc) nxp_vruler_init,
	/* reserved_1 */ NULL,
        /* reserved_2 */ NULL,
        (GtkClassInitFunc) NULL,
      };

      vruler_type = gtk_type_unique (NXP_TYPE_RULER, &vruler_info);
    }

  return vruler_type;
}

static void
nxp_vruler_class_init (NxpVRulerClass *klass)
{
  GtkWidgetClass *widget_class;
  NxpRulerClass *ruler_class;

  widget_class = (GtkWidgetClass*) klass;
  ruler_class = (NxpRulerClass*) klass;

  widget_class->motion_notify_event  = nxp_vruler_motion_notify;
  widget_class->button_press_event   = nxp_vruler_button_press;
  widget_class->button_release_event = nxp_vruler_button_release;

  ruler_class->draw_ticks = nxp_vruler_draw_ticks;
  ruler_class->draw_pos = nxp_vruler_draw_pos;
}

static void
nxp_vruler_init (NxpVRuler *vruler)
{
  GtkWidget *widget;

  widget = GTK_WIDGET (vruler);
  widget->requisition.width = widget->style->xthickness * 2 + RULER_WIDTH;
  widget->requisition.height = widget->style->ythickness * 2 + 1;
}

/**
 * nxp_vruler_new:
 * @adjustment: a #NxpRulerAdjustment; the ruler will *not* take ownership of this object.
 * 
 * Create new vertical ruler widget
 * 
 * Return value: the new widget
 **/
GtkWidget*
nxp_vruler_new (NxpRulerAdjustment *adjustment)
{
  GtkWidget *widget = GTK_WIDGET (gtk_type_new (NXP_TYPE_VRULER));
  nxp_ruler_set_adjustment(NXP_RULER(widget), adjustment);
  return widget;
}


static void
nxp_vruler_draw_ticks(NxpRuler *ruler)
{
  GtkWidget *widget;
  GdkGC *gc, *bg_gc;
  gint j;
  gint width, height;
  gint xthickness;
  gint ythickness;
  gint length;
  gchar unit_str[32];
  gint digit_height, digit_offset;
  NxpRulerClass *klass;
  gdouble ry0, ry, ryl, ryf, rdiv;
  gfloat y0, y, y1, yf, suby, div, subdiv;
  gint wy, t;
  PangoLayout *layout;
  PangoRectangle logical_rect, ink_rect;


  g_return_if_fail(ruler != NULL);
  g_return_if_fail(NXP_IS_VRULER(ruler));

  /* if (!GTK_WIDGET_DRAWABLE (ruler)) 
     return;*/

  widget = GTK_WIDGET(ruler);
  klass = NXP_RULER_GET_CLASS(ruler);

  gc = widget->style->fg_gc[widget->state];
  bg_gc = widget->style->bg_gc[widget->state];
  xthickness = widget->style->xthickness;
  ythickness = widget->style->ythickness;
  layout = gtk_widget_create_pango_layout(widget, "012456789E.+-");
  pango_layout_get_extents(layout, &ink_rect, &logical_rect);
  digit_height = PANGO_PIXELS(ink_rect.height) + 1;
  width = widget->allocation.height;
  height = widget->allocation.width - ythickness * 2;
  digit_offset = ink_rect.y;

  gdk_draw_rectangle(ruler->backing_store,
		     widget->style->bg_gc[widget->state],
		     TRUE, 0, 0, widget->allocation.width,
		     widget->allocation.height);
  gtk_paint_box(widget->style, ruler->backing_store, 
		widget->state, GTK_SHADOW_OUT,
		NULL, widget, "vruler", 
		0, 0,
		widget->allocation.width, widget->allocation.height);

  gdk_draw_line(ruler->backing_store, gc, 
		height + xthickness, 
		ythickness, 
		height + xthickness, 
		widget->allocation.height - ythickness);

  nxp_ruler_compute_ticks(ruler);

  y0 = ruler->tick_start;
  div = ruler->tick_div;
  subdiv = ruler->tick_subdiv;
  yf = ruler->tick_limit + div;

  ry0 = ruler->rtick_start;
  rdiv = ruler->rtick_div;
  ryf = ruler->rtick_limit;

  /* drawing starts here */
  switch (ruler->mode)
    {
    case NXP_RULER_LINEAR:
      for (y = y0, ry = ry0, ryl = ry0; y < yf; y += div, ry += rdiv) {
	wy = width - rint(y);
	length = 10;
	gdk_draw_line(ruler->backing_store, gc, 
		      height + xthickness - length, wy, 
		      height + xthickness, wy);
	/* draw label */
	if (fabs(ry / ryl) <= 1e-10) // is this number very small compared to the previous
	  sprintf(unit_str, "%G", 0.0); // number? If so, it's probably just 0.0
	else
	  sprintf(unit_str, "%G", ry);
	ryl = ry;

	for (j = 0; j < (int) strlen (unit_str); j++)
	  {
	    pango_layout_set_text (layout, unit_str + j, 1);
	    pango_layout_get_extents (layout, NULL, &logical_rect);
	    gtk_paint_layout (widget->style, ruler->backing_store,
			      GTK_WIDGET_STATE(widget),
			      FALSE, NULL, widget,
			      "vruler",
			      xthickness + 1,
			      wy + digit_height * j + 2 + PANGO_PIXELS (logical_rect.y - digit_offset),
			      layout);
	  }

	for (suby = y; suby < y + div; suby += subdiv)
	  {
	    length = 5;
	    wy = width - rint(suby);
	    gdk_draw_line(ruler->backing_store, gc, 
			  height + xthickness - length, wy, 
			  height + xthickness, wy);
	    if (subdiv >= 6)
	      {
		wy -= subdiv / 2;
		length = 3;
		gdk_draw_line(ruler->backing_store, gc, 
			      height + xthickness - length, wy, 
			      height + xthickness, wy);
	      }
	  }
      }
      break;
    case NXP_RULER_LOG:
      if (div < 10) break;

      for (y = y0, ry = ry0; y < yf; y += div, ry++) {
	wy = width - rint(y);
	length = 10;
	gdk_draw_line(ruler->backing_store, gc, 
		      height + xthickness - length, wy, 
		      height + xthickness, wy);
	/* draw label */
	if (ry <= 3 && ry >= -2)
	  sprintf(unit_str, "%G", pow(10, ry));
	else
	  sprintf(unit_str, "1E%i", (int)ry);
	//sprintf (unit_str, "%G", pow(10,ry));
	for (j = 0; j < (int) strlen (unit_str); j++)
	  {
	    pango_layout_set_text (layout, unit_str + j, 1);
	    pango_layout_get_extents (layout, NULL, &logical_rect);
	    gtk_paint_layout (widget->style, ruler->backing_store,
			      GTK_WIDGET_STATE(widget),
			      FALSE, NULL, widget,
			      "vruler",
			      xthickness + 1,
			      wy + digit_height * j + 2 + PANGO_PIXELS (logical_rect.y - digit_offset),
			      layout);
	  }

	for (t = 2 ; t <= 9 ; t++) {
	  y1 = y + div*klass->log29[t - 2];
	  if (y1 > yf) break;
	  if (y1 >= 0) {
	    length = 3;
	    wy = width - rint(y1);
	    gdk_draw_line(ruler->backing_store, gc, 
			  height + xthickness - length, wy, 
			  height + xthickness, wy);
	  }
	}
      }
      break;
    }
  g_object_unref(layout);
}





static void
nxp_vruler_draw_pos (NxpRuler *ruler)
{
  GtkWidget *widget;
  GdkGC *gc;
  int i;
  gint x, y;
  gint width, height;
  gint bs_width, bs_height;
  gint xthickness;
  gint ythickness;
  gdouble increment;

  if (GTK_WIDGET_DRAWABLE (ruler))
    {
      widget = GTK_WIDGET (ruler);

      gc = widget->style->fg_gc[GTK_STATE_NORMAL];
      xthickness = widget->style->xthickness;
      ythickness = widget->style->ythickness;
      width = widget->allocation.width - xthickness * 2;
      height = widget->allocation.height;

      bs_height = width / 2;
      bs_height |= 1;  /* make sure it's odd */
      bs_width = bs_height / 2 + 1;

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

	  increment = (gdouble) height / (ruler->upper - ruler->lower);

	  x = (width + bs_width) / 2 + xthickness;
	  y = ROUND ((ruler->position - ruler->lower) * increment) + (ythickness - bs_height) / 2 - 1;

	  for (i = 0; i < bs_width; i++)
	    gdk_draw_line (widget->window, gc,
			   x + i, y + i,
			   x + i, y + bs_height - 1 - i);

	  ruler->xsrc = x;
	  ruler->ysrc = y;
	}
    }
}



/* Horizontal ruler drag event handlers */
static gint nxp_vruler_button_press(GtkWidget *widget, GdkEventButton *event)
{
    NxpRuler *ruler;
/*     gint width; */

    g_return_val_if_fail(NXP_IS_VRULER(widget), 0);
    ruler = NXP_RULER(widget);
/*     if (!ruler->interactive) return FALSE; */
/*     width = widget->allocation.width; */
    if (ruler->dragging)
	return FALSE;
    ruler->dragging = TRUE;
    ruler->drag_start = event->y;
    ruler->drag_button = event->button;
    gdk_pointer_grab(widget->window, FALSE,
		     GDK_POINTER_MOTION_MASK|GDK_BUTTON_RELEASE_MASK,
		     NULL, NULL, event->time);
    nxp_ruler_adjustment_begin_interaction(ruler->adjustment);
    return TRUE;
}


static gint nxp_vruler_button_release(GtkWidget *widget, GdkEventButton *event)
{
    NxpRuler *ruler;
    g_return_val_if_fail(NXP_IS_VRULER(widget), 0);
    ruler = NXP_RULER(widget);
    if (!ruler->dragging) return FALSE;
    ruler->dragging = FALSE;
    gdk_pointer_ungrab(event->time);
    nxp_ruler_adjustment_end_interaction(ruler->adjustment, TRUE);
    nxp_ruler_adjustment_set_range(ruler->adjustment, ruler->lower, ruler->upper);
    return FALSE;
}


static void nxp_vruler_drag_motion(GtkWidget *widget, gint event_y)
{
    gdouble dy, view_h, yinc;
    NxpRuler *ruler;
    gint height;

    g_return_if_fail(NXP_IS_VRULER(widget));
    ruler = NXP_RULER(widget);
    height = widget->allocation.height;

    if (!ruler->dragging)
	return;
    dy = event_y - ruler->drag_start;
    view_h = ruler->upper_save - ruler->lower_save;
    switch (ruler->drag_button)
    {
    case 1: /* Button 1 -> Shift mode */
	yinc = dy / height*view_h;
	nxp_ruler_adjustment_interactive_translate(ruler->adjustment, yinc);
	break;
    case 3: /* Button 3 -> Scale mode */
    {
	gdouble yc, scale;
	yc = ruler->lower_save + (height - ruler->drag_start)/((double)height)*view_h;
	scale = exp(dy/100);
	nxp_ruler_adjustment_interactive_scale(ruler->adjustment, yc, scale);
    }
    break;
    }
}


static gint
nxp_vruler_motion_notify(GtkWidget *widget, GdkEventMotion *event)
{
    NxpRuler *ruler;
    gint y;

    g_return_val_if_fail(widget != NULL, FALSE);
    g_return_val_if_fail(NXP_IS_VRULER(widget), FALSE);
    g_return_val_if_fail(event != NULL, FALSE);

    ruler = NXP_RULER(widget);

    if (event->is_hint)
	gdk_window_get_pointer(widget->window, NULL, &y, NULL);
    else
	y = event->y;

    ruler->position = ruler->lower + ((ruler->upper - ruler->lower) * y) / 
	widget->allocation.height;


    if (ruler->backing_store != NULL && !ruler->dragging)
	nxp_ruler_draw_pos(ruler);
    nxp_vruler_drag_motion(widget, y);

    return FALSE;
}

