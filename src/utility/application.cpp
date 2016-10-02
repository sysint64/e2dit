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

/** @file application.cpp
    @brief handle application, main parameters and events
*/

#include "utility/application.h"

Application *Application::getInstance() {
	static Application *app = 0;

	if (!app) {

		app = new Application();

		for (int i = 0; i < 128; i++) {

			app->timersMap[i] = false;
			app->timers[i] = new Timer();

		}

	}

	return app;
}
