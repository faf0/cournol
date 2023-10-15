/* cournol
 * Copyright (C) (2007) (Fabian Förg) <fabian.foerg@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <nxpgtkdatabox.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_grid.h>
#include <gtkdatabox_marker.h>

#include "output.h"
#include "main.h"
#include "message.h"
#include "options.h"
#include "input.h"
#include "init_points.h"
#include "create_table.h"

#ifdef HAVE_CONFIG_H
# include "../config.h"
#else
#ifdef WIN32
# include "../config.h.w32"
#endif
#endif

/* thickness of the graphs */
#define GAUGE 1

/* size of the (triangle) marker */
#define MARKER_SIZE 8

enum {
	SHOW_ACTUAL_X,
	SHOW_ACTUAL_Y,
	SHOW_NUM_ENTRIES
};

static GtkWidget *show_entry(GtkWidget * hbox, const gchar * text);
static gint show_motion_notify_cb(GtkWidget * darea, GdkEventMotion * event,
				  GtkWidget ** entries);
static void zoom_in_cb(void);
static void zoom_out_cb(void);
static void about_cb(void);
static void back_cb(appl_t * appl);
static void print_dialog(appl_t * appl);
static void draw_page(GtkPrintOperation * operation, GtkPrintContext * context,
		      int page_nr, appl_t * appl);
static GtkWidget *create_print_win(appl_t * appl);
static gboolean delete_event_cb(void);

