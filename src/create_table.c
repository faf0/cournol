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
#include <math.h>
#include <glib/gi18n.h>

#include "create_table.h"
#include "main.h"
#include "input.h"
#include "message.h"

#define COL_SPACINGS 20
#define ROW_SPACINGS 10

#ifndef SQR
#define SQR(a) ((a) * (a))
#endif

#ifndef THR
#define THR(a) ((a) * (a) * (a))
#endif

GtkWidget *create_table_poly_1(appl_t * appl)
{
	values_t *val = &(appl->values);
	GtkWidget *table, *label;
	gchar *text;

	/* create info table for poly 1 */
	table = gtk_table_new(6, 3, FALSE);
	gtk_table_set_col_spacings(GTK_TABLE(table), COL_SPACINGS);
	gtk_table_set_row_spacings(GTK_TABLE(table), ROW_SPACINGS);

	/* Cost functions texts */
	label = gtk_label_new(_("Cost functions"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 0, 1);

	/* C(x) text */
	text =
	    g_strdup_printf(_("<span color=\"#15a40a\">C(x) = %gx %+g</span>"),
			    val->a, val->b);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 0, 1);

	/* C'(x) text */
	text =
	    g_strdup_printf(_("<span color=\"#15a40a\">C\'(x) = %g</span>"),
			    val->a);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 0, 1);

	/* c(x) text */
	text = g_strdup_printf(_("c(x) = %g %+g/x"), val->a, val->b);
	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 3, 1, 2);

	/* Cv(x) text */
	text = g_strdup_printf(_("Cv(x) = %gx"), val->a);
	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 2, 3);

	/* cv(x) text */
	text =
	    g_strdup_printf(_("<span color=\"#e71234\">cv(x) = %g</span>"),
			    val->a);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 2, 3);

	/* Revenue function text, market price */
	label = gtk_label_new(_("Revenue function"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 3, 4);

	/* R(x) text */
	text =
	    g_strdup_printf(_("<span color=\"#2304ee\">R(x) = %gx</span>"),
			    val->e);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 3, 4);

	/* market price */
	text =
	    g_strdup_printf(_
			    ("<span color=\"#2304ee\">Market price, R\'(x) = %g</span>"),
			    val->e);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 3, 4);

	/* Maximum profit texts */
	label = gtk_label_new(_("Maximum profit"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 4, 5);

	if ((val->a + val->b / val->f) < val->e)
		text = g_strdup_printf(_("Quantity: %g"), val->f);
	else
		text = g_strdup_printf(_("Quantity: -"));

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 4, 5);

	if ((val->a + val->b / val->f) < val->e) {
		val->price = val->f * (val->e - val->a) - val->b;

		text = g_strdup_printf(_("Amount: %g"), val->price);
	} else
		text = g_strdup_printf(_("Amount: -"));

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 4, 5);

	/* profit range */
	label = gtk_label_new(_("Profit range"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 5, 6);

	if (val->e > val->a) {
		val->quantity = val->b / (val->e - val->a);

		text =
		    g_strdup_printf(_("Break-even-point: %g"), val->quantity);
	} else
		text = g_strdup_printf(_("Break-even-point: -"));

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 3, 5, 6);

	return table;
}

