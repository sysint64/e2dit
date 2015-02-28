/*
 * E2DIT - is a 2D Map Editor to create a levels for 2d games
 * Copyright (C) 2015 Kabylin Andrey <andrey@kabylin.ru>

 * This file is part of E2DIT-GAPI.

 * E2DIT-GAPI is free library: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.

 * E2DIT-GAPI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with E2DIT-GAPI. If not, see <http://www.gnu.org/licenses/>.
 */

#include "renderer/texture.h"

/**
 * Create Texture
 *
 * @param fileName
 * @param filter
 * @param wrapS
 * @param wrapT
 */

Texture::Texture (const char *fileName, GLuint filter, GLuint wrapS, GLuint wrapT)
		: filter (filter), wrapS (wrapS), wrapT (wrapT)
{

	/* Create Texture buffer */

	glGenTextures (1, &handle);

	/* Check texture exist */

	if (!fs::exists (fileName)) {

		/* if not exist, then write error to log */

		Application::getInstance()->log.write ("Load texture error : %s", fileName);
		loaded = false;

		/* And Exit */

		return;

	}

	/* Load texture */

	data = SOIL_load_image (fileName, &width, &height, nullptr, SOIL_LOAD_RGBA);
	update();

	/* Save Texture path */

	fs::path p (fileName);
	this->fileName = p.filename().string();

}

/**
 * Destructor
 */

Texture::~Texture() {

	/* Destroy buffer & data */

	glDeleteTextures     (1, &handle);
	SOIL_free_image_data (data);

}

/**
 * Bind texture
 */

void Texture::bind() const {

	glBindTexture (GL_TEXTURE_2D, handle);

}

/**
 * Unbind texture
 */

void Texture::unbind() const {

	glBindTexture (GL_TEXTURE_2D, 0);

}

/**
 * Update Texture Parameters
 */

void Texture::update() const {

	/* Bind Textuew */

	glBindTexture   (GL_TEXTURE_2D, handle);
	glTexImage2D    (GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	/* Display Parameters */

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     wrapS);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     wrapT);

	/* Build Mipmaps if true */

	if (buildMipmaps) {

		gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

	}

}
