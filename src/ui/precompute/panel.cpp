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

#include "ui/panel.h"

/**
 * User Interface Precompute Texture Atlas values
 */

void UIPanel::precompute() {

	/* ScrollHorizontal */

	precomputeElement (0, "scrollhorizontal", "left");
	precomputeElement (1, "scrollhorizontal", "center");
	precomputeElement (2, "scrollhorizontal", "right");

	/* ScrollVertical */

	precomputeElement (3, "scrollvertical", "left");
	precomputeElement (4, "scrollvertical", "center");
	precomputeElement (5, "scrollvertical", "right");

	/* Scroll Button */
	/* Leave */

	precomputeElement (6, "scrollbuttonleave", "left");
	precomputeElement (7, "scrollbuttonleave", "center");
	precomputeElement (8, "scrollbuttonleave", "right");

	/* Enter */

	precomputeElement (9 , "scrollbuttonenter", "left");
	precomputeElement (10, "scrollbuttonenter", "center");
	precomputeElement (11, "scrollbuttonenter", "right");

	/* Split */

	precomputeElement (12, "panel", "splitlight");
	precomputeElement (13, "panel", "splitdark");

	/* Expand arrow */

	precomputeElement (14, "panel", "arrowopen");
	precomputeElement (15, "panel", "arrowclose");

	/* Header Background */

	precomputeElement (16, "panel", "headerleave");
	precomputeElement (17, "panel", "headerenter");

	/* Calculate Sizes */

	precomputeColor4f ("panel" , "backgroundlight" , backgroundLight);
	precomputeColor4f ("panel" , "backgrounddark"  , backgroundDark);
	precomputeColor4f ("panel" , "backgroundaction", backgroundAction);
	precomputeColor3f ("panel" , "textcolor"       , textColor);

}
