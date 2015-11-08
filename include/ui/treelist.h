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

#ifndef E2DIT_UI_TREELIST_H
#define E2DIT_UI_TREELIST_H

#include <string>
#include <vector>

#include "ui/button.h"
#include "ui/element.h"
#include "ui/manager.h"

#include "renderer/base_object.h"

class UITreeList;
class UITreeListNode : public UIButton {
private:
	std::unique_ptr<BaseObject> expandElement = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
	UITreeList *treeList = nullptr;

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

	UITreeListNode (UIManager *manager, UITreeList *treeList) : UIButton (manager) {

		this->manager  = manager;
		this->treeList = treeList;

		style = "treelistnode";

		textAlign  = Align::Left;
		drawChilds = false;

		precompute();

	}

};

class UITreeList : public UIElement {
private:
	UIElement *lastCheck = nullptr;

public:

	friend UITreeListNode;
	UITreeListNode *selected = nullptr;
	bool drawLines = true;

	virtual void render() override;

	UITreeList (UIManager *manager) : UIElement (manager) {

		this->manager = manager;
		precompute();

	}

	inline void checkElement (UITreeListNode *el) {

		//assert (el->parent == this); TODO: Make method for check if el has child of this

		if (lastCheck != nullptr)
			lastCheck->checked = false;

		el->checked = true;
		lastCheck   = el;
		selected    = el;

	}

};

#endif