static GtkWidget *create_main_vbox(appl_t * appl, gboolean scrollbar)
{
	GtkWidget *vbox, *table, *label, *hbox, *gtable_1, *gtable_2;
	GdkColor color;
	GtkDataboxGraph *graph;
	values_t *val = &(appl->values);

	/* create main vbox for the table and hboxes */
	vbox = gtk_vbox_new(FALSE, 0);

	/* create and add the info table */
	switch (val->type) {
	case 1:
		table = create_table_poly_1(appl);
		break;
	case 2:
		table = create_table_poly_2(appl);
		break;
	case 3:
		table = create_table_poly_3(appl);
		break;
	case 4:
		table = create_table_mono_1(appl);
		break;
	case 5:
		table = create_table_mono_2(appl);
		break;
	default:
		table = create_table_mono_3(appl);
		break;
	}
	gtk_box_pack_start(GTK_BOX(vbox), table, FALSE, TRUE, 0);

	/* create hbox for observation texts */
	hbox = gtk_hbox_new(TRUE, 20);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 20);

	label = gtk_label_new(_("<b>Absolute observation</b>"));
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 0);

	label = gtk_label_new(_("<b>Marginal observation</b>"));
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_box_pack_end(GTK_BOX(hbox), label, TRUE, TRUE, 0);

	/* create hbox for graphs and add it to vbox */
	hbox = gtk_hbox_new(TRUE, 20);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);

	gtk_databox_create_box_with_scrollbars_and_rulers(&(appl->darea_1),
							  &gtable_1, scrollbar,
							  scrollbar, TRUE,
							  TRUE);
	gtk_box_pack_start(GTK_BOX(hbox), gtable_1, TRUE, TRUE, 0);

	gtk_databox_create_box_with_scrollbars_and_rulers(&(appl->darea_2),
							  &gtable_2, scrollbar,
							  scrollbar, TRUE,
							  TRUE);
	gtk_box_pack_end(GTK_BOX(hbox), gtable_2, TRUE, TRUE, 0);

	/* drawing area background */
	color.red = 65535;
	color.green = 65535;
	color.blue = 57015;

	gtk_widget_modify_bg(appl->darea_1, GTK_STATE_NORMAL, &color);
	gtk_widget_modify_bg(appl->darea_2, GTK_STATE_NORMAL, &color);

	/* C(x) */
	color.red = 5397;
	color.green = 42148;
	color.blue = 2570;

	graph =
	    gtk_databox_lines_new(val->points_cost, val->x_cost, val->y_cost,
				  &color, GAUGE);
	gtk_databox_graph_add(GTK_DATABOX(appl->darea_1), graph);

	graph =
	    gtk_databox_marker_new(val->points_cost, val->x_cost, val->y_cost,
				   &color, MARKER_SIZE,
				   GTK_DATABOX_MARKER_NONE);
	gtk_databox_marker_set_label(GTK_DATABOX_MARKER(graph),
				     val->points_cost - 1, GTK_DATABOX_TEXT_SW,
				     _("C(x)"), FALSE);
	gtk_databox_graph_add(GTK_DATABOX(appl->darea_1), graph);

	/* C'(x) */
	graph =
	    gtk_databox_lines_new(val->points_cost_, val->x_cost_, val->y_cost_,
				  &color, GAUGE);
	gtk_databox_graph_add(GTK_DATABOX(appl->darea_2), graph);

	graph =
	    gtk_databox_marker_new(val->points_cost_, val->x_cost_,
				   val->y_cost_, &color, MARKER_SIZE,
				   GTK_DATABOX_MARKER_NONE);
	gtk_databox_marker_set_label(GTK_DATABOX_MARKER(graph),
				     val->points_cost_ - 1, GTK_DATABOX_TEXT_SW,
				     _("C\'(x)"), FALSE);
	gtk_databox_graph_add(GTK_DATABOX(appl->darea_2), graph);

	if (val->type <= 3) {
		/* c(x) */
		color.red = 0;
		color.green = 0;
		color.blue = 0;

		graph =
		    gtk_databox_lines_new(val->points_costu, val->x_costu,
					  val->y_costu, &color, GAUGE);
		gtk_databox_graph_add(GTK_DATABOX(appl->darea_2), graph);

		graph =
		    gtk_databox_marker_new(val->points_costu, val->x_costu,
					   val->y_costu, &color, MARKER_SIZE,
					   GTK_DATABOX_MARKER_NONE);
		gtk_databox_marker_set_label(GTK_DATABOX_MARKER(graph),
					     0,
					     GTK_DATABOX_TEXT_SE, _("c(x)"),
					     FALSE);
		gtk_databox_graph_add(GTK_DATABOX(appl->darea_2), graph);

		/* cv(x) */
		color.red = 59367;
		color.green = 4626;
		color.blue = 13367;

		graph =
		    gtk_databox_lines_new(val->points_costuv, val->x_costuv,
					  val->y_costuv, &color, GAUGE);
		gtk_databox_graph_add(GTK_DATABOX(appl->darea_2), graph);

		graph =
		    gtk_databox_marker_new(val->points_costuv, val->x_costuv,
					   val->y_costuv, &color, MARKER_SIZE,
					   GTK_DATABOX_MARKER_NONE);
		gtk_databox_marker_set_label(GTK_DATABOX_MARKER(graph),
					     0,
					     GTK_DATABOX_TEXT_NE, _("cv(x)"),
					     FALSE);
		gtk_databox_graph_add(GTK_DATABOX(appl->darea_2), graph);
	} else {
		/* p(x) */
		color.red = 59367;
		color.green = 4626;
		color.blue = 13367;

		graph =
		    gtk_databox_lines_new(2, val->x_price_sales,
					  val->y_price_sales, &color, GAUGE);
		gtk_databox_graph_add(GTK_DATABOX(appl->darea_1), graph);
		gtk_databox_graph_add(GTK_DATABOX(appl->darea_2), graph);

		graph =
		    gtk_databox_marker_new(2, val->x_price_sales,
					   val->y_price_sales, &color,
					   MARKER_SIZE,
					   GTK_DATABOX_MARKER_NONE);
		gtk_databox_marker_set_label(GTK_DATABOX_MARKER(graph), 1,
					     GTK_DATABOX_TEXT_NW, _("p(x)"),
					     FALSE);
		gtk_databox_graph_add(GTK_DATABOX(appl->darea_1), graph);
		gtk_databox_graph_add(GTK_DATABOX(appl->darea_2), graph);
	}

	/* R(x) */
	color.red = 8995;
	color.green = 1028;
	color.blue = 61166;

	graph =
	    gtk_databox_lines_new(val->points_revenue, val->x_revenue,
				  val->y_revenue, &color, GAUGE);
	gtk_databox_graph_add(GTK_DATABOX(appl->darea_1), graph);

	graph =
	    gtk_databox_marker_new(val->points_revenue, val->x_revenue,
				   val->y_revenue, &color, MARKER_SIZE,
				   GTK_DATABOX_MARKER_NONE);
	if (val->type <= 3)
		gtk_databox_marker_set_label(GTK_DATABOX_MARKER(graph), 1,
					     GTK_DATABOX_TEXT_SW, _("R(x)"),
					     FALSE);
	else
		gtk_databox_marker_set_label(GTK_DATABOX_MARKER(graph),
					     (POINTS - 1) / 2,
					     GTK_DATABOX_TEXT_N, _("R(x)"),
					     FALSE);
	gtk_databox_graph_add(GTK_DATABOX(appl->darea_1), graph);

	/* R'(x) or market price, respectively */
	graph =
	    gtk_databox_lines_new(2, val->x_revenue_, val->y_revenue_, &color,
				  GAUGE);
	gtk_databox_graph_add(GTK_DATABOX(appl->darea_2), graph);

	graph =
	    gtk_databox_marker_new(2, val->x_revenue_, val->y_revenue_, &color,
				   MARKER_SIZE, GTK_DATABOX_MARKER_NONE);
	if (val->type <= 3)
		gtk_databox_marker_set_label(GTK_DATABOX_MARKER(graph), 1,
					     GTK_DATABOX_TEXT_SW, _("R\'(x)"),
					     FALSE);
	else
		gtk_databox_marker_set_label(GTK_DATABOX_MARKER(graph), 1,
					     GTK_DATABOX_TEXT_NE, _("R\'(x)"),
					     FALSE);
	gtk_databox_graph_add(GTK_DATABOX(appl->darea_2), graph);

	/* Cournot point */
	if (val->cournot_points != 0) {
		color.red = 0;
		color.green = 0;
		color.blue = 0;

		graph =
		    gtk_databox_marker_new(1, &(val->quantity), &(val->price),
					   &color, MARKER_SIZE,
					   GTK_DATABOX_MARKER_TRIANGLE);
		gtk_databox_marker_set_label(GTK_DATABOX_MARKER(graph), 0,
					     GTK_DATABOX_TEXT_N,
					     _("Cournot point"), FALSE);
		gtk_databox_graph_add(GTK_DATABOX(appl->darea_1), graph);
		gtk_databox_graph_add(GTK_DATABOX(appl->darea_2), graph);
	}

	/* automatically rescale the drawing areas */
	gtk_databox_auto_rescale(GTK_DATABOX(appl->darea_1), 0.);
	gtk_databox_auto_rescale(GTK_DATABOX(appl->darea_2), 0.);

	return vbox;
}

