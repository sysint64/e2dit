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

#pragma once

#include "ui/element.h"
#include "ui/button.h"
#include "ui/manager.h"

#include "renderer/base_object.h"
#include <string>

class UIToolbarSplit : public UIElement {
protected:
	std::unique_ptr<BaseObject> splitElement = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());

public:

	virtual void precompute() override;
	virtual void render()     override;

	UIToolbarSplit (UIManager *manager) : UIElement (manager) {

		this->style   = "toolbar";
		this->manager = manager;

		precompute();

	}

};

class UIToolbarTab : public UIButton {
public:

	UIToolbarTab (UIManager *manager) : UIButton (manager) {

		style         = "toolbartab";
		drawChilds    = false;
		iconLeft      = 12;
		allowCheck    = true;
		this->manager = manager;

		precompute();

	}

};

class UIToolbar;
class UIToolbarItem : public UIElement {
protected:
	std::unique_ptr<BaseObject> iconElement = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
	UIToolbar *toolbar = nullptr;

public:
	std::string leaveElement = "leave";
	std::string enterElement = "enter";
	std::string clickElement = "click";

	int iconOffset[2];
	std::wstring caption;
	bool allowCheck = true;
	float alphas[3];

	virtual void precompute() override;
	virtual void render()     override;

	virtual void mouseDown (int x, int y, int button) override;

	UIToolbarItem (UIManager *manager, UIToolbar *toolbar) : UIElement (manager) {

		style  = "toolbaritem";
		width  = manager->toolIcons->sizeIcon;
		height = manager->toolIcons->sizeIcon+18+manager->theme->fontHeight+2;

		this->manager = manager;
		this->toolbar = toolbar;

		precompute();

	}

};

class UIToolbar : public UICheckedElements {
protected:
	std::unique_ptr<BaseObject> bgElement    = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
	std::unique_ptr<BaseObject> splitElement = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());

	int shadowHeight = 0;
	int tabOffset    = 0;

public:

	UIElement *lastTool = nullptr;

	int tabsOffset = 20;
	int tabSize    = 150;

	virtual void precompute() override;
	virtual void render()     override;

	UIToolbar (UIManager *manager) : UICheckedElements (manager) {

		align = Align::Top;
		style = "toolbar";
		this->manager = manager;

		precompute();

	}

	void checkTool (UIElement *tool) {

		if (lastTool != nullptr)
			lastTool->checked = false;

		tool->checked = true;
		lastTool = tool;

	}

};
