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

#include "init_points.h"
#include "main.h"
#include "output.h"

#ifndef SQR
#define SQR(a) ((a) * (a))
#endif

static void init_points_poly(appl_t * appl);
static void init_points_mono(appl_t * appl);

void init_points_1(appl_t * appl)
{
	values_t *val = &(appl->values);

	switch (val->type) {
	case 1:
	case 2:
	case 3:
		init_points_poly(appl);
		break;

	case 4:
	case 5:
	case 6:
	default:
		init_points_mono(appl);
		break;
	}

	/* C(x) */
	val->points_cost = 2;

	val->x_cost = g_new(gfloat, 2);
	val->x_cost[0] = 0.;
	val->x_cost[1] = val->length;

	val->y_cost = g_new(gfloat, 2);
	val->y_cost[0] = val->b;
	val->y_cost[1] = val->a * val->length + val->b;

	/* C'(x) */
	val->points_cost_ = 2;

	val->x_cost_ = val->x_cost;

	val->y_cost_ = g_new(gfloat, 2);
	val->y_cost_[0] = val->a;
	val->y_cost_[1] = val->a;

	if (val->type <= 3) {
		guint i;

		/* c(x) */
		val->points_costu = POINTS;

		val->x_costu = g_new(gfloat, POINTS);
		val->y_costu = g_new(gfloat, POINTS);

		val->factor = (gfloat) val->length / POINTS;

		for (i = 0; i < POINTS; i++) {
			val->x_costu[i] = (gfloat) val->factor * (i + 1);
			val->y_costu[i] = val->a + val->b / val->x_costu[i];
		}

		/* cv(x) */
		val->points_costuv = 2;

		val->x_costuv = val->x_cost_;
		val->y_costuv = val->y_cost_;
	}

	output(appl);
}

void init_points_2(appl_t * appl)
{
	values_t *val = &(appl->values);
	guint i;

	switch (val->type) {
	case 1:
	case 2:
	case 3:
		init_points_poly(appl);
		break;

	case 4:
	case 5:
	case 6:
		init_points_mono(appl);
		break;
	}

	/* C(x) */
	val->points_cost = POINTS;

	val->x_cost = g_new(gfloat, POINTS);
	val->y_cost = g_new(gfloat, POINTS);

	for (i = 0; i < POINTS; i++) {
		val->x_cost[i] = (gfloat) i *val->factor;
		val->y_cost[i] =
		    val->x_cost[i] * (val->a * val->x_cost[i] + val->b) +
		    val->c;
	}

	/* C'(x) */
	val->points_cost_ = 2;

	val->x_cost_ = g_new(gfloat, 2);
	val->x_cost_[0] = 0.;
	val->x_cost_[1] = val->length;

	val->y_cost_ = g_new(gfloat, 2);
	val->y_cost_[0] = val->b;
	val->y_cost_[1] = 2. * val->a * val->length + val->b;

	if (val->type <= 3) {
		/* c(x) */
		val->points_costu = POINTS;

		val->x_costu = g_new(gfloat, POINTS);
		val->y_costu = g_new(gfloat, POINTS);

		val->factor = (gfloat) val->length / POINTS;

		for (i = 0; i < POINTS; i++) {
			val->x_costu[i] = (gfloat) val->factor * (i + 1);
			val->y_costu[i] =
			    val->a * val->x_costu[i] + val->b +
			    val->c / val->x_costu[i];
		}

		/* cv(x) */
		val->points_costuv = 2;

		val->x_costuv = val->x_cost_;

		val->y_costuv = g_new(gfloat, 2);
		val->y_costuv[0] = val->b;
		val->y_costuv[1] = val->a * val->length + val->b;
	}

	output(appl);
}

