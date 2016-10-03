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

#include "ui/button.h"

/**
 *
 */

void ui::Button::precompute() {

	/* Leave */

	precomputeElement (0, style+leaveElement, "left");
	precomputeElement (1, style+leaveElement, "center");
	precomputeElement (2, style+leaveElement, "right");

	/* Enter */

	precomputeElement (3, style+enterElement, "left");
	precomputeElement (4, style+enterElement, "center");
	precomputeElement (5, style+enterElement, "right");

	/* Enter */

	precomputeElement (6, style+clickElement, "left");
	precomputeElement (7, style+clickElement, "center");
	precomputeElement (8, style+clickElement, "right");

	/* Focused */

	precomputeElement (9 , style+focusElement, "left");
	precomputeElement (10, style+focusElement, "center");
	precomputeElement (11, style+focusElement, "right");

	/* Focus Offset */

	precomputeIntArray (style+focusElement, "offsets", focusOffsets, 3);

	/* Text */

	precomputeText    (0, style+leaveElement);
	precomputeText    (1, style+enterElement);
	precomputeText    (2, style+clickElement);

	/* Calculate Size */

	height = iHeights[1];

	if (drawAlign == Align::Left || drawAlign == Align::Right)
		width += iWidths[0];

}
