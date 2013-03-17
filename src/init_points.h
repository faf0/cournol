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

#ifndef __INIT_POINTS_H__
#define __INIT_POINTS_H__

#include "main.h"

/* number of points to draw
 * ASSERT POINTS >= 3 and uneven
 */
#undef POINTS
#define POINTS 61

void init_points_1(appl_t * appl);
void init_points_2(appl_t * appl);
void init_points_3(appl_t * appl);

#endif				/* __INIT_POINTS_H__ */
