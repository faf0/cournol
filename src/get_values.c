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

#include "get_values.h"
#include "main.h"
#include "init_points.h"

static void get_generic(appl_t * appl);

void get_values_1(appl_t * appl)
{
	/* get values from spin buttons */
	get_generic(appl);

	init_points_1(appl);
}

void get_values_2(appl_t * appl)
{
	values_t *val = &(appl->values);
	sbuttons_t *sbutton = &(appl->sbuttons);

	/* get values from spin buttons */
	get_generic(appl);
	val->c = gtk_spin_button_get_value(GTK_SPIN_BUTTON(sbutton->c));

	init_points_2(appl);
}

void get_values_3(appl_t * appl)
{
	values_t *val = &(appl->values);
	sbuttons_t *sbutton = &(appl->sbuttons);

	/* get values from spin buttons */
	get_generic(appl);
	val->c = gtk_spin_button_get_value(GTK_SPIN_BUTTON(sbutton->c));
	val->d = gtk_spin_button_get_value(GTK_SPIN_BUTTON(sbutton->d));

	init_points_3(appl);
}

static void get_generic(appl_t * appl)
{
	values_t *val = &(appl->values);
	sbuttons_t *sbutton = &(appl->sbuttons);

	/* get entered values a, b, e and f from spin buttons */
	val->a = gtk_spin_button_get_value(GTK_SPIN_BUTTON(sbutton->a));
	val->b = gtk_spin_button_get_value(GTK_SPIN_BUTTON(sbutton->b));
	val->e = gtk_spin_button_get_value(GTK_SPIN_BUTTON(sbutton->e));
	val->f = gtk_spin_button_get_value(GTK_SPIN_BUTTON(sbutton->f));
}
