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

#include "ui/image.h"

void UIImage::loadImage (const std::string &fileName) {

	texture = std::make_unique<Texture> (fileName.c_str());

	width  = texture->width;
	height = texture->height;

}

void UIImage::cropImage (int ox, int oy, int cw, int ch) {

	float tfw = static_cast<float>(texture->width );
	float tfh = static_cast<float>(texture->height);

	offsetsX[0] = static_cast<float>(ox) / tfw;  fWidths [0] = static_cast<float>(cw) / tfw;
	offsetsY[0] = static_cast<float>(oy) / tfh;  fHeights[0] = static_cast<float>(ch) / tfh;

	width  = cw;
	height = ch;

}

void UIImage::render() {

	updateAbsPos();

	glActiveTexture (GL_TEXTURE4);
	glBindTexture   (GL_TEXTURE_2D, texture->handle);

	glUniform1i   (manager->atlasShader->locations["Texture"], 4);
	renderElement (0, absLeft, absTop, width, height, imageElement.get());
	glUniform1i   (manager->atlasShader->locations["Texture"], manager->themeTexID);

}
