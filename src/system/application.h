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

/** @file application.h
    @brief handle application, main parameters and events
*/

#ifndef E2DIT_SYSTEM_APPLICATION_H
#define E2DIT_SYSTEM_APPLICATION_H

#include <SFML/Window.hpp>
#include "../../logger.h";

class Application {
public:
	
	/* Window */

	int screenWidth  = 0;
	int screenHeight = 0;
	int windowWidth  = 0;
	int windowHeight = 0;

	sf::WindowHandle windowHandle;

	/* Cursor */

	int mouseX, mouseY;
	int clickX, clickY;

	/* Log */

	Logger log;

};

#endif
