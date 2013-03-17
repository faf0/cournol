/* cournol
 * Copyright (C) (2006) (Fabian Förg) <fabian.foerg@gmx.de>
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

#include "input.h"
#include "main.h"
#include "options.h"
#include "get_values.h"

#ifdef HAVE_CONFIG_H
# include "../config.h"
#else
#ifdef WIN32
# include "../config.h.w32"
#endif
#endif

#define COL_SPACINGS 20
#define ROW_SPACINGS 10

static void input(GtkWidget * table_cost, appl_t * appl);
static GtkWidget *init_table_price(appl_t * appl);
static GtkWidget *init_table_price_sales(appl_t * appl);

void input_1(appl_t * appl)
{
	GtkWidget *table, *label;
	GtkAdjustment *adj;
	sbuttons_t *sbutton = &(appl->sbuttons);
	values_t *val = &(appl->values);

	/* create cost function table */
	table = gtk_table_new(3, 2, FALSE);
	gtk_table_set_col_spacings(GTK_TABLE(table), COL_SPACINGS);
	gtk_table_set_row_spacings(GTK_TABLE(table), ROW_SPACINGS);

	/* linear cost function labels */
	label = gtk_label_new(_("<b>Cost function \'C(x) = ax + b\'</b>"));
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 2, 0, 1);

	label = gtk_label_new("a :");
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 1, 2);

	label = gtk_label_new("b :");
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 1, 2);

	/* spin button a */
	adj = (GtkAdjustment *) gtk_adjustment_new(val->a, 0.01, G_MAXFLOAT,
						   1.0, 10.0, 0.0);
	sbutton->a = gtk_spin_button_new(adj, 0.0, 2);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(sbutton->a), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), sbutton->a, 0, 1, 2, 3);

	/* spin button b */
	adj = (GtkAdjustment *) gtk_adjustment_new(val->b, 0.0, G_MAXFLOAT,
						   1.0, 10.0, 0.0);
	sbutton->b = gtk_spin_button_new(adj, 0.0, 2);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(sbutton->b), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), sbutton->b, 1, 2, 2, 3);

	input(table, appl);
}

void input_2(appl_t * appl)
{
	GtkWidget *table, *label;
	GtkAdjustment *adj;
	sbuttons_t *sbutton = &(appl->sbuttons);
	values_t *val = &(appl->values);

	/* create cost function table */
	table = gtk_table_new(5, 3, FALSE);
	gtk_table_set_col_spacings(GTK_TABLE(table), COL_SPACINGS);
	gtk_table_set_row_spacings(GTK_TABLE(table), ROW_SPACINGS);

	/* quadratic cost function labels */
	label =
	    gtk_label_new(_("<b>Cost function \'C(x) = ax² + bx + c\'</b>"));
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 3, 0, 1);

	label = gtk_label_new("a :");
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 1, 2);

	label = gtk_label_new("b :");
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 1, 2);

	label = gtk_label_new("c :");
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 1, 2);

	/* spin button a */
	adj = (GtkAdjustment *) gtk_adjustment_new(val->a, 0.01, G_MAXFLOAT,
						   1.0, 10.0, 0.0);
	sbutton->a = gtk_spin_button_new(adj, 0.0, 2);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(sbutton->a), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), sbutton->a, 0, 1, 2, 3);

	/* spin button b */
	adj =
	    (GtkAdjustment *) gtk_adjustment_new(val->b, -G_MAXFLOAT,
						 G_MAXFLOAT, 1.0, 10.0, 0.0);
	sbutton->b = gtk_spin_button_new(adj, 0.0, 2);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(sbutton->b), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), sbutton->b, 1, 2, 2, 3);

	/* spin button c */
	adj =
	    (GtkAdjustment *) gtk_adjustment_new(val->c, 0.0, G_MAXFLOAT,
						 1.0, 10.0, 0.0);
	sbutton->c = gtk_spin_button_new(adj, 0.0, 2);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(sbutton->c), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), sbutton->c, 2, 3, 2, 3);

	input(table, appl);
}

