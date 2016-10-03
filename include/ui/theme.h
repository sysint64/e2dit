/*
 * E2DIT - is a 2D Map Editor to create a levels for 2d games
 * Copyright (C) 2015 Kabylin Andrey <andrey@kabylin.ru>

 * This file is part of E2DIT-UI.

 * E2DIT-UI is free library: you can redistribute it and/or modify
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

#pragma once

#include "utility/data_map.h"
#include "renderer/texture.h"
#include "GL/glew.h"
#include "FTGL/ftgl.h"

using namespace FTGL;

namespace ui {
	class Theme : public DataMap {
	public:
		gapi::Texture *skin;

		FTGLfont *font; // Main font
		FTGLfont *font12;
		FTGLfont *font14;
		FTGLfont *font16;

		int fontHeight;
		int font12Height;
		int font14Height;
		int font16Height;

	public:

		Theme (const std::string &fileName, ReadType rt, gapi::Texture *skin, FTGLfont *font);

	};
};
