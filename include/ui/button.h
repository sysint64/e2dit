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
> */

#ifndef E2DIT_UI_BUTTON_H
#define E2DIT_UI_BUTTON_H

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "utility/config.h"
#include "utility/renderer.h"

#include "ui/element.h"
#include "ui/manager.h"

#include "renderer/shader.h"
#include "renderer/data_render.h"
#include "renderer/base_object.h"

class UIButton : public UIElement {
protected:

	/* Param Names for load from Layout file */

	std::string leaveElement = "buttonleave";
	std::string enterElement = "buttonenter";
	std::string clickElement = "buttonclick";
	std::string focusElement = "buttonfocus";

	/* Render Objects */

	std::unique_ptr<BaseObject> leftElement   = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
	std::unique_ptr<BaseObject> rightElement  = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
	std::unique_ptr<BaseObject> middleElement = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
	std::unique_ptr<BaseObject> iconElement   = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
	std::unique_ptr<BaseObject> iconElement2  = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());

	int focusOffsets[3]; // Left, Top, Width

	/* Draw Text */

	void renderText (Align align, std::string text, int size, int offset = 0);
	void renderSkin();

public:

	std::wstring caption = L"Test Button";
	int  icoOffset = 0;
	bool noRenderText = false;
	Align textAlign = Align::Center;

	virtual void precompute() override;
	virtual void render()     override;

	//using UIElement::UIElement;
	UIButton (UIManager *manager) : UIElement (manager) {

		this->manager = manager;
		precompute();

	}

};

#endif