void input_3(appl_t * appl)
{
	GtkWidget *table, *label;
	GtkAdjustment *adj;
	sbuttons_t *sbutton = &(appl->sbuttons);
	values_t *val = &(appl->values);

	/* create cost function table */
	table = gtk_table_new(4, 3, FALSE);
	gtk_table_set_col_spacings(GTK_TABLE(table), COL_SPACINGS);
	gtk_table_set_row_spacings(GTK_TABLE(table), ROW_SPACINGS);

	/* sigmoid cost function labels */
	label = gtk_label_new
	    (_("<b>Cost function \'C(x) = ax³ + bx² + cx + d\'</b>"));
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 4, 0, 1);

	label = gtk_label_new("a :");
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 1, 2);

	label = gtk_label_new("b :");
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 1, 2);

	label = gtk_label_new("c :");
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 1, 2);

	label = gtk_label_new("d :");
	gtk_table_attach_defaults(GTK_TABLE(table), label, 3, 4, 1, 2);

	/* spin button a */
	adj = (GtkAdjustment *) gtk_adjustment_new(val->a, 0.01, G_MAXFLOAT,
						   1.0, 10.0, 0.0);
	sbutton->a = gtk_spin_button_new(adj, 0.0, 2);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(sbutton->a), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), sbutton->a, 0, 1, 2, 3);

	/* spin button b */
	adj = (GtkAdjustment *) gtk_adjustment_new(val->b, -G_MAXFLOAT,
						   G_MAXFLOAT, 1.0, 10.0, 0.0);
	sbutton->b = gtk_spin_button_new(adj, 0.0, 2);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(sbutton->b), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), sbutton->b, 1, 2, 2, 3);

	/* spin button c */
	adj = (GtkAdjustment *) gtk_adjustment_new(val->c, -G_MAXFLOAT,
						   G_MAXFLOAT, 1.0, 10.0, 0.0);
	sbutton->c = gtk_spin_button_new(adj, 0.0, 2);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(sbutton->c), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), sbutton->c, 2, 3, 2, 3);

	/* spin button d */
	adj = (GtkAdjustment *) gtk_adjustment_new(val->d, 0.0, G_MAXFLOAT,
						   1.0, 10.0, 0.0);
	sbutton->d = gtk_spin_button_new(adj, 0.0, 2);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(sbutton->d), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), sbutton->d, 3, 4, 2, 3);

	input(table, appl);
}

static void input(GtkWidget * table_cost, appl_t * appl)
{
	GtkWidget *window, *vbox, *table_spec, *table_buttons, *button;
	values_t *val = &(appl->values);
	GCallback callback;

	/* create window */
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(window), _("Input - " PACKAGE));
	g_signal_connect(G_OBJECT(window), "delete_event",
			 G_CALLBACK(gtk_main_quit), NULL);

	/* create vbox for the two tables and buttons */
	vbox = gtk_vbox_new(FALSE, 20);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	/* create cost function table */
	gtk_box_pack_start(GTK_BOX(vbox), table_cost, TRUE, TRUE, 0);

	if (val->type <= 3)
		table_spec = init_table_price(appl);
	else
		table_spec = init_table_price_sales(appl);

	gtk_box_pack_start(GTK_BOX(vbox), table_spec, TRUE, TRUE, 0);

	/* create button table */
	table_buttons = gtk_table_new(1, 2, FALSE);
	gtk_table_set_col_spacings(GTK_TABLE(table_buttons), COL_SPACINGS);
	gtk_table_set_row_spacings(GTK_TABLE(table_buttons), ROW_SPACINGS);

	/* back button: call options and destroy window */
	button = gtk_button_new_from_stock(GTK_STOCK_GO_BACK);
	g_signal_connect_swapped(G_OBJECT(button), "clicked",
				 G_CALLBACK(options), appl);
	g_signal_connect_swapped(G_OBJECT(button), "clicked",
				 G_CALLBACK(gtk_widget_destroy), window);
	gtk_table_attach_defaults(GTK_TABLE(table_buttons), button, 0, 1, 0, 1);

	/* init wanted callback function */
	switch (val->type) {
	case 1:
	case 4:
		callback = G_CALLBACK(get_values_1);
		break;
	case 2:
	case 5:
		callback = G_CALLBACK(get_values_2);
		break;
	case 3:
	case 6:
	default:
		callback = G_CALLBACK(get_values_3);
		break;
	}

	/* create go forward button, call appropriate get_values function */
	button = gtk_button_new_from_stock(GTK_STOCK_GO_FORWARD);
	g_signal_connect_swapped(G_OBJECT(button), "clicked", callback, appl);
	g_signal_connect_swapped(G_OBJECT(button), "clicked",
				 G_CALLBACK(gtk_widget_destroy), window);
	gtk_table_attach_defaults(GTK_TABLE(table_buttons), button, 1, 2, 0, 1);

	gtk_box_pack_end(GTK_BOX(vbox), table_buttons, TRUE, TRUE, 0);

	/* display window with all widgets */
	gtk_widget_show_all(window);
}

