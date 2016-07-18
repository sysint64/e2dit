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
 * Author: Kabylin Andrey <andrey@kabylin.ru
 */

#include "ui/checkbox.h"

#include "utility/renderer.h"
#include "utility/string.h"

void UICheckbox::render() {

	UIElement::render();
	updateAbsPos();

	/* Tables Indices */

	int n  = 0; int tn = enter ? 2 : 0;

	if (checked) n = enter ? 3 : 1;
	else         n = enter ? 2 : 0;

	renderElement (n, absLeft, absTop, iWidths[0], iHeights[0], boxElement.get());

	if (focused) {

		renderElement (4, absLeft+focusOffsets[0], absTop+focusOffsets[1], iWidths[0], iHeights[0], focusElement.get());

	}

	/* Render Text */

	if (autoSize)
		width = iWidths[0]+ftglGetTextWidth(manager->theme->font, caption)+textOffsets[n]+10;

	manager->atlasShader->unbind();
	renderText (manager->theme->font, &textColors[n], absLeft+iWidths[0]+textOffsets[tn], absTop+manager->theme->fontHeight+textOffsets[tn+1], caption);
	manager->atlasShader->bind();

}

void UICheckbox::mouseDown (int x, int y, int button) {

	if (enter) checked = !checked;

}
