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

class UIManager;
class UIElement {
public:
	int id;

	int width;
	int height;
	int left;
	int top;

	UIElement *parent;
	UIManager *manager;

	/* Methods */

	UIElement (UIManager *manager) {

		this->manager = manager;
		precompute();

	}

	void setFocused();
	void unFocused();

	virtual void precompute() {}
	virtual void draw (int x, int y) {}

	/* Events */

	virtual void dblClick    (int x, int y, int shift);
	//
	virtual void mouseDown   (int x, int y, int shift);
	virtual void mouseMove   (int x, int y, int shift) {}
	virtual void mouseUp     (int x, int y, int shift);
	virtual void keyDown     (int key) {}
	virtual void resized     (int width, int height) {}
	virtual void keyPressed  (Uint16 key);
	virtual void textEntered (Uint16 key) {}

};

#endif
