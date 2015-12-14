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

void UIDialog::render() {

	updateAbsPos();
	renderPartsElementBlock (indices, drawElementsPtr, absLeft-iWidths[0], absTop-iHeights[1], width+(iWidths[0] << 1), height+iHeights[1]+iHeights[7]);
	UIPanel::render();

	/* Draw Text */

	manager->atlasShader->unbind();
	renderText (manager->theme->font, textColor, absLeft, absTop-10, caption);
	manager->atlasShader->bind();

	if (headerClick) {

		left = lastLeft+app->mouseX-app->clickX;
		top  = lastTop +app->mouseY-app->clickY;

	}

}

void UIDialog::mouseDown (int x, int y, int button) {

	UIElement::mouseDown (x, y, button);
	headerClick = pointInRect (x, y, absLeft, absTop-iHeights[1], width, iHeights[1]);

	lastLeft = left;
	lastTop  = top;

}

void UIDialog::mouseUp (int x, int y, int button) {

	headerClick = false;

}

void UIDialog::show() {

	visible = true;
	layer   = manager->currentLayer+1;

	manager->currentLayer++;

}

void UIDialog::hide() {

	visible = false;
	manager->currentLayer--;

}
