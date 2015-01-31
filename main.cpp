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

/* @file  main.cpp
 * @brief main() entrypoint implementation for e2dit
 */

#include "config.h"
#include "logger.h"
#include "GL/glew.h"
#include "src/system/application.h"
#include "src/core/core.h"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

Application app;

/* Main */

int main() {

	setlocale (LC_CTYPE, "");
	app.log.create ("log.txt");

	/* Initialize OpenGL */

	sf::ContextSettings settings;
	
	settings.depthBits         = 24;
	settings.stencilBits       = 8;
	settings.antialiasingLevel = 0;
	settings.majorVersion      = OGL_MAJOR;
	settings.minorVersion      = OGL_MINOR;

	app.screenWidth  = sf::VideoMode::getDesktopMode().width;
	app.screenHeight = sf::VideoMode::getDesktopMode().height;

	/* TODO */

	app.windowWidth  = app.screenWidth -40;
	app.windowHeight = app.screenHeight-80;

	/* Create Window */

	sf::Window window (sf::VideoMode (app.windowWidth, app.windowHeight, 24), APP_NAME,
					   sf::Style::Default, settings);

	app.windowHandle = window.getSystemHandle();

	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(0);

	/* Check OpenGL Errors */

	GLenum err = glewInit();

	if (err != GLEW_OK) {
		app.log.write ("Error: %s\n", glewGetErrorString(err));
		exit (EXIT_FAILURE);
	}

	if (!__GLEW_VERSION_2_1) {
		app.log.write ("Error: OpenGL Version less than 2.1\n");
		exit (EXIT_FAILURE);
	}

	if (OGL_MAJOR == 3 && !__GLEW_VERSION_3_3) {
		app.log.write ("Error: OpenGL Version less than 3.3\n");
		exit (EXIT_FAILURE);
	}

	if (OGL_MAJOR == 4 && !__GLEW_VERSION_4_3) {
		app.log.write ("Error: OpenGL Version less than 4.3\n");
		exit (EXIT_FAILURE);
	}
	
	app.log.write ("Initialize OpenGL, Version : %d.%d\n", OGL_MAJOR, OGL_MINOR);

	/* Create Core */

	Core *core = new Core (&app);

	/* Main loop */

	bool running  = true;
	bool dblClick = false;

	while (running) {

		sf::Event ev;

		/* Poll Events */

		while (window.pollEvent (ev)) {

			switch (ev.type) {

				/* Window */

				case sf::Event::Closed	: running = false; break;
				case sf::Event::Resized	: core->onResize (ev.size.width, ev.size.height); break;

				/* Mouse */

				case sf::Event::MouseMoved			: core->onMouseMove  (ev.mouseMove.x     , ev.mouseMove  .y);                        break;
				case sf::Event::MouseWheelMoved		: core->onMouseWheel (ev.mouseWheel.delta, ev.mouseWheel .x, ev.mouseWheel.y);       break;
				case sf::Event::MouseButtonPressed	: core->onMouseDown  (ev.mouseButton.x   , ev.mouseButton.y, ev.mouseButton.button); break;
				case sf::Event::MouseButtonReleased :

					core->onMouseUp (ev.mouseButton.x, ev.mouseButton.y, ev.mouseButton.button);

					if (dblClick)
						core->onDblClick (ev.mouseButton.x, ev.mouseButton.y, ev.mouseButton.button);

					dblClick = true;
					break;

				/* Keyboard */

				case sf::Event::KeyPressed	: core->onKeyPressed  (ev.key.code);      break;
				case sf::Event::TextEntered	: core->onTextEntered (ev.text.unicode);  break;
				case sf::Event::KeyReleased : core->onKeyReleased (ev.key.code);      break;

			}

		}

		window.setActive();

		/* Render */

		glClear (GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		core->render();
		core->step();

		/* Display */

		glFlush();
		window.display();

	}

	delete core;
	return 0;

}
