/*
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

/** @file core.h
    @brief
*/

#ifndef E2DIT_CORE_H
#define E2DIT_CORE_H

#include "../system/application.h"

class Application;
class Core {
public:

	Application *app;

	/* Constructor */

	 Core();
	~Core();

	/* Events */

	void onKeyPressed  (int key);
	void onKeyReleased (int key);
	void onTextEntered (unsigned int key);
	void onMouseMove   (int x, int y);
	void onMouseDown   (int x, int y, int button);
	void onMouseUp     (int x, int y, int button);
	void onDblClick    (int x, int y, int button);
	void onMouseWheel  (int x, int y, int button);
	void onResize      (int width, int height);

	/* */

	void render();
	void step();

};

#endif
