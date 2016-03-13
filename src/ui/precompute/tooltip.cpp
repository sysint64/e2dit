/*
 * E2DIT - is a 2D Map Editor to create a levels for 2d games
 * Copyright (C) 2015 Kabylin Andrey <andrey@kabylin.ru>

 * This file is part of E2DIT-UI.

 * E2DIT-UI is free library: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.

 * E2DIT-UI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with E2DIT-UI. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Kabylin Andrey <andrey@kabylin.ru>
 */

#include "ui/tooltip.h"

void UITooltip::precompute() {

	precomputeText    (0, style);

	// Left align
	precomputeElement  (0, style+leftParam, "left");
	precomputeElement  (1, style+leftParam, "center");
	precomputeElement  (2, style+leftParam, "right");

	// Right align
	precomputeElement (3, style+rightParam, "left");
	precomputeElement (4, style+rightParam, "center");
	precomputeElement (5, style+rightParam, "right");

	precomputeIntArray (style+leftParam , "aria", leftAria , 4);
	precomputeIntArray (style+rightParam, "aria", rightAria, 4);

	// TODO: add up and bottom align

}