static GtkWidget *init_table_price(appl_t * appl)
{
	GtkWidget *table_price, *label;
	GtkAdjustment *adj;
	sbuttons_t *sbutton = &(appl->sbuttons);
	values_t *val = &(appl->values);

	/* create price, capacity and button table */
	table_price = gtk_table_new(2, 2, FALSE);
	gtk_table_set_col_spacings(GTK_TABLE(table_price), COL_SPACINGS);
	gtk_table_set_row_spacings(GTK_TABLE(table_price), ROW_SPACINGS);

	/* price label */
	label = gtk_label_new(_("Market price:"));
	gtk_table_attach_defaults(GTK_TABLE(table_price), label, 0, 1, 0, 1);

	/* spin button e = market price */
	adj = (GtkAdjustment *) gtk_adjustment_new(val->e, 0.01, G_MAXFLOAT,
						   1.0, 10.0, 0.0);
	sbutton->e = gtk_spin_button_new(adj, 0.0, 2);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(sbutton->e), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table_price), sbutton->e,
				  0, 1, 1, 2);

	/* maximum capacity label */
	label = gtk_label_new(_("Maximum capacity:"));
	gtk_table_attach_defaults(GTK_TABLE(table_price), label, 1, 2, 0, 1);

	/* spin button f = maximum capacity */
	adj = (GtkAdjustment *) gtk_adjustment_new(val->f, 1.0, G_MAXFLOAT,
						   1.0, 10.0, 0.0);
	sbutton->f = gtk_spin_button_new(adj, 0.0, 0);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(sbutton->f), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table_price), sbutton->f,
				  1, 2, 1, 2);

	return table_price;
}

static GtkWidget *init_table_price_sales(appl_t * appl)
{
	GtkWidget *table_price_sales, *label;
	GtkAdjustment *adj;
	sbuttons_t *sbutton = &(appl->sbuttons);
	values_t *val = &(appl->values);

	/* create price-sales-function and button table */
	table_price_sales = gtk_table_new(3, 2, FALSE);
	gtk_table_set_col_spacings(GTK_TABLE(table_price_sales), COL_SPACINGS);
	gtk_table_set_row_spacings(GTK_TABLE(table_price_sales), ROW_SPACINGS);

	/* price-sales-function labels */
	label =
	    gtk_label_new(_("<b>Price-sales-function \'p(x) = ex + f\'</b>"));
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table_price_sales), label,
				  0, 2, 0, 1);

	label = gtk_label_new("e :");
	gtk_table_attach_defaults(GTK_TABLE(table_price_sales), label,
				  0, 1, 1, 2);

	label = gtk_label_new("f :");
	gtk_table_attach_defaults(GTK_TABLE(table_price_sales), label,
				  1, 2, 1, 2);

	/* spin button e */
	adj = (GtkAdjustment *) gtk_adjustment_new(val->e, -G_MAXFLOAT, -0.01,
						   1.0, 10.0, 0.0);
	sbutton->e = gtk_spin_button_new(adj, 0.0, 2);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(sbutton->e), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table_price_sales), sbutton->e,
				  0, 1, 2, 3);

	/* spin button f */
	adj = (GtkAdjustment *) gtk_adjustment_new(val->f, 0.01, G_MAXFLOAT,
						   1.0, 10.0, 0.0);
	sbutton->f = gtk_spin_button_new(adj, 0.0, 2);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(sbutton->f), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table_price_sales), sbutton->f,
				  1, 2, 2, 3);

	return table_price_sales;
}