void output(appl_t * appl)
{
	GtkWidget *window, *vbox, *hbox, **entries, *button;
	gboolean scrollbar = TRUE;

	/* create window */
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	gtk_window_set_title(GTK_WINDOW(window), _("Output - " PACKAGE));
	gtk_window_maximize(GTK_WINDOW(window));
	g_signal_connect(G_OBJECT(window), "delete_event",
			 G_CALLBACK(gtk_main_quit), NULL);

	vbox = create_main_vbox(appl, scrollbar);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	/* create lowest hbox and add it to vbox */
	hbox = gtk_hbox_new(FALSE, 10);
	gtk_box_pack_end(GTK_BOX(vbox), hbox, FALSE, TRUE, 15);

	/* display current horizontal and vertical value */
	entries = g_new(GtkWidget *, SHOW_NUM_ENTRIES);
	entries[SHOW_ACTUAL_X] = show_entry(hbox, _("Current x"));
	entries[SHOW_ACTUAL_Y] = show_entry(hbox, _("Current y"));
	g_signal_connect(G_OBJECT(appl->darea_1), "motion_notify_event",
			 G_CALLBACK(show_motion_notify_cb), entries);
	g_signal_connect(G_OBJECT(appl->darea_2), "motion_notify_event",
			 G_CALLBACK(show_motion_notify_cb), entries);

	/* zoom in button */
	button = gtk_button_new_from_stock(GTK_STOCK_ZOOM_IN);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(zoom_in_cb), NULL);
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, TRUE, 0);

	/* zoom out button */
	button = gtk_button_new_from_stock(GTK_STOCK_ZOOM_OUT);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(zoom_out_cb), NULL);
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, TRUE, 0);

	/* about button */
	button = gtk_button_new_from_stock(GTK_STOCK_ABOUT);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(about_cb), NULL);
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, TRUE, 0);

	/* back button: call back_cb and destroy window */
	button = gtk_button_new_from_stock(GTK_STOCK_GO_BACK);
	g_signal_connect_swapped(G_OBJECT(button), "clicked",
				 G_CALLBACK(back_cb), appl);
	g_signal_connect_swapped(G_OBJECT(button), "clicked",
				 G_CALLBACK(gtk_widget_destroy), window);
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, TRUE, 0);

	/* new button: call options and destroy window */
	button = gtk_button_new_from_stock(GTK_STOCK_NEW);
	g_signal_connect_swapped(G_OBJECT(button), "clicked",
				 G_CALLBACK(options), appl);
	g_signal_connect_swapped(G_OBJECT(button), "clicked",
				 G_CALLBACK(gtk_widget_destroy), window);
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, TRUE, 0);

	/* print button */
	button = gtk_button_new_from_stock(GTK_STOCK_PRINT);
	g_signal_connect_swapped(G_OBJECT(button), "clicked",
				 G_CALLBACK(print_dialog), appl);
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, TRUE, 0);

	/* quit button */
	button = gtk_button_new_from_stock(GTK_STOCK_QUIT);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(gtk_main_quit), NULL);
	gtk_box_pack_end(GTK_BOX(hbox), button, FALSE, TRUE, 0);

	/* display window with all widgets */
	gtk_widget_show_all(window);

	/* show a cross cursor over the drawing areas */
	gdk_window_set_cursor(appl->darea_1->window, gdk_cursor_new(GDK_CROSS));
	gdk_window_set_cursor(appl->darea_2->window, gdk_cursor_new(GDK_CROSS));
}

