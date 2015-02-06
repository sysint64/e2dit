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

/** @file application.h
    @brief handle application, main parameters and events
*/

#ifndef E2DIT_SYSTEM_APPLICATION_H
#define E2DIT_SYSTEM_APPLICATION_H

#include <SFML/Window.hpp>
#include "../../logger.h"

class Application {
public:

	/* Window */

	int screenWidth  = 0;
	int screenHeight = 0;
	int windowWidth  = 0;
	int windowHeight = 0;

	sf::WindowHandle windowHandle;

	/* OpenGL */

	bool VAOEXT   = false;
	int  OGLMajor = 3;
	int  OGLMinor = 3;

	/* Cursor */

	int mouseX, mouseY;
	int clickX, clickY;

	/* Log */

	Logger log;

	/* */

	static Application *getInstance();

};

#endif
