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

#include "options.h"
#include "main.h"
#include "init_values.h"

#ifdef HAVE_CONFIG_H
# include "../config.h"
#else
#ifdef WIN32
# include "../config.h.w32"
#endif
#endif

static void options_cb(appl_t * appl);

void options(appl_t * appl)
{
	GtkWidget *window, *vbox, *label, *button;
	rbuttons_t *var = &(appl->rbuttons);

	/* create window */
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(window), _("Options - " PACKAGE));
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	g_signal_connect(G_OBJECT(window), "delete_event",
			 G_CALLBACK(gtk_main_quit), NULL);

	/* create vbox for the label and buttons */
	vbox = gtk_vbox_new(FALSE, 20);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	label =
	    gtk_label_new(_
			  ("<b>Analysis of an absolute supply monopoly or a polypoly.</b>"));
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);

	label = gtk_label_new(_("Choose the market type:"));
	gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);

	/* button for polypoly */
	var->rbutton_poly =
	    gtk_radio_button_new_with_label(NULL, _("Polypoly"));
	gtk_box_pack_start(GTK_BOX(vbox), var->rbutton_poly, TRUE, TRUE, 0);

	/* button for monopoly */
	var->rbutton_mono =
	    gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON
							(var->rbutton_poly),
							_("Monopoly"));
	gtk_box_pack_start(GTK_BOX(vbox), var->rbutton_mono, TRUE, TRUE, 0);

	label = gtk_label_new(_("Choose the cost function type:"));
	gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);

	/* button for option 1: linear cost function */
	var->rbutton_1 =
	    gtk_radio_button_new_with_label(NULL, _("Linear cost function"));
	gtk_box_pack_start(GTK_BOX(vbox), var->rbutton_1, TRUE, TRUE, 0);

	/* button for option 2: quadratic cost function */
	var->rbutton_2 =
	    gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON
							(var->rbutton_1),
							_
							("Quadratic cost function"));
	gtk_box_pack_start(GTK_BOX(vbox), var->rbutton_2, TRUE, TRUE, 0);

	/* button for option 3: sigmoid cost function */
	var->rbutton_3 =
	    gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON
							(var->rbutton_2),
							_
							("Sigmoid cost function"));
	gtk_box_pack_start(GTK_BOX(vbox), var->rbutton_3, TRUE, TRUE, 0);

	/* go forward button */
	button = gtk_button_new_from_stock(GTK_STOCK_GO_FORWARD);
	g_signal_connect_swapped(G_OBJECT(button), "clicked",
				 G_CALLBACK(options_cb), appl);
	g_signal_connect_swapped(G_OBJECT(button), "clicked",
				 G_CALLBACK(gtk_widget_destroy), window);
	gtk_box_pack_end(GTK_BOX(vbox), button, FALSE, TRUE, 0);

	/* display window with all widgets */
	gtk_widget_show_all(window);
}

static void options_cb(appl_t * appl)
{
	rbuttons_t *var = &(appl->rbuttons);
	values_t *val = &(appl->values);

	/* find out which options were selected */
	if (GTK_TOGGLE_BUTTON(var->rbutton_poly)->active) {
		if (GTK_TOGGLE_BUTTON(var->rbutton_1)->active) {
			val->type = 1;
			init_values_poly_1(appl);
		} else if (GTK_TOGGLE_BUTTON(var->rbutton_2)->active) {
			val->type = 2;
			init_values_poly_2(appl);
		} else {
			val->type = 3;
			init_values_poly_3(appl);
		}
	} else {
		if (GTK_TOGGLE_BUTTON(var->rbutton_1)->active) {
			val->type = 4;
			init_values_mono_1(appl);
		} else if (GTK_TOGGLE_BUTTON(var->rbutton_2)->active) {
			val->type = 5;
			init_values_mono_2(appl);
		} else {
			val->type = 6;
			init_values_mono_3(appl);
		}
	}
}
