/* GtkDatabox - An extension to the gtk+ library
 * Copyright (C) 1998 - 2006  Dr. Roland Bock
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <gtkdatabox_grid.h>

static void gtk_databox_grid_real_draw (GtkDataboxGraph *grid, 
                                  GtkDataboxCanvas *canvas);
static void gtk_databox_grid_real_create_gc (GtkDataboxGraph *graph,
                                       GtkDataboxCanvas *canvas);

/* IDs of properties */
enum
{
   GRID_HLINES = 1,
   GRID_VLINES
};

struct _GtkDataboxGridPrivate
{
   gint hlines;
   gint vlines;
};

static gpointer parent_class = NULL;

static void
gtk_databox_grid_set_property (GObject      *object,
                    guint         property_id,
                    const GValue *value,
                    GParamSpec   *pspec)
{
   GtkDataboxGrid *grid = GTK_DATABOX_GRID (object);

   switch (property_id) 
   {
      case GRID_HLINES: 
         {
            gtk_databox_grid_set_hlines (grid, g_value_get_int (value));
         }
         break;
      case GRID_VLINES: 
         {
            gtk_databox_grid_set_vlines (grid, g_value_get_int (value));
         }
         break;
      default:
        /* We don't have any other property... */
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
   }
}

static void
gtk_databox_grid_get_property (GObject      *object,
                    guint         property_id,
                    GValue       *value,
                    GParamSpec   *pspec)
{
   GtkDataboxGrid *grid = GTK_DATABOX_GRID (object);

   switch (property_id) 
   {
      case GRID_HLINES: 
         {
            g_value_set_int (value, gtk_databox_grid_get_hlines (grid));
         }
         break;
      case GRID_VLINES: 
         {
            g_value_set_int (value, gtk_databox_grid_get_vlines (grid));
         }
         break;
      default:
         /* We don't have any other property... */
         G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
         break;
   }
}

static void
gtk_databox_grid_real_create_gc (GtkDataboxGraph *graph,
                                  GtkDataboxCanvas *canvas)
{
   GdkGCValues values;

   g_return_if_fail (GTK_DATABOX_IS_GRID (graph));

   GTK_DATABOX_GRAPH_CLASS (parent_class)->create_gc (graph, canvas);

   if (graph->gc)
   {
      values.line_style = GDK_LINE_ON_OFF_DASH;
      values.cap_style = GDK_CAP_BUTT;
      values.join_style = GDK_JOIN_MITER;
      gdk_gc_set_values (graph->gc, &values, 
                         GDK_GC_LINE_STYLE | 
                         GDK_GC_CAP_STYLE |
                         GDK_GC_JOIN_STYLE);
   }
}

