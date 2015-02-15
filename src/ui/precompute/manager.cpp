/*
 * E2DIT - is a 2D Map Editor to create a levels for 2d games
 * Copyright (C) 2015 Kabylin Andrey <andrey@kabylin.ru>

 * This file is part of E2DIT-UI.

 * E2DIT-UI is free software: you can redistribute it and/or modify
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

#include "ui/manager.h"

/* Scissor */

void UIManager::pushScissor (int sx, int sy, int sw, int sh) {

	if (scissorStack.size() == 0)
		glEnable (GL_SCISSOR_TEST);

	scissorStack.push_back(glm::vec4(sx, sy, sx+sw, sy+sh));
	setScissor();

}

void UIManager::popScissor() {

	scissorStack.pop_back();

	if (scissorStack.size() == 0) glDisable (GL_SCISSOR_TEST);
	else setScissor();

}

void UIManager::setScissor () {

	int scx = scissorStack.back()[0]; int scy = scissorStack.back()[1];
	int scw = scissorStack.back()[2]; int sch = scissorStack.back()[3];

	if (scissorStack.size() >= 2)
	for (int i = 0; i < scissorStack.size(); i++) {

		if (scx < scissorStack[i][0]) scx = scissorStack[i][0];
		if (scy < scissorStack[i][1]) scy = scissorStack[i][1];

		if (scw > scissorStack[i][2]) scw = scissorStack[i][2];
		if (sch > scissorStack[i][3]) sch = scissorStack[i][3];

	}

	glScissor (scx, scy, scw-scx, sch-scy);

}

/* Render */

void UIManager::render() {

	texShader->bind();



}

/* Events */
