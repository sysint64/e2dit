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

void UIButton::precomputeElement (const int n, std::string element, std::string params) {

	offsetsX[n] = manager->theme->element[element].params[params][0].num / manager->theme->skin->width;
	offsetsY[n] = manager->theme->element[element].params[params][1].num / manager->theme->skin->height;
	fWidths [n] = manager->theme->element[element].params[params][2].num / manager->theme->skin->width;
	fHeights[n] = manager->theme->element[element].params[params][3].num / manager->theme->skin->height;

	iWidths [n] = floor (manager->theme->element[element].params[params][2].num);
	iHeights[n] = floor (manager->theme->element[element].params[params][3].num);

}

/**
 *
 */

void UIButton::precomputeText (const int n, std::string element) {

	int tcs = n*3; /* Text Color  Index Start */
	int tos = n*2; /* Text Offset Index Start */

	/* Text Color */

	textColors [tcs+0] = manager->theme->element[element].params["textcolor"][0].num/255.f;
	textColors [tcs+1] = manager->theme->element[element].params["textcolor"][1].num/255.f;
	textColors [tcs+2] = manager->theme->element[element].params["textcolor"][2].num/255.f;

	/* Text Offset */

	textOffsets[tos+0] = floor (manager->theme->element[element].params["textoffset"][0].num);
	textOffsets[tos+1] = floor (manager->theme->element[element].params["textoffset"][1].num);

}

/**
 *
 */

void UIButton::precompute() {

	/* Leave */

	precomputeElement (0, leaveElement, "left");
	precomputeElement (1, leaveElement, "center");
	precomputeElement (2, leaveElement, "right");
	precomputeText    (0, leaveElement);

	/* Enter */

	precomputeElement (3, enterElement, "left");
	precomputeElement (4, enterElement, "center");
	precomputeElement (5, enterElement, "right");
	precomputeText    (1, enterElement);

	/* Enter */

	precomputeElement (6, clickElement, "left");
	precomputeElement (7, clickElement, "center");
	precomputeElement (8, clickElement, "right");
	precomputeText    (2, clickElement);

	/* Calculate Size */

	height = iHeights[1];

	if (drawAlign == Align::Left || drawAlign == Align::Right)
		width += iWidths[0];

}