GtkWidget *create_table_poly_2(appl_t * appl)
{
	values_t *val = &(appl->values);
	GtkWidget *table, *label;
	gchar *text;
	gfloat discr;

	/* create info table for poly 2 */
	table = gtk_table_new(7, 3, FALSE);
	gtk_table_set_col_spacings(GTK_TABLE(table), COL_SPACINGS);
	gtk_table_set_row_spacings(GTK_TABLE(table), ROW_SPACINGS);

	/* Cost functions texts */
	label = gtk_label_new(_("Cost functions"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 0, 1);

	/* C(x) text */
	text =
	    g_strdup_printf(_
			    ("<span color=\"#15a40a\">C(x) = %gx² %+gx %+g</span>"),
			    val->a, val->b, val->c);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 0, 1);

	/* C'(x) text */
	text =
	    g_strdup_printf(_
			    ("<span color=\"#15a40a\">C\'(x) = %gx %+g</span>"),
			    2. * val->a, val->b);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 0, 1);

	/* c(x) text */
	text =
	    g_strdup_printf(_("c(x) = %gx %+g %+g/x"), val->a, val->b, val->c);
	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 3, 1, 2);

	/* Cv(x) text */
	text =
	    g_strdup_printf(_
			    ("Cv(x) = %gx² %+gx %+g"), val->a, val->b, val->c);
	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 2, 3);

	/* cv(x) text */
	text =
	    g_strdup_printf(_
			    ("<span color=\"#e71234\">cv(x) = %gx %+g</span>"),
			    val->a, val->b);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 2, 3);

	/* Revenue function text, market price */
	label = gtk_label_new(_("Revenue function"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 3, 4);

	/* R(x) text */
	text =
	    g_strdup_printf(_("<span color=\"#2304ee\">R(x) = %gx</span>"),
			    val->e);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 3, 4);

	/* market price */
	text =
	    g_strdup_printf(_
			    ("<span color=\"#2304ee\">Market price, R\'(x) = %g</span>"),
			    val->e);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 3, 4);

	/* Maximum profit texts */
	label = gtk_label_new(_("Maximum profit"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 4, 5);

	val->quantity = (val->e - val->b) / (2. * val->a);
	if (val->quantity < 0.)
		text = g_strdup_printf(_("Quantity: -"));
	else
		text = g_strdup_printf(_("Quantity: %g"), val->quantity);

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 4, 5);

	if (val->quantity < 0.)
		text = g_strdup_printf(_("Amount: -"));
	else {
		val->price =
		    val->quantity * (-val->a * val->quantity - val->b +
				     val->e) - val->c;

		text = g_strdup_printf(_("Amount: %g"), val->price);
	}

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 4, 5);

	/* Profit range */
	label = gtk_label_new(_("Profit range"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 5, 6);

	discr = SQR(val->b - val->e) - 4. * val->a * val->c;
	if (discr < 0.) {
		text = g_strdup_printf(_("Break-even-point: -"));
	} else if (discr == 0.) {
		val->quantity = (-val->b + val->e) / (2. * val->a);

		text =
		    g_strdup_printf(_("Break-even-point: %g"), val->quantity);
	} else {
		val->quantity =
		    (-val->b + val->e - sqrt(discr)) / (2. * val->a);

		text =
		    g_strdup_printf(_("Break-even-point: %g"), val->quantity);
	}

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 5, 6);

	if (discr > 0.) {
		val->quantity =
		    (-val->b + val->e + sqrt(discr)) / (2. * val->a);

		text = g_strdup_printf(_("End: %g"), val->quantity);
	} else
		text = g_strdup_printf(_("End: -"));

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 5, 6);

	/* cost optimum */
	label = gtk_label_new(_("Cost optimum"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 6, 7);

	if (val->c < 0.)
		text = g_strdup_printf(_("Quantity: -"));
	else {
		val->quantity = sqrt(val->c / val->a);

		text = g_strdup_printf(_("Quantity: %g"), val->quantity);
	}

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 6, 7);

	if (val->c < 0.)
		text = g_strdup_printf(_("Price: -"));
	else {
		val->price = 2. * sqrt(val->a * val->c) + val->b;

		text = g_strdup_printf(_("Price: %g"), val->price);
	}

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 6, 7);

	return table;
}

