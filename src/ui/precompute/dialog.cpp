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

#include "ui/dialog.h"

void ui::UIDialog::precompute() {

	/* Top */

	precomputeElement (0, style+"top", "left");
	precomputeElement (1, style+"top", "center");
	precomputeElement (2, style+"top", "right");

	/* Middle */

	precomputeElement (3, style+"middle", "left");
	precomputeElement (4, style+"middle", "center");
	precomputeElement (5, style+"middle", "right");

	/* Bottom */

	precomputeElement (6, style+"bottom", "left");
	precomputeElement (7, style+"bottom", "center");
	precomputeElement (8, style+"bottom", "right");

	precomputeText    (0, style);
	precomputeIntArray(style, "captionarea", captionArea, 4);

	/* Buttons */

	precomputeElement (9 , style+".maximizebuttonleave", "area");
	precomputeElement (10, style+".maximizebuttonenter", "area");

	precomputeElement (11, style+".closebuttonleave"   , "area");
	precomputeElement (12, style+".closebuttonenter"   , "area");

	opacity[0]      = precomputeFloat (style+".maximizebuttonleave", "opacity");
	opacity[1]      = precomputeFloat (style+".maximizebuttonenter", "opacity");

	opacity[2]      = precomputeFloat (style+".closebuttonleave"   , "opacity");
	opacity[3]      = precomputeFloat (style+".closebuttonenter"   , "opacity");

	buttonsTop[0]   = precomputeFloat (style+".maximizebutton"     , "top");
	buttonsTop[1]   = precomputeFloat (style+".closebutton"        , "top");

	buttonsRight[0] = precomputeFloat (style+".maximizebutton"     , "right");
	buttonsRight[1] = precomputeFloat (style+".closebutton"        , "right");

	precomputeText (0, style);

}
