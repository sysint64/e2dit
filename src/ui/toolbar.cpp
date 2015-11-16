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

void UIToolbar::render() {

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

		tab->render();

	}

	renderElement (0, absLeft, absTop, width, iHeights[0], bgElement.get());

	if (lastSelected != nullptr) {

		lastSelected->render();

	}

}

void UIToolbarItem::render() {

	int ai = enter ? 1 : (checked ? 2 : 0);
	int ci = enter ? 3 : (checked ? 6 : 0);

	glUniform1f (manager->atlasShader->locations["Alpha"], alphas[ai]);

	updateAbsPos();
	manager->toolIcons->render (absLeft, app->windowHeight-manager->toolIcons->sizeIcon-absTop,
	                            iconOffset[0], iconOffset[1], iconElement.get());

	int twidth = ftglGetTextWidth (manager->theme->font, caption);
	int tx = absLeft+((width-twidth) >> 1);

	manager->atlasShader->unbind();
	renderText (manager->theme->font, &(textColors[ci]), tx, absTop+manager->toolIcons->sizeIcon+18, caption);
	manager->atlasShader->bind();

	glUniform1f (manager->atlasShader->locations["Alpha"], 1.0f);

}