GtkWidget *create_table_poly_3(appl_t * appl)
{
	values_t *val = &(appl->values);
	GtkWidget *table, *label;
	gchar *text;
	gfloat discr, p, q;

	/* create info table for poly 3 */
	table = gtk_table_new(8, 3, FALSE);
	gtk_table_set_col_spacings(GTK_TABLE(table), COL_SPACINGS);
	gtk_table_set_row_spacings(GTK_TABLE(table), ROW_SPACINGS);

	/* Cost functions texts */
	label = gtk_label_new(_("Cost functions"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 0, 1);

	/* C(x) text */
	text =
	    g_strdup_printf(_
			    ("<span color=\"#15a40a\">C(x) = %gx³ %+gx² %+gx %+g</span>"),
			    val->a, val->b, val->c, val->d);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 0, 1);

	/* C'(x) text */
	text =
	    g_strdup_printf(_
			    ("<span color=\"#15a40a\">C\'(x) = %gx² %+gx %+g</span>"),
			    3. * val->a, 2. * val->b, val->c);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 0, 1);

	/* c(x) text */
	text =
	    g_strdup_printf(_
			    ("c(x) = %gx² %+gx %+g %+g/x"),
			    val->a, val->b, val->c, val->d);
	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 3, 1, 2);

	/* Cv(x) text */
	text =
	    g_strdup_printf(_
			    ("Cv(x) = %gx³ %+gx² %+gx"),
			    val->a, val->b, val->c);
	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 2, 3);

	/* cv(x) text */
	text =
	    g_strdup_printf(_
			    ("<span color=\"#e71234\">cv(x) = %gx² %+gx %+g</span>"),
			    val->a, val->b, val->c);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 2, 3);

	/* Revenue function text, market price */
	label = gtk_label_new(_("Revenue function"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 3, 4);

	/* R(x) text */
	text =
	    g_strdup_printf(_("<span color=\"#2304ee\">R(x) = %gx</span>"),
			    val->e);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 3, 4);

	/* market price */
	text =
	    g_strdup_printf(_
			    ("<span color=\"#2304ee\">Market price, R\'(x) = %g</span>"),
			    val->e);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 3, 4);

	/* Maximum profit texts */
	label = gtk_label_new(_("Maximum profit"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 4, 5);

	discr = 4. * SQR(val->b) + 12. * val->a * (val->e - val->c);
	if (discr < 0.)
		val->quantity = -1.;
	else if (discr == 0.)
		val->quantity = -val->b / (3. * val->a);
	else {
		gfloat quantity_2;

		val->quantity = (2. * val->b + sqrt(discr)) / (-6. * val->a);
		quantity_2 = (2. * val->b - sqrt(discr)) / (-6. * val->a);

		val->quantity = MAX(val->quantity, quantity_2);
	}

	if (val->quantity >= 0.)
		text = g_strdup_printf(_("Quantity: %g"), val->quantity);
	else
		text = g_strdup_printf(_("Quantity: -"));

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 4, 5);

	if (val->quantity >= 0.) {
		val->price =
		    val->quantity * (-val->quantity *
				     (val->a * val->quantity + val->b) +
				     val->e - val->c) - val->d;

		text = g_strdup_printf(_("Amount: %g"), val->price);
	} else
		text = g_strdup_printf(_("Amount: -"));

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 4, 5);

	/* Profit range */
	label = gtk_label_new(_("Profit range"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 5, 6);

	p = (3. * val->a * (val->c - val->e) -
	     SQR(val->b)) / (3. * SQR(val->a));
	q = (2. * THR(val->b)) / (27. * THR(val->a)) -
	    (val->b * (val->c - val->e)) / (3. * SQR(val->a)) + val->d / val->a;

	discr = 4. * THR(p) + 27. * SQR(q);
	if (discr < 0.) {
		gfloat u1;

		u1 = -sqrt(-4. / 3. * p) * cos(1. / 3. *
					       acos(-q / 2. *
						    sqrt(-27. / THR(p))) +
					       M_PI / 3.);
		u1 -= val->b / (3. * val->a);

		text = g_strdup_printf(_("Break-even-point: %g"), u1);
	} else
		text = g_strdup_printf(_("Break-even-point: -"));

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 5, 6);

	if (discr < 0.) {
		gfloat u2;

		u2 = sqrt(-4. / 3. * p) * cos(1. / 3. *
					      acos(-q / 2. *
						   sqrt(-27. / THR(p))));
		u2 -= val->b / (3. * val->a);

		text = g_strdup_printf(_("End: %g"), u2);
	} else
		text = g_strdup_printf(_("End: -"));

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 5, 6);

	/* cost optimum */
	label = gtk_label_new(_("Cost optimum"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 6, 7);

	p = -SQR(val->b / val->a) / 12.;
	q = THR(val->b / val->a) / 108. - val->d / (2. * val->a);

	discr = 4. * THR(p) + 27. * SQR(q);
	if (discr > 0.) {
		gfloat u, v;

		v = sqrt(SQR(q / 2.) + THR(p / 3.));
		u = pow(-q / 2. + v, 1. / 3.) + pow(-q / 2. - v, 1. / 3.);
		val->quantity = u - val->b / (6. * val->a);

		text = g_strdup_printf(_("Quantity: %g"), val->quantity);
	} else
		text = g_strdup_printf(_("Quantity: -"));

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 6, 7);

	if (discr > 0.) {
		val->price =
		    val->quantity * (val->a * val->quantity + val->b) + val->c +
		    val->d / val->quantity;

		text = g_strdup_printf(_("Price: %g"), val->price);
	} else
		text = g_strdup_printf(_("Price: -"));

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 6, 7);

	/* cost minimum */
	label = gtk_label_new(_("Cost minimum"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 7, 8);

	val->quantity = -val->b / (2. * val->a);
	if (val->quantity <= 0.)
		text = g_strdup_printf(_("Quantity: -"));
	else
		text = g_strdup_printf(_("Quantity: %g"), val->quantity);

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 7, 8);

	if (val->quantity <= 0.)
		text = g_strdup_printf(_("Price: -"));
	else {
		val->price =
		    val->quantity * (val->a * val->quantity + val->b) + val->c;

		text = g_strdup_printf(_("Price: %g"), val->price);
	}

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 7, 8);

	return table;
}

