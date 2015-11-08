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

#include "ui/grouped.h"
#include "utility/math.h"

void UIGrouped::render() {

	updateAbsPos();

	int wh = 0;
	int countFixSize = 0;

	/* Calculate sizes */

	for (const auto &kvp : elements) {

		UIElement *el = kvp.second.get();

		if (!el->autoSize) {

			wh += el->width;
			++countFixSize;

		}

	}

	int partWidth = round (static_cast<float>(width-wh)/static_cast<float>(elements.size()-countFixSize));
	int offset = 0;

	partWidth += spacing;

	UIElement *firstElement = elements. begin()->second.get();
	UIElement *lastElement  = elements.rbegin()->second.get();

	if (spacing == 0) {

		firstElement->drawAlign = Align::Left;
		lastElement ->drawAlign = Align::Right;

	}

	for (const auto &kvp : elements) {

		UIElement *el = kvp.second.get();

		el->top  = 0;
		el->left = offset;

		if (spacing == 0 && el != firstElement && el != lastElement)
			el->drawAlign = Align::Center;

		if (el->autoSize) {

			offset += partWidth;
			el->width = partWidth;

		} else {

			offset += el->width;

		}

		height = math::max (height, el->height);
		el->render();

		// Draw Split

		if (spacing == 0 && el != lastElement)
			renderElement (0, absLeft+offset-iWidths[0], absTop, iWidths[0], iHeights[0], splitElement.get());

	}

}
