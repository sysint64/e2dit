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
#include <boost/range/adaptor/reversed.hpp>

void ui::UIToolbar::render() {

	updateAlign();
	updateAbsPos();

	int taby = -21+2;
	int tabx = tabsOffset;

	for (const auto &kvp : elements)
		tabx += kvp.second.get()->width-tabOffset;

	for (const auto &kvp : boost::adaptors::reverse(elements)) {

		UIElement *tab = kvp.second.get();

		tabx -= tab->width-tabOffset;

		tab->left = tabx;
		tab->top  = taby;

		if (tab->checked)
			continue;

		tab->over = false;
		tab->render();

	}

	renderElement (0, absLeft, absTop, width, iHeights[0], bgElement.get());
	over = pointInRect (app->mouseX, app->mouseY, absLeft, absTop-22, width, iHeights[0]+22);

	if (lastSelected == nullptr)
		return;

	lastSelected->render();
	lastSelected->over = over;

	//  Render Active tab elements
	// TODO: Remove hardcode

	int elx = -tabsOffset+10;
	int ely = 27;

	for (const auto &kvp : lastSelected->elements) {

		UIElement *el = kvp.second.get();

		el->left = elx;
		el->top  = ely;

		el->render();
		elx += el->width+10;

	}

}

void ui::UIToolbarItem::render() {

	int ai = enter ? 1 : (checked ? 2 : 0);
	int ci = enter ? 3 : (checked ? 6 : 0);

	glUniform1f (manager->atlasShader->locations["Alpha"], alphas[ai]);

	updateAbsPos();
	manager->toolIcons->render (absLeft, app->windowHeight-manager->toolIcons->sizeIcon-absTop,
	                            iconOffset[0], iconOffset[1], iconElement.get());

	int twidth = ftglGetTextWidth (manager->theme->font, caption);
	int tx = absLeft+((width-twidth) >> 1);

	manager->atlasShader->unbind();
	renderText (manager->theme->font, &(textColors[ci]), tx, absTop+manager->toolIcons->sizeIcon+14, caption);
	manager->atlasShader->bind();

	glUniform1f (manager->atlasShader->locations["Alpha"], 1.0f);

}

void ui::UIToolbarItem::mouseDown (int x, int y, int button) {

	if (enter)
		toolbar->checkTool (this);

}

void ui::UIToolbarSplit::render() {

	updateAbsPos();
	renderElement (0, absLeft+5, absTop-1, iWidths[0], iHeights[0], splitElement.get());

}

void ui::UIToolbar::updateAlign() {

	if (align == Align::None)
		return;

	int x0 = 0; int mWidth  = 0; int mLeft = 0;
	int y0 = 0; int mHeight = 0; int mTop  = 0;

	/* TODO: Incapsulate find border */
	/* Find Border */

	for (const auto &kvp : parent->elements) {

		UIElement *el = kvp.second.get();

		if (el == this)
			break;

		if (!el->visible || el->align == Align::None)
			continue;

		switch (el->align) {

			case Align::Top    : y0 += el->height; mTop  += el->height; break;
			case Align::Left   : x0 += el->width ; mLeft += el->width;  break;

			case Align::Bottom : mHeight += el->height; break;
			case Align::Right  : mWidth  += el->width;  break;

			default: continue;

		}

	}

	/* Update Position and Size depending on the align */

	switch (align) {

		case Align::Top:

			width = parent->width-mWidth-x0-offsetSizeX();

			left = x0;
			top  = y0;

			break;

		default: return;

	}

}