GtkWidget *create_table_mono_1(appl_t * appl)
{
	GtkWidget *table, *label;
	gchar *text;
	values_t *val = &(appl->values);
	gfloat amount, discr;

	/* create a table for the texts */
	table = gtk_table_new(6, 3, FALSE);
	gtk_table_set_col_spacings(GTK_TABLE(table), COL_SPACINGS);
	gtk_table_set_row_spacings(GTK_TABLE(table), ROW_SPACINGS);

	/* Cost functions texts */
	label = gtk_label_new(_("Cost functions"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 0, 1);

	/* C(x) text */
	text =
	    g_strdup_printf(_("<span color=\"#15a40a\">C(x) = %gx %+g</span>"),
			    val->a, val->b);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 0, 1);

	/* C'(x) text */
	text =
	    g_strdup_printf(_("<span color=\"#15a40a\">C\'(x) = %g</span>"),
			    val->a);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 0, 1);

	/* Price-sales-function text */
	label = gtk_label_new(_("Price-sales-function"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 1, 2);

	text =
	    g_strdup_printf(_("<span color=\"#e71234\">p(x) = %gx %+g</span>"),
			    val->e, val->f);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 3, 1, 2);

	/* Revenue functions texts */
	label = gtk_label_new(_("Revenue functions"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 2, 3);

	/* R(x) text */
	text =
	    g_strdup_printf(_
			    ("<span color=\"#2304ee\">R(x) = %gx² %+gx</span>"),
			    val->e, val->f);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 2, 3);

	/* R'(x) text */
	text =
	    g_strdup_printf(_
			    ("<span color=\"#2304ee\">R\'(x) = %gx %+g</span>"),
			    2. * val->e, val->f);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 2, 3);

	/* Maximum profit texts */
	label = gtk_label_new(_("Maximum profit"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 3, 4);

	/* initialize quantity */
	val->quantity = (val->a - val->f) / (2. * val->e);

	if (val->quantity > 0.) {
		text = g_strdup_printf(_("Quantity: %g"), val->quantity);

		val->cournot_points = 1;
	} else
		text = g_strdup_printf(_("Quantity: -"));

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 3, 4);

	if (val->cournot_points != 0) {
		val->price = val->e * val->quantity + val->f;

		text = g_strdup_printf(_("Price: %g"), val->price);
	} else
		text = g_strdup_printf(_("Price: -"));

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 3, 4);

	if (val->cournot_points != 0) {
		amount =
		    val->quantity * (val->e * val->quantity + val->f - val->a) -
		    val->b;

		text = g_strdup_printf(_("Amount: %g"), amount);
	} else
		text = g_strdup_printf(_("Amount: -"));

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 3, 4, 5);

	/* Profit range texts */
	label = gtk_label_new(_("Profit range"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 5, 6);

	discr = SQR(val->f - val->a) + 4. * val->e * val->b;
	if (discr < 0.)
		text = g_strdup_printf(_("Break-even-point: -"));
	else if (discr == 0.) {
		gfloat prof_beg;

		prof_beg = (-val->f + val->a) / (2. * val->e);
		text = g_strdup_printf(_("Break-even-point: %g"), prof_beg);
	} else {
		gfloat prof_beg;

		prof_beg = (-val->f + val->a + sqrt(discr)) / (2. * val->e);
		text = g_strdup_printf(_("Break-even-point: %g"), prof_beg);
	}

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 5, 6);

	if (discr > 0.) {
		gfloat prof_end;

		prof_end = (-val->f + val->a - sqrt(discr)) / (2. * val->e);
		text = g_strdup_printf(_("End: %g"), prof_end);
	} else
		text = g_strdup_printf(_("End: -"));

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 5, 6);

	return table;
}

