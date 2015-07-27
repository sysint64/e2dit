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

#include "ui/listmenu.h"

/* Menu Item */

void UIMenuItem::render() {

	UIButton::render();

	if (shortKey.size() != 0) {

		std::string text = wstr2str (shortKey);

		if (menu == nullptr) renderText (Align::Right, text, shortKey.size());
		else 				 renderText (Align::Right, text, shortKey.size(), 12);

	}

}

/* List Menu */

void UIListMenu::render() {

	updateAbsPos();

	if (popup)
		renderPartsElementBlock (indices, drawElementsPtr, absLeft, absTop, width, height);

	int ey = iHeights[1];

	for (const auto &kvp : elements) {

		UIElement *el = kvp.second.get();

		if (dynamic_cast<UIMenuHr*>(el)) {

			int x = absLeft+iWidths[1];
			int y = absTop+ey+1;

			int w = width - (iWidths[1] << 1);
			int h = iHeights[11];

			renderElement (11, x, y, w, h, drawElementsPtr[11]);
			ey += 7;

		} else {

			el->width = width-8;
			el->left  = 4;
			el->top   = ey;

			ey += el->height+1;
			el->render();

			UIMenuItem *item = dynamic_cast<UIMenuItem*>(el);

			if (item) {

				if (item->menu == nullptr)
					continue;

				int n = 9;
				if (item->enter) n = 10;

				int x = absLeft+width-iWidths[5]-iWidths[3]-4;
				int y = absTop+ey-item->height-1;

				renderElement (n, x, y, iWidths[n], iHeights[n], drawElementsPtr[9]);

			}

		}

	}

	height = ey+iHeights[7];

}

void UIListMenu::mouseDown (int x, int y, int button) {

	for (const auto &kvp : elements) {

		UIElement *el = kvp.second.get();
		el->mouseDown (x, y, button);

		if (!checkList) return;
		if (el->enter && button == mouseLeft) {

			if (lastCheck != nullptr)
				lastCheck->checked = false;

			el->checked = true;
			lastCheck   = el;

		}

	}

}
