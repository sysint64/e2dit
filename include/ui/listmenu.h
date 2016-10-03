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

#pragma once

#include "ui/element.h"
#include "ui/manager.h"
#include "ui/button.h"

#include "renderer/base_object.h"

/* Menu */

class UIListMenu : public UIElement {
protected:
	std::unique_ptr<gapi::BaseObject> drawElements[12];
	gapi::BaseObject *drawElementsPtr[12];
	int indices[12];
	float delay = 0.f; // Menu delay

	UIElement  *lastCheck  = nullptr;
	UIListMenu *openedMenu = nullptr;
	bool setedTime = false;

public:
	bool checkList   = false;
	bool transparent = false;
	bool popup       = true;

	std::string listitemstyle;

	inline void closeSubMenus() const {

		if (!openedMenu) return;

		openedMenu->visible = false;
		openedMenu->closeSubMenus();

	}

	virtual void precompute() override;
	virtual void render()     override;

	virtual void mouseDown (int x, int y, int button) override;

	UIListMenu (UIManager *manager) : UIElement (manager) {

		for (int i = 0; i < 12; ++i) {

			drawElements   [i] = std::make_unique<gapi::BaseObject>(manager->uiDataRender, app->screenCamera.get());
			drawElementsPtr[i] = drawElements[i].get();

			indices[i] = i;

		}

		this->manager = manager;

		style         = "listmenu";
		listitemstyle = "listitem";

		precompute();

	}

	inline void checkElement (UIElement *el) {

		assert (el->parent == this);

		if (lastCheck != nullptr)
			lastCheck->checked = false;

		el->checked = true;
		lastCheck   = el;

	}

};
