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

void UIDialog::setCursor() {
	if (maximized || !visible)
		return;

	switch (edgeEnter) {
		case Edge::TopLeftCorner     : manager->cursor = CursorIco::TopLeftCorner;     break;
		case Edge::TopRightCorner    : manager->cursor = CursorIco::TopRightCorner;    break;
		case Edge::BottomLeftCorner  : manager->cursor = CursorIco::BottomLeftCorner;  break;
		case Edge::BottomRightCorner : manager->cursor = CursorIco::BottomRightCorner; break;
		case Edge::TopSide           : manager->cursor = CursorIco::TopSide;           break;
		case Edge::BottomSide        : manager->cursor = CursorIco::BottomSide;        break;
		case Edge::LeftSide          : manager->cursor = CursorIco::LeftSide;          break;
		case Edge::RightSide         : manager->cursor = CursorIco::RightSide;         break;
	}

}

void UIDialog::render() {

	if (headerClick && !edgeClick && !btnDown) {

		left = lastLeft+app->mouseX-app->clickX;
		top  = lastTop +app->mouseY-app->clickY;

	}

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

	renderButtons(x, y);
	handleResize();
	handleEdgeEnter();

}

void UIDialog::handleResize() {

	if (!allowResize || !edgeClick)
		return;

	switch (edgeEnter) {

		// Corners
		case Edge::TopLeftCorner:
			width  = lastWidth -app->mouseX+app->clickX;
			height = lastHeight-app->mouseY+app->clickY;
			break;

		case Edge::TopRightCorner:
			width  = lastWidth +app->mouseX-app->clickX;
			height = lastHeight-app->mouseY+app->clickY;
			break;

		case Edge::BottomLeftCorner:
			width  = lastWidth -app->mouseX+app->clickX;
			height = lastHeight+app->mouseY-app->clickY;
			break;

		case Edge::BottomRightCorner:
			width  = lastWidth +app->mouseX-app->clickX;
			height = lastHeight+app->mouseY-app->clickY;
			break;

		// Sides
		case Edge::TopSide:
			height = lastHeight-app->mouseY+app->clickY;
			break;

		case Edge::BottomSide:
			height = lastHeight+app->mouseY-app->clickY;
			break;

		case Edge::LeftSide:
			width = lastWidth -app->mouseX+app->clickX;
			break;

		case Edge::RightSide:
			width = lastWidth+app->mouseX-app->clickX;
			break;

	}

	math::clamp (&width , minWidth , maxWidth );
	math::clamp (&height, minHeight, maxHeight);

	if (edgeEnter == Edge::TopLeftCorner || edgeEnter == Edge::BottomLeftCorner || edgeEnter == Edge::LeftSide)
		left = lastLeft-(width -lastWidth);

	if (edgeEnter == Edge::TopLeftCorner || edgeEnter == Edge::TopRightCorner || edgeEnter == Edge::TopSide)
		top = lastTop -(height-lastHeight);

}

void UIDialog::handleEdgeEnter() {

	if (!allowResize || edgeClick)
		return;

	const int uiEdgeWidth = 5;  const int ew = edgeWidth;
	const int mx = app->mouseX; const int my = app->mouseY;
	const int w  = width;       const int h  = height;

	bool enter = false;
	edgeEnter = Edge::None;

	const int left   = absLeft-edgeWidth-uiEdgeWidth;
	const int right  = absLeft+width+uiEdgeWidth;
	const int top    = absTop-edgeWidth-captionArea[1];
	const int bottom = absTop+uiEdgeWidth+height;

	const int aleft  = absLeft;
	const int atop   = absTop-captionArea[1];

	// Corners
	enter = pointInRect (mx, my, left , top   , ew, ew);  if (enter) { edgeEnter = Edge::TopLeftCorner;     return; }
	enter = pointInRect (mx, my, left , bottom, ew, ew);  if (enter) { edgeEnter = Edge::BottomLeftCorner;  return; }
	enter = pointInRect (mx, my, right, top   , ew, ew);  if (enter) { edgeEnter = Edge::TopRightCorner;    return; }
	enter = pointInRect (mx, my, right, bottom, ew, ew);  if (enter) { edgeEnter = Edge::BottomRightCorner; return; }

	// Sides
	enter = pointInRect (mx, my, aleft, top   , w , ew);  if (enter) { edgeEnter = Edge::TopSide;           return; }
	enter = pointInRect (mx, my, aleft, bottom, w , ew);  if (enter) { edgeEnter = Edge::BottomSide;        return; }
	enter = pointInRect (mx, my, left , atop  , ew, h );  if (enter) { edgeEnter = Edge::LeftSide;          return; }
	enter = pointInRect (mx, my, right, atop  , ew, h );  if (enter) { edgeEnter = Edge::RightSide;         return; }

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
	edgeClick   = edgeEnter != Edge::None;

	if (!maximized) {
		lastLeft   = left;
		lastTop    = top;
		lastWidth  = width;
		lastHeight = height;
	}

	if (maximizeEnter || closeEnter)
		btnDown = true;

}

void UIDialog::mouseUp (int x, int y, int button) {

	headerClick = false;
	edgeClick   = false;
	btnDown     = false;

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
