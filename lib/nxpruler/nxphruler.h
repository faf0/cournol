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
 * Original file: gtkhruler.h, version 1.8
 */


#ifndef __NXP_HRULER_H__
#define __NXP_HRULER_H__


#include <gdk/gdk.h>
#include <nxpruler.h>


G_BEGIN_DECLS

#define NXP_TYPE_HRULER	           (nxp_hruler_get_type ())
#define NXP_HRULER(obj)            (GTK_CHECK_CAST ((obj), NXP_TYPE_HRULER, NxpHRuler))
#define NXP_HRULER_CLASS(klass)    (GTK_CHECK_CLASS_CAST ((klass), NXP_TYPE_HRULER, NxpHRulerClass))
#define NXP_IS_HRULER(obj)         (GTK_CHECK_TYPE ((obj), NXP_TYPE_HRULER))
#define NXP_IS_HRULER_CLASS(klass) (GTK_CHECK_CLASS_TYPE ((klass), NXP_TYPE_HRULER))
#define NXP_HRULER_GET_CLASS(obj)  (GTK_CHECK_GET_CLASS ((obj), NXP_TYPE_HRULER, NxpHRulerClass))


typedef struct _NxpHRuler       NxpHRuler;
typedef struct _NxpHRulerClass  NxpHRulerClass;

typedef enum {
    NXP_HRULER_FACING_UP, NXP_HRULER_FACING_DOWN
} NxpHRulerFacing;


struct _NxpHRuler
{
  NxpRuler ruler;
  NxpHRulerFacing facing;
};

struct _NxpHRulerClass
{
  NxpRulerClass parent_class;
};


GtkType    nxp_hruler_get_type   (void) G_GNUC_CONST;
GtkWidget* nxp_hruler_new        (NxpRulerAdjustment *adjustment);
void       nxp_hruler_set_facing (NxpHRuler          *hruler,
				  NxpHRulerFacing     facing);



G_END_DECLS

#endif /* __NXP_HRULER_H__ */
