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

#ifdef HAVE_CONFIG_H
# include "../config.h"
#else
#ifdef WIN32
# include "../config.h.w32"
#endif
#endif

#include "message.h"

void message(const gchar * text)
{
	GtkWidget *window, *vbox, *label, *button;

	/* create window */
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(window), _("Message - " PACKAGE));
	g_signal_connect(G_OBJECT(window), "delete_event",
			 G_CALLBACK(gtk_widget_destroy), NULL);

	/* create vbox for the label and button */
	vbox = gtk_vbox_new(FALSE, 20);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	/* display text */
	label = gtk_label_new(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);

	/* create close button */
	button = gtk_button_new_from_stock(GTK_STOCK_CLOSE);
	g_signal_connect_swapped(G_OBJECT(button), "clicked",
				 G_CALLBACK(gtk_widget_destroy), window);
	gtk_box_pack_end(GTK_BOX(vbox), button, FALSE, TRUE, 0);

	/* display window with all widgets */
	gtk_widget_show_all(window);
}