static void grid_finalize (GObject *object) 
{
   GtkDataboxGrid *grid = GTK_DATABOX_GRID (object);

   g_free (grid->priv);

   /* Chain up to the parent class */
   G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gtk_databox_grid_class_init (gpointer g_class/*, gpointer g_class_data*/)
{
   GObjectClass *gobject_class = G_OBJECT_CLASS (g_class);
   GtkDataboxGraphClass *graph_class = GTK_DATABOX_GRAPH_CLASS (g_class);
   GtkDataboxGridClass *klass = GTK_DATABOX_GRID_CLASS (g_class);
   GParamSpec *grid_param_spec;

   parent_class = g_type_class_peek_parent (klass);

   gobject_class->set_property = gtk_databox_grid_set_property;
   gobject_class->get_property = gtk_databox_grid_get_property;
   gobject_class->finalize     = grid_finalize;

   grid_param_spec = g_param_spec_int ("grid-hlines",
                                       "grid-hlines",
                                       "Number of horizontal lines",
                                       G_MININT,
                                       G_MAXINT,
                                       0, /* default value */
                                       G_PARAM_READWRITE);

   g_object_class_install_property (gobject_class,
                                    GRID_HLINES,
                                    grid_param_spec);
   
   grid_param_spec = g_param_spec_int ("grid-vlines",
                                       "grid-vlines",
                                       "Number of vertical lines",
                                       G_MININT,
                                       G_MAXINT,
                                       0, /* default value */
                                       G_PARAM_READWRITE);

   g_object_class_install_property (gobject_class,
                                    GRID_VLINES,
                                    grid_param_spec);
   
   graph_class->draw = gtk_databox_grid_real_draw;
   graph_class->create_gc = gtk_databox_grid_real_create_gc;
}

static void
gtk_databox_grid_instance_init (GTypeInstance   *instance/*,
                                 gpointer         g_class*/)
{
   GtkDataboxGrid *grid = GTK_DATABOX_GRID (instance);

   grid->priv = g_new0 (GtkDataboxGridPrivate, 1);
}

GType
gtk_databox_grid_get_type (void)
{
   static GType type = 0;
   
   if (type == 0) {
      static const GTypeInfo info = 
      {
         sizeof (GtkDataboxGridClass),
         NULL,                                          /* base_init */
         NULL,                                          /* base_finalize */
         (GClassInitFunc) gtk_databox_grid_class_init,  /* class_init */
         NULL,                                          /* class_finalize */
         NULL,                                          /* class_data */
         sizeof (GtkDataboxGrid),                       /* instance_size */
         0,                                             /* n_preallocs */
         (GInstanceInitFunc) gtk_databox_grid_instance_init, /* instance_init */
         NULL,                                          /* value_table */
      };
      type = g_type_register_static (GTK_DATABOX_TYPE_GRAPH,
                                     "GtkDataboxGrid",
                                     &info, 0);
   }
   
   return type;
}

GtkDataboxGraph *
gtk_databox_grid_new (gint hlines, gint vlines,
                        GdkColor *color, guint size)
{
  GtkDataboxGrid *grid;

  grid = g_object_new (GTK_DATABOX_TYPE_GRID, 
                       "color", color,
                       "size", size,
                       "grid-hlines", hlines,
                       "grid-vlines", vlines,
                       NULL);

  return GTK_DATABOX_GRAPH (grid);
}

static void
gtk_databox_grid_real_draw (GtkDataboxGraph *graph,
                             GtkDataboxCanvas *canvas)
{
   GtkDataboxGrid *grid = GTK_DATABOX_GRID (graph);
   gint i = 0;
   gfloat x;
   gfloat y;

   g_return_if_fail (GTK_DATABOX_IS_GRID (grid));
   g_return_if_fail (canvas);
   g_return_if_fail (canvas->pixmap);

   if (!graph->gc) 
      gtk_databox_graph_create_gc (graph, canvas);

   for (i = 0; i < grid->priv->hlines; i++)
   {
      y = canvas->top_left_total.y 
          + (i + 1) * (canvas->bottom_right_total.y - canvas->top_left_total.y) 
                    / (grid->priv->hlines + 1);
      y = (y - canvas->top_left_visible.y) * canvas->translation_factor.y;
      gdk_draw_line (canvas->pixmap, graph->gc, 
                     0, y, canvas->width, y);
   }

   for (i = 0; i < grid->priv->vlines; i++)
   {
      x = canvas->top_left_total.x 
          + (i + 1) * (canvas->bottom_right_total.x - canvas->top_left_total.x) 
                    / (grid->priv->vlines + 1);
      x = (x - canvas->top_left_visible.x) * canvas->translation_factor.x;
      gdk_draw_line (canvas->pixmap, graph->gc, 
                     x, 0, x, canvas->height);
   }

   return;
}

void
gtk_databox_grid_set_hlines (GtkDataboxGrid *grid, 
                             gint hlines)
{
   g_return_if_fail (GTK_DATABOX_IS_GRID (grid));

   grid->priv->hlines = MAX (1, hlines);

   g_object_notify (G_OBJECT (grid), "grid-hlines");
}

gint
gtk_databox_grid_get_hlines (GtkDataboxGrid *grid)
{
   g_return_val_if_fail (GTK_DATABOX_IS_GRID (grid), -1);

   return grid->priv->hlines;
}

void
gtk_databox_grid_set_vlines (GtkDataboxGrid *grid, 
                             gint vlines)
{
   g_return_if_fail (GTK_DATABOX_IS_GRID (grid));

   grid->priv->vlines = MAX (1, vlines);

   g_object_notify (G_OBJECT (grid), "grid-vlines");
}

gint
gtk_databox_grid_get_vlines (GtkDataboxGrid *grid)
{
   g_return_val_if_fail (GTK_DATABOX_IS_GRID (grid), -1);

   return grid->priv->vlines;
}


