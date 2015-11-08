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

#ifndef E2DIT_UI_GROUPED_H
#define E2DIT_UI_GROUPED_H

#include <string>

#include "ui/element.h"
#include "ui/manager.h"
#include "ui/button.h"

#include "renderer/base_object.h"

class UIGrouped : public UIElement {
protected:

	std::unique_ptr<BaseObject> splitElement = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
	UIElement *lastSelected = nullptr;

	bool multiSelect = false;
	int  spacing     = 0;

public:

	friend UIButton;

	virtual void precompute() override;
	virtual void render()     override;

	inline void checkElement (UIElement* el) {

		if (el->parent != this)
			return;

		if (!multiSelect && lastSelected != nullptr)
			lastSelected->checked = false;

		el->checked  = true;
		lastSelected = el;

	}

	UIGrouped (UIManager *manager, bool multiSelect = false, bool spacing = 0) : UIElement (manager) {

		this->manager     = manager;
		this->multiSelect = multiSelect;
		this->spacing     = spacing;

		precompute();

	}

};

#endif
