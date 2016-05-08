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

#include "ui/theme.h"

UITheme::UITheme (const std::string &fileName, ReadType rt, Texture *skin, FTGLfont *font) {

	this->skin = skin;
	this->font = font;

	skin->filter = GL_NEAREST;
	skin->wrapS  = GL_NONE;
	skin->wrapT  = GL_NONE;
	skin->buildMipmaps = false;

	skin->update();

	switch (rt) {

		case ReadType::Bin  : loadFromBin  (fileName.c_str()); break;
		case ReadType::Text : loadFromText (fileName.c_str()); break;
		default             : return;

	}

}
