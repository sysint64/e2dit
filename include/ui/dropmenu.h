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

#ifndef E2DIT_UI_DROPMENU_H
#define E2DIT_UI_DROPMENU_H

#include "ui/button.h"
#include "ui/listmenu.h"

class UIDropMenu : public UIButton {
private:
	UIListMenu *menu = nullptr;

public:
	UIDropMenu (UIManager *manager, bool isMenu = false) : UIButton(manager) {

		this->manager = manager;

		if (isMenu) {

			leaveElement = "flatbuttonleave";
			enterElement = "flatbuttonenter";
			clickElement = "flatbuttonenter";
			focusElement = "flatbuttonleave";

		} else {

			leaveElement = "droplistleave";
			enterElement = "droplistenter";
			clickElement = "droplistclick";

		}

		textAlign = Align::Left;
		showIcon  = true;

		precompute();

	}

};

#endif
