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

#ifndef E2DIT_UTILITY_UI_H
#define E2DIT_UTILITY_UI_H

#include "renderer/shader.h"
#include "utility/string.h"
#include "FTGL/ftgl.h"

using namespace FTGL;

/**
 *
 */

inline bool pointInRect (int X, int Y, int Xe, int Ye, int W, int H) {

	return ((X <= Xe+W) && (X >= Xe) &&
	        (Y <= Ye+H) && (Y >= Ye));

}

/**
 *
 */

inline int ftglGetTextWidth (FTGLfont *font, std::wstring text) {

	float bounds[6];
	int   n = text.size();
	std::string encodedText = wstr2str (text);

	ftglGetFontBBox (font, encodedText.c_str(), n, bounds);
	float twidth = bounds[3]-bounds[0];

	if (text[n-1] == ' ') twidth += 3; // FIXME: Why += 3 ?
	return twidth;

}

inline int ftglGetCharWidth (FTGLfont *font, wchar_t ch) {

	float bounds[6];
	std::string encodedCh = encodeUTF8 (ch);

	ftglGetFontBBox (font, encodedCh.c_str(), 1, bounds);
	float twidth = bounds[3]-bounds[0];

	if (ch == ' ') twidth += 3; // FIXME: Why += 3 ?
	return twidth;

}

#endif
