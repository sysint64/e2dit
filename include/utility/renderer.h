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

#ifndef E2DIT_UTILITY_RENDERER_H
#define E2DIT_UTILITY_RENDERER_H

#include "GL/glew.h"

#include "utility/application.h"
#include "utility/string.h"

#include "FTGL/ftgl.h"

using namespace FTGL;

inline void glBegin2D () {

	Application *app = Application::getInstance();

	glMatrixMode (GL_PROJECTION);

	glPushMatrix(); glLoadIdentity();

	gluOrtho2D   (0, app->screenWidth, 0, app->screenHeight);
	glMatrixMode (GL_MODELVIEW);

	glPushMatrix(); glLoadIdentity();

}

inline void glEnd2D () {

	glPopMatrix();
	glMatrixMode (GL_PROJECTION);

	glPopMatrix();
	glMatrixMode (GL_MODELVIEW);

}

inline void renderText (FTGLfont *font, float r, float g, float b, int x, int y, std::wstring text) {

	Application *app = Application::getInstance();
	std::string decodedText = wstr2str (text);
	glBegin2D();

	glActiveTexture (GL_TEXTURE0);
	glColor3f       (r, g, b);

	glTranslatef    (x, app->windowHeight-y, 0);
	ftglRenderFont  (font, decodedText.c_str(), RENDER_ALL);

	glEnd2D();

}

inline void renderText (FTGLfont *font, float *color, int x, int y, std::wstring text) {

	Application *app = Application::getInstance();
	std::string decodedText = wstr2str (text);
	glBegin2D();

	glActiveTexture (GL_TEXTURE0);
	glColor3fv      (color);

	glTranslatef    (x, app->windowHeight-y, 0);
	ftglRenderFont  (font, decodedText.c_str(), RENDER_ALL);

	glEnd2D();

}

#endif