GtkWidget *create_table_mono_2(appl_t * appl)
{
	GtkWidget *table, *label;
	gchar *text;
	values_t *val = &(appl->values);
	gfloat amount, discr;

	/* create a table for the texts */
	table = gtk_table_new(6, 3, FALSE);
	gtk_table_set_col_spacings(GTK_TABLE(table), COL_SPACINGS);
	gtk_table_set_row_spacings(GTK_TABLE(table), ROW_SPACINGS);

	/* Cost functions texts */
	label = gtk_label_new(_("Cost functions"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 0, 1);

	/* C(x) text */
	text =
	    g_strdup_printf(_
			    ("<span color=\"#15a40a\">C(x) = %gx² %+gx %+g</span>"),
			    val->a, val->b, val->c);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 0, 1);

	/* C'(x) text */
	text =
	    g_strdup_printf(_
			    ("<span color=\"#15a40a\">C\'(x) = %gx %+g</span>"),
			    2. * val->a, val->b);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 0, 1);

	/* Price-sales-function text */
	label = gtk_label_new(_("Price-sales-function"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 1, 2);

	text =
	    g_strdup_printf(_("<span color=\"#e71234\">p(x) = %gx %+g</span>"),
			    val->e, val->f);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 3, 1, 2);

	/* Revenue functions texts */
	label = gtk_label_new(_("Revenue functions"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 2, 3);

	/* R(x) text */
	text =
	    g_strdup_printf(_
			    ("<span color=\"#2304ee\">R(x) = %gx² %+gx</span>"),
			    val->e, val->f);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 2, 3);

	/* R'(x) text */
	text =
	    g_strdup_printf(_
			    ("<span color=\"#2304ee\">R\'(x) = %gx %+g</span>"),
			    2. * val->e, val->f);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 2, 3);

	/* Maximum profit texts */
	label = gtk_label_new(_("Maximum profit"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 3, 4);

	/* initialize quantity */
	val->quantity = (val->b - val->f) / (2. * (val->e - val->a));

	if (val->quantity > 0.) {
		text = g_strdup_printf(_("Quantity: %g"), val->quantity);

		val->cournot_points = 1;
	} else
		text = g_strdup_printf(_("Quantity: -"));

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 3, 4);

	if (val->cournot_points != 0) {
		val->price = val->e * val->quantity + val->f;

		text = g_strdup_printf(_("Price: %g"), val->price);
	} else
		text = g_strdup_printf(_("Price: -"));

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 3, 4);

	if (val->cournot_points != 0) {
		amount =
		    val->quantity * (val->quantity * (val->e - val->a) +
				     val->f - val->b) - val->c;

		text = g_strdup_printf(_("Amount: %g"), amount);
	} else
		text = g_strdup_printf(_("Amount: -"));

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 3, 4, 5);

	/* Profit range texts */
	label = gtk_label_new(_("Profit range"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 5, 6);

	discr = SQR(val->f - val->b) + 4. * (val->e - val->a) * val->c;
	if (discr < 0.)
		text = g_strdup_printf(_("Break-even-point: -"));
	else if (discr == 0.) {
		gfloat prof_beg;

		prof_beg = (-val->f + val->b) / (2. * (val->e - val->a));
		text = g_strdup_printf(_("Break-even-point: %g"), prof_beg);
	} else {
		gfloat prof_beg;

		prof_beg =
		    (-val->f + val->b + sqrt(discr)) / (2. * (val->e - val->a));
		text = g_strdup_printf(_("Break-even-point: %g"), prof_beg);
	}

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 5, 6);

	if (discr > 0.) {
		gfloat prof_end;

		prof_end =
		    (-val->f + val->b - sqrt(discr)) / (2. * (val->e - val->a));
		text = g_strdup_printf(_("End: %g"), prof_end);
	} else
		text = g_strdup_printf(_("End: -"));

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 5, 6);

	return table;
}

