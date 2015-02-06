/*
 * This file is part of E2DIT-GAPI.

 * E2DIT-GAPI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.

 * E2DIT-GAPI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with E2DIT-GAPI. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Kabylin Andrey <andrey@kabylin.ru>
 */

#include "texture.h"

Texture::Texture (const char *fileName, GLuint filter, GLuint wrapS, GLuint wrapT)
		: filter (filter), wrapS (wrapS), wrapT (wrapT)
{

	glGenTextures (1, &handle);

	/* if Not Exist */

	if (!fs::exists (fileName)) {

		Application::getInstance()->log.write ("Load texture error : %s", fileName);
		loaded = false;
		return;

	}

	/* Load texture */

	data = SOIL_load_image (fileName, &width, &height, nullptr, SOIL_LOAD_RGBA);
	update();

	fs::path p (fileName);
	this->fileName = p.filename().string();

}

Texture::~Texture() {

	glDeleteTextures     (1, &handle);
	SOIL_free_image_data (data);

}

void Texture::bind() const {

	glBindTexture (GL_TEXTURE_2D, handle);

}

void Texture::unbind() const {

	glBindTexture (GL_TEXTURE_2D, 0);

}

void Texture::update() const {

	glBindTexture   (GL_TEXTURE_2D, handle);
	glTexImage2D    (GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     wrapS);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     wrapT);

	if (buildMipmaps) {

		gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

	}

}