void init_points_3(appl_t * appl)
{
	values_t *val = &(appl->values);
	guint i;

	switch (val->type) {
	case 1:
	case 2:
	case 3:
		init_points_poly(appl);
		break;

	case 4:
	case 5:
	case 6:
	default:
		init_points_mono(appl);
		break;
	}

	/* C(x) */
	val->points_cost = POINTS;

	val->x_cost = g_new(gfloat, POINTS);
	val->y_cost = g_new(gfloat, POINTS);

	for (i = 0; i < POINTS; i++) {
		val->x_cost[i] = (gfloat) i *val->factor;
		val->y_cost[i] =
		    val->x_cost[i] * (val->x_cost[i] *
				      (val->a * val->x_cost[i] + val->b) +
				      val->c) + val->d;
	}

	/* C'(x) */
	val->points_cost_ = POINTS;

	val->x_cost_ = val->x_cost;
	val->y_cost_ = g_new(gfloat, POINTS);

	for (i = 0; i < POINTS; i++)
		val->y_cost_[i] =
		    val->x_cost_[i] * (3. * val->a * val->x_cost_[i] +
				       2. * val->b) + val->c;

	if (val->type <= 3) {
		/* c(x) */
		val->points_costu = POINTS;

		val->x_costu = g_new(gfloat, POINTS);
		val->y_costu = g_new(gfloat, POINTS);

		val->factor = (gfloat) val->length / POINTS;

		for (i = 0; i < POINTS; i++) {
			val->x_costu[i] = (gfloat) val->factor * (i + 1);
			val->y_costu[i] =
			    val->x_costu[i] * (val->a * val->x_costu[i] +
					       val->b) + val->c +
			    val->d / val->x_costu[i];
		}

		/* cv(x) */
		val->points_costuv = POINTS;

		val->x_costuv = val->x_cost;

		val->y_costuv = g_new(gfloat, POINTS);

		for (i = 0; i < POINTS; i++)
			val->y_costuv[i] =
			    val->x_costuv[i] * (val->a * val->x_costuv[i] +
						val->b) + val->c;
	}

	output(appl);
}

static void init_points_poly(appl_t * appl)
{
	values_t *val = &(appl->values);

	/* length of x-axis: maximum capacity */
	val->length = val->f;
	val->factor = (gfloat) val->length / (POINTS - 1);

	/* R(x) */
	val->points_revenue = 2;

	val->x_revenue = g_new(gfloat, 2);
	val->x_revenue[0] = 0.;
	val->x_revenue[1] = val->length;

	val->y_revenue = g_new(gfloat, 2);
	val->y_revenue[0] = 0.;
	val->y_revenue[1] = val->length * val->e;

	/* R'(x): market price */
	val->x_revenue_ = val->x_revenue;

	val->y_revenue_ = g_new(gfloat, 2);
	val->y_revenue_[0] = val->e;
	val->y_revenue_[1] = val->e;
}

static void init_points_mono(appl_t * appl)
{
	values_t *val = &(appl->values);
	guint i;

	/* length of x-axis: (p(x) = 0) */
	val->length = -val->f / val->e;
	val->factor = (gfloat) val->length / (POINTS - 1);

	/* p(x) */
	val->x_price_sales = g_new(gfloat, 2);
	val->x_price_sales[0] = 0.;
	val->x_price_sales[1] = val->length;

	val->y_price_sales = g_new(gfloat, 2);
	val->y_price_sales[0] = val->f;
	val->y_price_sales[1] = 0.;

	/* R(x) */
	val->points_revenue = POINTS;

	val->x_revenue = g_new(gfloat, POINTS);
	val->y_revenue = g_new(gfloat, POINTS);

	for (i = 0; i < POINTS; i++) {
		val->x_revenue[i] = (gfloat) i *val->factor;
		val->y_revenue[i] =
		    val->x_revenue[i] * (val->e * val->x_revenue[i] + val->f);
	}

	/* R'(x) */
	val->x_revenue_ = g_new(gfloat, 2);
	val->x_revenue_[0] = 0.;
	val->x_revenue_[1] = val->length / 2.;

	val->y_revenue_ = g_new(gfloat, 2);
	val->y_revenue_[0] = val->f;
	val->y_revenue_[1] = 0.;
}
