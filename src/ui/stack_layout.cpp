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

#include "ui/stack_layout.h"

/**
 * \brief Add row to stack layout and put element into row
 *
 * +-------------+    Row stretch to parent by width or height
 * |   ROW/COL   |    In depending of orientation
 * | +---------+ |
 * | | ELEMENT | |
 * | +---------+ |
 * +-------------+
 */

void UIStackLayout::addElement (std::unique_ptr<UIElement> el) {

	if (!parent)
		Application::getInstance()->log.ewrite ("Can't add element, element don't has parent!");

	std::unique_ptr<UIElement> row = std::make_unique<UIElement>(manager);
	auto _row = row.get();

	UIElement::addElement (std::move(row));
	_row->addElement (std::move(el));

}

/**
 * \brief Render and recalculate location and sizes of elements
 */

void UIStackLayout::render() {

	updateAbsPos();

	int lastLoc   = 0;
	int maxWidth  = 0;
	int maxHeight = 0;

	for (const auto &kvp : elements) {

		UIElement *el = kvp.second.get();
		UIElement *child = (*(el->elements.begin())).second.get(); // Get first element

		if (orientation == Orientation::Vertical) {

			int h = child->height+child->marginTop+child->marginBottom;

			el->width  = parent->width-parent->paddingRight-parent->paddingLeft;
			el->height = h;

			el->top    = lastLoc;
			lastLoc   += h;

			int cwidth = child->align == Align::Right ? 0 :
				child->left+child->width+offsetSizeX();

			if (maxWidth < cwidth)
				maxWidth = cwidth;

		} else {

			int w = child->width+child->marginLeft+child->marginRight;

			el->width  = w;
			el->height = parent->height-parent->paddingTop-parent->paddingBottom;

			el->left   = lastLoc+child->marginLeft;
			lastLoc   += w;

			int cheight = child->verticalAlign == Align::Bottom ? 0 :
			              child->top+child->height+offsetSizeY();

			if (maxHeight < cheight)
				maxHeight = cheight;

		}

		el->updateAbsPos();
		el->render();

	}

	if (orientation == Orientation::Vertical) {
		width   = maxWidth > parent->width ? maxWidth : parent->width;
		width  -= offsetSizeX();
		height  = lastLoc;
	} else {
		width   = lastLoc;
		height  = maxHeight > parent->height ? maxHeight : parent->height;
		height -= offsetSizeY();
	}


}
