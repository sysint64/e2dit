/*
 * E2DIT - is a 2D Map Editor to create a levels for 2d games
 * Copyright (C) 2015 Kabylin Andrey <andrey@kabylin.ru>

 * This file is part of E2DIT.

 * E2DIT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * E2DIT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with E2DIT.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Kabylin Andrey <andrey@kabylin.ru>
 */

/** @file events.cpp
    @brief Core events implementaion
*/

#include "core.h"

void Core::onKeyPressed (int key) {

	uiManager->keyPressed (key);

}

void Core::onKeyReleased (int key) {

	uiManager->keyReleased (key);

}

void Core::onTextEntered (unsigned int key) {

	uiManager->textEntered (key);

}

void Core::onMouseDown (int x, int y, int button) {

	app->clickX = x;
	app->clickY = y;
	app->mouseButton = button;
	uiManager->mouseDown (x, y, button);

}

void Core::onMouseUp (int x, int y, int button) {

	app->mouseButton = mouseNone;
	uiManager->mouseUp (x, y, button);

}

void Core::onDblClick (int x, int y, int button) {

	uiManager->dblClick (x, y, button);

}

void Core::onMouseMove (int x, int y) {

	app->mouseX = x;
	app->mouseY = y;

	uiManager->mouseMove (x, y, app->mouseButton);

}

void Core::onMouseWheel (int dx, int dy) {

	uiManager->mouseWheel (dx, dy);

}

void Core::onResize (int width, int height) {

	if (height == 0) height = 1;

	app->windowWidth  = width;
	app->windowHeight = height;

	uiManager->resized (width, height);

}
