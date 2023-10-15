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

#include "init_values.h"
#include "main.h"
#include "input.h"

static void init_price(appl_t * appl);
static void init_price_sales(appl_t * appl);

void init_values_poly_1(appl_t * appl)
{
	values_t *val = &(appl->values);

	val->a = 300.;
	val->b = 1000.;

	init_price(appl);
	input_1(appl);
}

void init_values_poly_2(appl_t * appl)
{
	values_t *val = &(appl->values);

	val->a = 16.;
	val->b = 32.;
	val->c = 1000.;

	init_price(appl);
	input_2(appl);
}

void init_values_poly_3(appl_t * appl)
{
	values_t *val = &(appl->values);

	val->a = 1.;
	val->b = -10.;
	val->c = 200.;
	val->d = 1000.;

	init_price(appl);
	input_3(appl);
}

void init_values_mono_1(appl_t * appl)
{
	values_t *val = &(appl->values);

	val->a = 300.;
	val->b = 1000.;

	init_price_sales(appl);
	input_1(appl);
}

void init_values_mono_2(appl_t * appl)
{
	values_t *val = &(appl->values);

	val->a = 16.;
	val->b = 32.;
	val->c = 1000.;

	init_price_sales(appl);
	input_2(appl);
}

void init_values_mono_3(appl_t * appl)
{
	values_t *val = &(appl->values);

	val->a = 1.;
	val->b = -10.;
	val->c = 200.;
	val->d = 1000.;

	init_price_sales(appl);
	input_3(appl);
}

static void init_price(appl_t * appl)
{
	values_t *val = &(appl->values);

	val->e = 550.;
	val->f = 35.;

	val->cournot_points = 0;
}

static void init_price_sales(appl_t * appl)
{
	values_t *val = &(appl->values);

	val->e = -50.;
	val->f = 1000.;

	val->cournot_points = 0;
}
