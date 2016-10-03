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

#include "ui/dropmenu.h"

void ui::DropMenu::render() {

	overHeight = 1000;
	Button::render();

	if (!menu || isMenu)
		return;

	int x = absLeft+width-iWidths[0]-iWidths[2]-4;
	int y = absTop;

	renderElement (12, x, y, iWidths[12], iHeights[12], arrowElement.get());

}

void ui::DropMenu::updateMenu() {

	menu->top  = height+menuOffsets[2];
	menu->left = menuOffsets[0];

	if (menu->autoWidth)
		menu->width = width+menuOffsets[1]-menuOffsets[0];

}

void ui::DropMenu::mouseUp (int x, int y, int button) {

	if (!menu)
		return;

	if (enter) {

		checked = !checked;

		if (!checked)
			menu->closeSubMenus();
		updateMenu();

	} else {

		checked = false;
		menu->closeSubMenus();

	}

	menu->visible = checked;

}
