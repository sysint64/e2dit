/*
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

/** @file texture.h
    @brief Textures classes
*/

#ifndef E2DIT_RENDERER_TEXTURE_H
#define E2DIT_RENDERER_TEXTURE_H

#include "GL/glew.h"
#include "../system/application.h"
#include <string>
#include <SOIL/SOIL.h>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

class Texture {
public:
	int      width;
	int      height;

	GLuint   handle;
	bool     loaded       = true;
	bool     buildMipmaps = false;
	GLuint   filter;
	GLuint   wrapS;
	GLuint   wrapT;

	unsigned char *data;
	std::string    fileName;

	 Texture (const char *fileName, GLuint filter = GL_LINEAR,
					GLuint wrapS  = GL_CLAMP_TO_EDGE,
					GLuint wrapT  = GL_CLAMP_TO_EDGE);
	~Texture ();

	void bind  () const;
	void unbind() const;
	void update() const;

};

#endif