/* create frame and entry field for current value */
static GtkWidget *show_entry(GtkWidget * hbox, const gchar * text)
{
	GtkWidget *frame, *entry;

	frame = gtk_frame_new(text);
	gtk_container_add(GTK_CONTAINER(hbox), frame);

	entry = gtk_entry_new();
	gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE);
	gtk_container_add(GTK_CONTAINER(frame), entry);

	return entry;
}

/* current horizontal and vertical value callback */
static gint show_motion_notify_cb(GtkWidget * darea, GdkEventMotion * event,
				  GtkWidget ** entries)
{
	GtkDataboxCoord coord;
	GtkDataboxValue value;
	GtkDatabox *box = GTK_DATABOX(darea);
	gchar *text;

	coord.x = event->x;
	coord.y = event->y;

	value = gtk_databox_value_from_coord(box, coord);

	text = g_strdup_printf("%g", value.x);
	gtk_entry_set_text(GTK_ENTRY(entries[SHOW_ACTUAL_X]), text);
	g_free(text);
	text = g_strdup_printf("%g", value.y);
	gtk_entry_set_text(GTK_ENTRY(entries[SHOW_ACTUAL_Y]), text);
	g_free(text);

	return FALSE;
}

/* callback of zoom in button */
static void zoom_in_cb(void)
{
	message(_
		("Select an area on the drawing area and left-click on it\nin order to zoom in."));
}

