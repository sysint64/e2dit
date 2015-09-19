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

/* @file  main.cpp
 * @brief main() entrypoint implementation for e2dit
 */

#include "utility/config.h"
#include "GL/glew.h"

#include "utility/logger.h"
#include "utility/application.h"
#include "utility/data_map.h"
#include "utility/system.h"
#include "core/core.h"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#ifdef _linux_
	#include <X11/Xlib.h>
	#include <X11/Xatom.h>
#endif

#define _NET_WM_STATE_REMOVE   0l
#define _NET_WM_STATE_ADD      1l
#define _NET_WM_STATE_TOGGLE   2l

namespace fs = boost::filesystem;

/* Main */

int main (int argc,char** argv) {

	fs::path full_path (fs::initial_path<fs::path>());
	full_path = fs::system_complete (fs::path (argv[0]));

	//puts (full_path.string().c_str());

	Application *app = Application::getInstance();

	setlocale (LC_CTYPE, "");
	app->log.create ("log.txt");

	/* Initialize OpenGL */

	sf::ContextSettings settings;

	settings.depthBits         = 24;
	settings.stencilBits       = 8;
	settings.antialiasingLevel = 0;
	settings.majorVersion      = app->OGLMajor;
	settings.minorVersion      = app->OGLMinor;

	app->screenWidth  = sf::VideoMode::getDesktopMode().width;
	app->screenHeight = sf::VideoMode::getDesktopMode().height;

	/* #TODO:0 */

	app->windowWidth  = 1024;//app->screenWidth -100;
	app->windowHeight = 640;//app->screenHeight-100;

	/* Create Window */

	sf::Window window (sf::VideoMode (app->windowWidth, app->windowHeight, 24), APP_NAME,
					   sf::Style::Default, settings);

	#ifdef _linux_

		/* Maximize Window */

		XEvent xev;
		Display* display  = XOpenDisplay (NULL);

		Atom _NET_WM_STATE = XInternAtom(display, "_NET_WM_STATE", False);
		Atom _NET_WM_STATE_MAXIMIZED_VERT = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_VERT", False);
		Atom _NET_WM_STATE_MAXIMIZED_HORZ = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);

		XEvent e;

		e.xany.type = ClientMessage;
		e.xany.window = window.getSystemHandle();
		e.xclient.message_type = _NET_WM_STATE;
		e.xclient.format = 32;
		e.xclient.data.l[0] = _NET_WM_STATE_ADD;
		e.xclient.data.l[1] = _NET_WM_STATE_MAXIMIZED_VERT;
		e.xclient.data.l[2] = _NET_WM_STATE_MAXIMIZED_HORZ;
		e.xclient.data.l[3] = 0l;
		e.xclient.data.l[4] = 0l;

		/*XSendEvent (display, DefaultRootWindow(display), 0, SubstructureNotifyMask | SubstructureRedirectMask, &e);
		XSync (display, False);*/

	#endif

	app->windowHandle = window.getSystemHandle();
	app->cursor = std::make_unique<UICursor> (window.getSystemHandle());

	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(60);

	/* Check OpenGL Errors */

	GLenum err = glewInit();

	if (err != GLEW_OK) {

		app->log.write ("Error: %s\n", glewGetErrorString(err));
		exit (EXIT_FAILURE);

	}

	if (!__GLEW_VERSION_2_1) {

		app->log.write ("Error: OpenGL Version less than 2.1\n");
		exit (EXIT_FAILURE);

	}

	/*if (app->OGLMajor == 3 && !__GLEW_VERSION_3_3) {

		app->log.write ("Error: OpenGL Version less than 3.3\n");
		exit (EXIT_FAILURE);

	}

	if (app->OGLMajor == 4 && !__GLEW_VERSION_4_3) {

		app->log.write ("Error: OpenGL Version less than 4.3\n");
		exit (EXIT_FAILURE);

	}*/

	app->log.write ("Initialize OpenGL, Version : %d.%d\n", app->OGLMajor, app->OGLMinor);

	/* Create Core */

	std::unique_ptr<Core> core = std::make_unique<Core>();

	sf::Event ev;
	window.setActive();
	window.pollEvent (ev);
	core->onResize (window.getSize().x, window.getSize().y);

	/* */

	bool  running  = true;
	bool  dblClick = false;
	float dblClickTime = 0;
	float lastTime = 0;

	sf::Clock clock;

	glDisable(GL_CULL_FACE);

	glDisable   (GL_MULTISAMPLE);
	glDisable   (GL_DEPTH_TEST);
	glEnable    (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport (0, 0, app->screenWidth, app->screenHeight);
	//glClearColor (0, 0, 0, 0);
	//glClearColor (144.f/255.f, 144.f/255.f, 144.f/255.f, 0);
	glClearColor (150.f/255.f, 150.f/255.f, 150.f/255.f, 0);

	/*setTimeout (1.f, []() {

		std::cout << "Hello World!" << std::endl;

	});

	setInterval (2.f, []() {

		std::cout << "Hello World2!" << std::endl;

	});*/

	/* Main loop */

	while (running) {

		sf::Event ev;
		glViewport (0, 0, app->screenWidth, app->screenHeight);

		//core->onResize (app->windowWidth, app->windowHeight);

		sf::Vector2i mp = sf::Mouse::getPosition(window);
		//std::cout << mp.x << ":" << mp.y << std::endl;
		app->mouseX = mp.x;
		app->mouseY = mp.y;

		/* Poll Events */

		while (window.pollEvent (ev)) {

			switch (ev.type) {

				/* Window */

				case sf::Event::Closed  : running = false; break;
				case sf::Event::Resized : core->onResize (ev.size.width, ev.size.height); break;

				/* Mouse */

				case sf::Event::MouseMoved: core->onMouseMove (ev.mouseMove.x, ev.mouseMove.y); break;
				case sf::Event::MouseWheelScrolled:

					     if (ev.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel  ) core->onMouseWheel (0, ev.mouseWheelScroll.delta);
					else if (ev.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) core->onMouseWheel (ev.mouseWheelScroll.delta, 0);

					break;

				case sf::Event::MouseButtonPressed  : core->onMouseDown  (ev.mouseButton.x   , ev.mouseButton.y, ev.mouseButton.button); break;
				case sf::Event::MouseButtonReleased :

					core->onMouseUp (ev.mouseButton.x, ev.mouseButton.y, ev.mouseButton.button);

					if (dblClick)
						core->onDblClick (ev.mouseButton.x, ev.mouseButton.y, ev.mouseButton.button);

					dblClick = true;
					break;

				/* Keyboard */

				case sf::Event::KeyPressed  : core->onKeyPressed  (ev.key.code);      break;
				case sf::Event::TextEntered : core->onTextEntered (ev.text.unicode);  break;
				case sf::Event::KeyReleased : core->onKeyReleased (ev.key.code);      break;

				default : break;
			}

		}

		/* Calculate DeltaTime */

		app->currentTime = clock.getElapsedTime().asSeconds();
		app->deltaTime   = (app->currentTime - lastTime)*0.001f;
		lastTime         =  app->currentTime;

		/* Timers */

		//for (auto timerId : app->activeTimers) {
		for (auto it = app->activeTimers.begin(); it != app->activeTimers.end(); ) {

			int timerId = *it;
			app->timers[timerId]->time += app->deltaTime*1000.f;

			if (app->timers[timerId]->time >= app->timers[timerId]->timeout) {

				app->timers[timerId]->time = 0.f;
				app->timers[timerId]->callback();

				if (!app->timers[timerId]->loop) {

					app->timers[timerId]->timeout = 0.f;
					app->timersMap[timerId] = false;
					app->activeTimers.erase(it);

				}

			} else {

				++it;

			}

		}

		/* Check Double Click */

		if (dblClick) dblClickTime += app->deltaTime*1000.f;
		else dblClickTime = 0;

		if (dblClickTime >= 0.3f) {

			dblClick      = false;
			dblClickTime  = 0;

		}

		/* Render */

		window.setActive();

		/* Render */

		glClear (GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		core->step();
		core->render();

		/* Display */

		glFlush();
		window.display();

	}

	return 0;

}