GtkWidget *create_table_mono_3(appl_t * appl)
{
	GtkWidget *table, *label;
	gchar *text;
	values_t *val = &(appl->values);
	gfloat discr, amount, p, q;

	/* create a table for the texts */
	table = gtk_table_new(6, 3, FALSE);
	gtk_table_set_col_spacings(GTK_TABLE(table), COL_SPACINGS);
	gtk_table_set_row_spacings(GTK_TABLE(table), ROW_SPACINGS);

	/* Cost functions texts */
	label = gtk_label_new(_("Cost functions"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 0, 1);

	/* C(x) text */
	text =
	    g_strdup_printf(_
			    ("<span color=\"#15a40a\">C(x) = %gx³ %+gx² %+gx %+g</span>"),
			    val->a, val->b, val->c, val->d);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 0, 1);

	/* C'(x) text */
	text =
	    g_strdup_printf(_
			    ("<span color=\"#15a40a\">C\'(x) = %gx² %+gx %+g</span>"),
			    3. * val->a, 2. * val->b, val->c);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 0, 1);

	/* Price-sales-function text */
	label = gtk_label_new(_("Price-sales-function"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 1, 2);

	text =
	    g_strdup_printf(_("<span color=\"#e71234\">p(x) = %gx %+g</span>"),
			    val->e, val->f);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 3, 1, 2);

	/* Revenue functions texts */
	label = gtk_label_new(_("Revenue functions"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 2, 3);

	/* R(x) text */
	text =
	    g_strdup_printf(_
			    ("<span color=\"#2304ee\">R(x) = %gx² %+gx</span>"),
			    val->e, val->f);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 2, 3);

	/* R'(x) text */
	text =
	    g_strdup_printf(_
			    ("<span color=\"#2304ee\">R\'(x) = %gx %+g</span>"),
			    2. * val->e, val->f);
	label = gtk_label_new(text);
	g_free(text);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 2, 3);

	/* Maximum profit texts */
	label = gtk_label_new(_("Maximum profit"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 3, 4);

	/* discriminant: term under the square root sign in the quadratic formula */
	discr =
	    4. * (pow((val->e - val->b), 2.) + 3. * val->a * (val->f - val->c));
	if (discr < 0.)
		text = g_strdup_printf(_("Quantity: -"));
	else if (discr == 0.) {
		/* initialize quantity */
		val->quantity = (val->e - val->b) / (3. * val->a);

		if (val->quantity > 0.) {
			text =
			    g_strdup_printf(_("Quantity: %g"), val->quantity);

			val->cournot_points = 1;
		} else
			text = g_strdup_printf(_("Quantity: -"));
	} else {
		gfloat quantity_2;

		/* initialize quantities */
		val->quantity =
		    (2. * (val->e - val->b) + sqrt(discr)) / (6. * val->a);
		quantity_2 =
		    (2. * (val->e - val->b) - sqrt(discr)) / (6. * val->a);

		val->quantity = MAX(val->quantity, quantity_2);

		if (val->quantity > 0.) {
			text =
			    g_strdup_printf(_("Quantity: %g"), val->quantity);

			val->cournot_points = 1;
		} else
			text = g_strdup_printf(_("Quantity: -"));
	}

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 3, 4);

	if (val->cournot_points != 0) {
		val->price = val->e * val->quantity + val->f;

		text = g_strdup_printf(_("Price: %g"), val->price);
	} else
		text = g_strdup_printf(_("Price: -"));

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 3, 4);

	if (val->cournot_points != 0) {
		amount =
		    -val->quantity * (val->quantity *
				      (val->a * val->quantity + val->b -
				       val->e) + val->c - val->f) - val->d;

		text = g_strdup_printf(_("Amount: %g"), amount);
	} else
		text = g_strdup_printf(_("Amount: -"));

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 3, 4, 5);

	/* Profit range texts */
	label = gtk_label_new(_("Profit range"));
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 5, 6);

	p = (3. * val->a * (val->c - val->f) -
	     SQR(val->b - val->e)) / (3. * SQR(val->a));
	q = (2. * THR(val->b - val->e)) / (27. * THR(val->a)) -
	    ((val->b - val->e) * (val->c - val->f)) / (3. * SQR(val->a)) +
	    val->d / val->a;
	discr = 4. * THR(p) + 27. * SQR(q);

	if (discr < 0.) {
		gfloat u1;

		u1 = -sqrt(-4. / 3. * p) * cos(1. / 3. *
					       acos(-q / 2. *
						    sqrt(-27. / THR(p))) +
					       M_PI / 3.);
		u1 -= (val->b - val->e) / (3. * val->a);
		text = g_strdup_printf(_("Break-even-point: %g"), u1);
	} else
		text = g_strdup_printf(_("Break-even-point: -"));

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 5, 6);

	if (discr < 0.) {
		gfloat u2;

		u2 = sqrt(-4. / 3. * p) * cos(1. / 3. *
					      acos(-q / 2. *
						   sqrt(-27. / THR(p))));
		u2 -= (val->b - val->e) / (3. * val->a);
		text = g_strdup_printf(_("End: %g"), u2);
	} else
		text = g_strdup_printf(_("End: -"));

	label = gtk_label_new(text);
	g_free(text);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 5, 6);

	return table;
}
