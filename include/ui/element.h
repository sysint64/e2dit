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

#ifndef E2DIT_UI_ELEMENT_H
#define E2DIT_UI_ELEMENT_H

#include "utility/config.h"
#include "utility/application.h"
#include "utility/string.h"
#include <functional>

enum class Align {Left, Center, Right, All};
class UIManager;
class UIElement {
protected:
	Application *app = Application::getInstance();

	/* Icon */

	int iconOffset[2];      bool showIcon      = false;
	int icon2Offset[2];     bool showIcon2     = false;
	int clickIconOffset[2]; bool showClickIcon = false;

public:
	int id;

	int width  , height;
	int absLeft, absTop;
	int left   , top;

	bool visible = true;
	bool enabled = true;
	bool focused = false;

	bool withoutSkin = false;

	/* Mouse State */

	bool enter = false;
	bool leave = true;
	bool click = false;

	bool inDialog = false;

	UIElement *parent;
	UIManager *manager;

	Align drawAlign = Align::All;

	/* Functors */

	std::function<void(UIElement*)>                onClick      = nullptr;
	std::function<void(UIElement*)>                onProgress   = nullptr;
	std::function<void(UIElement*)>                onUnfocused  = nullptr;
	std::function<void(UIElement*, Uint16)>        onKeyPressed = nullptr;
	std::function<void(UIElement*, int, int, int)> onMouseDown  = nullptr;
	std::function<void(UIElement*, int, int, int)> onDblClick   = nullptr;

	/* Methods */

	UIElement (UIManager *manager) {

		this->manager = manager;
		precompute();

	}

	virtual ~UIElement() {}

	void focus();
	void unfocus();

	virtual void precompute() {}
	virtual void render (int x, int y) {

		left    = x; top    = y;
		absLeft = x; absTop = y;

	}

	/* Events */

	virtual void dblClick    (int x, int y, int button);
	//
	virtual void mouseDown   (int x, int y, int button);
	virtual void mouseMove   (int x, int y, int button) {}
	virtual void mouseUp     (int x, int y, int button);
	virtual void keyDown     (int key) {}
	virtual void resized     (int width, int height) {}
	virtual void keyPressed  (int key);
	virtual void textEntered (int key) {}
	virtual void progress    () {}

};

#endif
