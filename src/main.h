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

#ifndef __MAIN_H__
#define __MAIN_H__

#include <gtk/gtk.h>

/* radio buttons */
typedef struct {
	GtkWidget *rbutton_poly, *rbutton_mono, *rbutton_1, *rbutton_2,
	    *rbutton_3;
} rbuttons_t;

/* spin buttons */
typedef struct {
	GtkWidget *a, *b, *c, *d, *e, *f;
} sbuttons_t;

/* values */
typedef struct {
	gfloat a, b, c, d, e, f, quantity, price, length, factor,
	    *x_cost, *x_cost_, *x_costu, *x_costuv,
	    *y_cost, *y_cost_, *y_costu, *y_costuv,
	    *x_price_sales, *y_price_sales,
	    *x_revenue, *x_revenue_, *y_revenue, *y_revenue_;
	guint points_cost, points_cost_, points_costu, points_costuv,
	    points_revenue, cournot_points, type;
} values_t;

/* appl_t: summarizes all previous structs */
typedef struct {
	GtkWidget *darea_1, *darea_2;
	rbuttons_t rbuttons;
	sbuttons_t sbuttons;
	values_t values;
} appl_t;

#endif				/* __MAIN_H__ */
