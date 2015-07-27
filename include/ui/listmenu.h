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

#ifndef E2DIT_UI_LISTMENU_H
#define E2DIT_UI_LISTMENU_H

#include "ui/element.h"
#include "ui/manager.h"
#include "ui/button.h"

#include "renderer/base_object.h"

/* Menu */

class UIListMenu : public UIElement {
protected:
	std::unique_ptr<BaseObject> drawElements[12];
	BaseObject *drawElementsPtr[12];
	int indices[12];
	float delay = 0.f; // Menu delay

	UIElement *lastCheck = nullptr;

public:
	bool checkList   = false;
	bool transparent = false;
	bool popup       = true;

	virtual void precompute() override;
	virtual void render()     override;

	virtual void mouseDown (int x, int y, int button) override;

	UIListMenu (UIManager *manager) : UIElement (manager) {

		for (int i = 0; i < 12; ++i) {

			drawElements   [i] = std::make_unique<BaseObject>(manager->uiDataRender, app->screenCamera.get());
			drawElementsPtr[i] = drawElements[i].get();

			indices[i] = i;

		}

		this->manager = manager;
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

/* Menu Item */

class UIMenuItem : public UIButton {
public:
	std::unique_ptr<UIListMenu> menu = nullptr;
	std::wstring shortKey;

	virtual void render() override;

	void setMenu (std::unique_ptr<UIListMenu> m) {

		menu = std::move (m);
		menu->parent = manager->root.get();

		menu->next   = menu.get();  menu->prev    = menu.get();
		menu->lastEl = menu.get();  menu->firstEl = menu.get();

		menu->visible = false;
		manager->elementsStack.push_back (menu.get());

	}

	UIMenuItem (UIManager *manager) : UIButton (manager) {

		this->manager = manager;

		leaveElement = "listitemleave";
		enterElement = "listitementer";
		clickElement = "listitementer";
		focusElement = "listitemfocus";

		textAlign = Align::Left;
		showIcon  = true;

		precompute();

	}

};

class UIMenuHr : public UIElement {
public:
	UIMenuHr (UIManager *manager) : UIElement (manager) {
		this->manager = manager;
	}
};

#endif
