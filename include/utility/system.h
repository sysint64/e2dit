/*
 * E2DIT - is a 2D Map Editor to create a levels for 2d games
 * Copyright (C) 2015 Kabylin Andrey <andrey@kabylin.ru>

 * This file is part of E2DIT-UTILITY.

 * E2DIT-UTILITY is free library: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * E2DIT-UTILITY is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with E2DIT-UTILITY.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Kabylin Andrey <andrey@kabylin.ru>
 */

#pragma once

#include "utility/application.h"
#include "utility/config.h"

class TimerT {
private:
	float time = 0.f;
	float timeout;
	bool  loop = false;
	std::function<void()> callback;
	Application *app = Application::getInstance();

	void tick() {
		while (1) {
			time += app->deltaTime*1000.f;
			if (time >= timeout) {
				if (loop) {
					cycle++;
					time = 0.f;
				} else {
					break;
				}
			}
		}

		delete this;
	}
public:
	int cycle = 0;
	TimerT (float timeout, bool loop, std::function<void()> callback)
			: timeout(timeout), loop(loop), callback(callback)
	{
		tick();
	}

	static TimerT *setTimeout(float timeout, std::function<void()> callback) {
		return new TimerT(timeout, false, callback);
	}
};

static void setTimeout (float timeout, std::function<void()> callback) {

	Application *app = Application::getInstance();

	for (int i = 0; i < Application::timersCount; ++i) {

		if (app->timersMap[i])
			continue;

		app->timersMap[i] = true;
		app->activeTimers.push_back(i);

		app->timers[i]->time = 0.f;
		app->timers[i]->timeout = timeout;
		app->timers[i]->callback = callback;
		app->timers[i]->loop = false;

		return;

	}

}

static int setInterval (float timeout, std::function<void()> callback) {

	Application *app = Application::getInstance();

	for (int i = 0; i < Application::timersCount; ++i) {

		if (app->timersMap[i])
			continue;

		app->timersMap[i] = true;
		app->activeTimers.push_back(i);

		app->timers[i]->time = 0.f;
		app->timers[i]->timeout = timeout;
		app->timers[i]->callback = callback;
		app->timers[i]->loop = true;

		return i;

	}

	return -1;

}
