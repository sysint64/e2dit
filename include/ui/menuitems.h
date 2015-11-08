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

#include "ui/button.h"
#include "ui/listmenu.h"
#include "ui/manager.h"

#include "renderer/texture.h"

class UIMenuItem : public UIButton {
public:
	UIListMenu *menu = nullptr;
	std::wstring shortKey;

	virtual void render() override;

	inline void setMenu (std::unique_ptr<UIListMenu> m) {

		menu = m.get();
		menu->visible = false;
		addElement (std::move(m));

	}

	UIMenuItem (UIManager *manager) : UIButton (manager) {

		this->manager = manager;

		clickElement = "enter";
		style = "listitem";

		textAlign = Align::Left;
		showIcon  = true;

		precompute();

	}

};

class UITallMenuItem : public UIButton {
private:
	float descColors[6];
	std::unique_ptr<BaseObject> maskElement = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());

public:
	std::unique_ptr<Texture> texture;
	std::wstring desc;

	virtual void render()     override;
	virtual void precompute() override;

	inline void loadImage (const std::string &fileName) {

		texture = std::make_unique<Texture> (fileName.c_str());

	}

	UITallMenuItem (UIManager *manager) : UIButton (manager) {

		this->manager = manager;

		clickElement = "enter";
		style = "tallmenuitem";

		textAlign = Align::Left;
		showIcon  = true;
		noRenderText = true;

		precompute();

	}

};

class UIMenuHr : public UIElement {
public:
	UIMenuHr (UIManager *manager) : UIElement (manager) {
		this->manager = manager;
	}
};