/* callback of zoom out button */
static void zoom_out_cb(void)
{
	message(_("Right-click on the drawing area\nin order to zoom out."));
}

/* callback of about button */
static void about_cb(void)
{
	gchar *text;

	text = g_strdup_printf(_("<b>%s %s</b>\n\nThis program is released \
under the GPL.\nHome page: http://cournol.k-2007.de/\n\
Copyright (C) (2007) (Fabian Förg) &lt;fabian.foerg@gmx.de&gt;"), PACKAGE, VERSION);
	message(text);
	g_free(text);
}

/* go back to input */
static void back_cb(appl_t * appl)
{
	values_t *val = &(appl->values);

	/* find out which option was selected */
	switch (val->type) {
	case 1:
	case 4:
		input_1(appl);
		break;
	case 2:
	case 5:
		input_2(appl);
		break;
	default:
		input_3(appl);
		break;
	}
}

static void print_dialog(appl_t * appl)
{
	GtkPrintOperation *op;
	GtkPrintOperationResult res;

	op = gtk_print_operation_new();

	gtk_print_operation_set_n_pages(op, 1);
	gtk_print_operation_set_unit(op, GTK_UNIT_MM);
	g_signal_connect(op, "draw_page", G_CALLBACK(draw_page), appl);
	res =
	    gtk_print_operation_run(op, GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG,
				    NULL, NULL);
}

static void draw_page(GtkPrintOperation * operation,
		      GtkPrintContext * print_context, gint page_nr,
		      appl_t * appl)
{
	GtkWidget *window;
	GdkPixbuf *pixbuf;
	gdouble scale;
	cairo_t *cr;

	/* get a cairo_context of the print_context */
	cr = gtk_print_context_get_cairo_context(print_context);

	/* create the print_win */
	window = create_print_win(appl);
	gtk_widget_show_all(window);

	/* make sure the win is shown before grabbing the screenshot */
	while (gtk_events_pending())
		gtk_main_iteration();

	g_usleep(400000);

	gdk_flush();

	/* make a screenshot */
	pixbuf =
	    gdk_pixbuf_get_from_drawable(NULL, window->window, NULL,
					 0, 0, 0, 0,
					 window->allocation.width,
					 window->allocation.height);

	/* calculate scale factor */
	scale =
	    (gdouble) gtk_print_context_get_width(print_context) /
	    window->allocation.width;

	/* scale, keep aspect ratio */
	cairo_scale(cr, scale, scale);

	gdk_cairo_set_source_pixbuf(cr, pixbuf, 0., 0.);

	cairo_paint(cr);

	/* destoy print_win */
	if (window != NULL)
		gtk_widget_destroy(window);
	else
		g_print("The print window was closed unexpectedly.");
}

/* create the window to be printed */
static GtkWidget *create_print_win(appl_t * appl)
{
	GtkWidget *window, *vbox;
	gboolean scrollbar = FALSE;
	GdkColor color;

	/* create window */
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), _("Print Window - " PACKAGE));
	gtk_window_maximize(GTK_WINDOW(window));
	g_signal_connect(G_OBJECT(window), "delete_event",
			 G_CALLBACK(delete_event_cb), NULL);

	/* create main vbox for the table and hboxes */
	vbox = create_main_vbox(appl, scrollbar);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	/* set background color of the print window to white */
	color.red = 65535;
	color.green = 65535;
	color.blue = 65535;

	gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &color);

	gtk_widget_realize(window);

	while (gtk_events_pending())
		gtk_main_iteration();

	g_usleep(400000);

	return window;
}

static gboolean delete_event_cb(void)
{
	return TRUE;
}
