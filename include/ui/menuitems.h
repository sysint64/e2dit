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

#include "ui/button.h"
#include "ui/listmenu.h"
#include "ui/manager.h"

#include "renderer/texture.h"

namespace ui {

	class MenuItem : public Button {
	public:
		ListMenu *menu = nullptr;
		std::wstring shortKey;

		virtual void render() override;

		inline void setMenu (std::unique_ptr<ListMenu> m) {

			menu = m.get();
			menu->visible = false;
			addElement (std::move(m));

		}

		virtual void addElement (std::unique_ptr<Widget> el) override {
			menu = dynamic_cast<ListMenu*>(el.get());

			if (menu) {
				menu->visible = false;
				manager->overlayElements.push_back(menu);
				Widget::addElement (std::move(el));
			} else {
				Widget::addElement (std::move(el));
			}

		}

		MenuItem (Manager *manager) : Button (manager) {

			this->manager = manager;

			clickElement = "enter";
			style = "listitem";

			textAlign  = Align::Left;
			showIcon   = true;
			drawChilds = false;

			precompute();

		}

	};

	class UITallMenuItem : public Button {
	private:
		float descColors[6];
		std::unique_ptr<gapi::BaseObject> maskElement = std::make_unique<gapi::BaseObject> (manager->uiDataRender, app->screenCamera.get());

	public:
		std::unique_ptr<gapi::Texture> texture;
		std::wstring desc;

		virtual void render()     override;
		virtual void precompute() override;

		inline void loadImage (const std::string &fileName) {
			texture = std::make_unique<gapi::Texture> (fileName.c_str());
		}

		UITallMenuItem (Manager *manager) : Button (manager) {

			this->manager = manager;

			clickElement = "enter";
			style = "tallmenuitem";

			textAlign = Align::Left;
			showIcon  = true;
			noRenderText = true;

			precompute();

		}

	};

	class UIMenuHr : public Widget {
	public:
		UIMenuHr (Manager *manager) : Widget (manager) {
			this->manager = manager;
		}
	};

};
