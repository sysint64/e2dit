/* E2DIT - 2D Map Editor for game
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author: Kabylin Andrey <andrey@kabylin.ru>
 */

/** @file texture.cpp
    @brief Textures classes
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
