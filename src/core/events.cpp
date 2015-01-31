/* E2DIT - 2D Map Editor for game
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public 
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author: Kabylin Andrey <andrey@kabylin.ru>
 */

/** @file events.cpp
    @brief Core events implementaion
*/

#include "core.h"
#include "../system/application.h"

void Core::onKeyPressed (int key) {

}

void Core::onKeyReleased (int key) {

}

void Core::onTextEntered (unsigned int key) {

}

void Core::onMouseDown (int x, int y, int button) {

	app->clickX = x;
	app->clickY = y;

}

void Core::onMouseUp (int x, int y, int button) {

}

void Core::onDblClick (int x, int y, int button) {

}

void Core::onMouseMove (int x, int y) {

	app->mouseX = x;
	app->mouseY = y;

}

void Core::onMouseWheel (int x, int y, int button) {

}

void Core::onResize (int width, int height) {

}