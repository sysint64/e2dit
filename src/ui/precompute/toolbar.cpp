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

#include "ui/toolbar.h"
#include "utility/renderer.h"

/**
 * User Interface Precompute Texture Atlas values
 */

void UIToolbar::precompute() {

	precomputeElement (0, style, "background");
	precomputeElement (1, style, "split");

	tabOffset    = precomputeInt (style, "taboffset");
	shadowHeight = precomputeInt (style, "shadowheight");

	height = iHeights[0]-shadowHeight+1;

}

void UIToolbarItem::precompute() {

	precomputeText (0, style+leaveElement);
	precomputeText (1, style+enterElement);
	precomputeText (2, style+clickElement);

	alphas[0] = static_cast<float> (precomputeInt (style+leaveElement, "alpha")) / 100.f;
	alphas[1] = static_cast<float> (precomputeInt (style+enterElement, "alpha")) / 100.f;
	alphas[2] = static_cast<float> (precomputeInt (style+clickElement, "alpha")) / 100.f;

}
