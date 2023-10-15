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
 * NOTE this widget is considered too specialized/little-used for
 * GTK+, and will in the future be moved to some other package.  If
 * your application needs this widget, feel free to use it, as the
 * widget does work and is useful in some applications; it's just not
 * of general interest. However, we are not accepting new features for
 * the widget, and it will eventually move out of the GTK+
 * distribution.
 */

/* 
 * Modified by Gustavo Carneiro <gustavo@users.sourceforge.net>
 * Original file: gtkruler.h, version 1.13
 */

#ifndef __NXP_RULER_H__
#define __NXP_RULER_H__


#include <gdk/gdk.h>
#include <gtk/gtkwidget.h>
#include <nxpruleradjustment.h>


G_BEGIN_DECLS

#define NXP_TYPE_RULER            (nxp_ruler_get_type ())
#define NXP_RULER(obj)            (GTK_CHECK_CAST ((obj), NXP_TYPE_RULER, NxpRuler))
#define NXP_RULER_CLASS(klass)    (GTK_CHECK_CLASS_CAST ((klass), NXP_TYPE_RULER, NxpRulerClass))
#define NXP_IS_RULER(obj)         (GTK_CHECK_TYPE ((obj), NXP_TYPE_RULER))
#define NXP_IS_RULER_CLASS(klass) (GTK_CHECK_CLASS_TYPE ((klass), NXP_TYPE_RULER))
#define NXP_RULER_GET_CLASS(obj)  (GTK_CHECK_GET_CLASS ((obj), NXP_TYPE_RULER, NxpRulerClass))


typedef struct _NxpRuler        NxpRuler;
typedef struct _NxpRulerClass   NxpRulerClass;


typedef enum {NXP_RULER_LINEAR, NXP_RULER_LOG} NxpRulerMode; 


struct _NxpRuler
{
  GtkWidget widget;

  GdkPixmap *backing_store;
  GdkGC *non_gr_exp_gc;
  gint xsrc, ysrc;
  gint slider_size;

  NxpRulerAdjustment *adjustment;
  struct {
    gulong range_changed;
    gulong begin_interaction;
    gulong interactive_translate;
    gulong interactive_scale;
    gulong end_interaction;
  } adj_signals;
  /* The upper limit of the ruler (in points) */
  gdouble lower;
  /* The lower limit of the ruler */
  gdouble upper;
  /* The position of the mark on the ruler */
  gdouble position;

  /* Tick marks information -- this information could be useful to
     the application */
  /* (tick marks in pixel units) */
  gfloat tick_start,tick_div,tick_subdiv,tick_limit;
  /* (tick marks in ruler units) */
  gdouble rtick_start,rtick_div,rtick_subdiv,rtick_limit;

  gdouble lower_save, upper_save, drag_start;
  gint drag_button;
  gboolean dragging:1;
  gboolean needs_redraw:1;  
  NxpRulerMode mode;
  
};


struct _NxpRulerClass
{
  GtkWidgetClass parent_class;

  void (* draw_ticks) (NxpRuler *ruler);
  void (* draw_pos)   (NxpRuler *ruler);

  gdouble log29[8];		/* log10(2..9) */
};



GtkType             nxp_ruler_get_type       (void) G_GNUC_CONST;
void                nxp_ruler_set_adjustment (NxpRuler           *ruler,
					      NxpRulerAdjustment *adj);
NxpRulerAdjustment* nxp_ruler_get_adjustment (NxpRuler           *ruler);
void                nxp_ruler_set_mode       (NxpRuler           *ruler,
					      NxpRulerMode        mode);
void                nxp_ruler_compute_ticks  (NxpRuler           *ruler);


/*< protected >*/
void nxp_ruler_draw_ticks    (NxpRuler *ruler);
void nxp_ruler_draw_pos      (NxpRuler *ruler);



G_END_DECLS

#endif /* __NXP_RULER_H__ */
