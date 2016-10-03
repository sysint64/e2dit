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

#include <string>
#include <vector>

#include "ui/button.h"
#include "ui/widget.h"
#include "ui/manager.h"

#include "renderer/base_object.h"

namespace ui {
	class TreeList;
	class TreeListNode : public Button {
	private:
		std::unique_ptr<gapi::BaseObject> expandElement = std::make_unique<gapi::BaseObject> (manager->uiDataRender, app->screenCamera.get());
		TreeList *treeList = nullptr;

		int  heightIn = 0;
		int  btnOffset = 0;
		bool buttonEnter = false;

		void renderLines();

	public:
		bool open      = true;
		bool allowHide = true;

		virtual void precompute() override;
		virtual void render()     override;

		virtual void mouseUp   (int x, int y, int button) override;
		virtual void mouseDown (int x, int y, int button) override;

		TreeListNode (Manager *manager, TreeList *treeList) : Button (manager) {

			this->manager  = manager;
			this->treeList = treeList;

			style = "treelistnode";

			textAlign  = Align::Left;
			drawChilds = false;

			precompute();

		}

	};

	class TreeList : public Widget {
	private:
		Widget *lastCheck = nullptr;

	public:
		friend TreeListNode;
		TreeListNode *selected = nullptr;
		bool drawLines = true;

		virtual void render() override;

		TreeList (Manager *manager) : Widget (manager) {

			this->manager = manager;
			precompute();

		}

		inline void checkElement (TreeListNode *el) {

			//assert (el->parent == this); TODO: Make method for check if el has child of this

			if (lastCheck != nullptr)
				lastCheck->checked = false;

			el->checked = true;
			lastCheck   = el;
			selected    = el;
		}

	};
};