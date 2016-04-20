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

	align = Align::None;
	updateAbsPos();

	int xOffset = abs(captionArea[0]-iOffsetsX[0]);
	int yOffset = abs(captionArea[1]-iOffsetsY[1]);

	int x = absLeft+xOffset-iWidths [0];
	int y = absTop +yOffset-iHeights[1];

	if (maximized) {

		align = Align::Client;
		UIPanel::render();
		renderButtons (x, y);

		return;

	}

	renderPartsElementBlock (indices, drawElementsPtr, absLeft-iWidths[0], absTop-iHeights[1], width+(iWidths[0] << 1), height+iHeights[1]+iHeights[7]);
	UIPanel::render();

	/* Draw Text */

	int tx = x+textOffsets[0];
	int ty = y+textOffsets[1]+(captionArea[3] >> 1);

	manager->atlasShader->unbind();
	renderText (manager->theme->font, &(textColors[0]), tx, ty, caption);
	manager->atlasShader->bind();

	if (headerClick) {

		left = lastLeft+app->mouseX-app->clickX;
		top  = lastTop +app->mouseY-app->clickY;

	}

	renderButtons(x, y);
}

void UIDialog::renderButtons (int x, int y) {

	/* Draw buttons */
	// Close button

	int n  = closeEnter ? 12 : 11;
	int bx = absLeft+width-iWidths[n]-buttonsRight[1];
	int by = y+buttonsTop[1];

	closeEnter = pointInRect (app->mouseX, app->mouseY, bx, by, iWidths[n], iHeights[n]);
	glUniform1f (manager->atlasShader->locations["Alpha"], opacity[n-11]);
	renderElement (n, bx, by, iWidths[n], iHeights[n], drawElementsPtr[n]);

	// Maximize button

	if (!showMaximizeButton)
		return;

	n  = maximizeEnter ? 10 : 9;
	bx = absLeft+width-iWidths[n]-buttonsRight[0];
	by = y+buttonsTop[0];

	maximizeEnter = pointInRect (app->mouseX, app->mouseY, bx, by, iWidths[n], iHeights[n]);
	glUniform1f (manager->atlasShader->locations["Alpha"], opacity[n-9]);
	renderElement (n, bx, by, iWidths[n], iHeights[n], drawElementsPtr[n]);

}

void UIDialog::mouseDown (int x, int y, int button) {

	UIElement::mouseDown (x, y, button);

	int yOffset = abs(captionArea[1]-iOffsetsY[1]);
	int hy = absTop+yOffset-iHeights[1];

	headerClick = pointInRect (x, y, absLeft, hy, width, captionArea[3]);

	if (!maximized) {
		lastLeft = left;
		lastTop  = top;
	}

}

void UIDialog::mouseUp (int x, int y, int button) {

	headerClick = false;

	if (showMaximizeButton && maximizeEnter) {
		maximized = !maximized;

		if (maximized) {
			lastWidth = width; lastHeight = height;
			lastLeft  = left;  lastTop    = top;
		} else {
			width = lastWidth; height = lastHeight;
			left  = lastLeft;  top    = lastTop;
		}
	}

	if (closeEnter) {
		hide();
	}

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
