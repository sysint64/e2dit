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
 * Author: Kabylin Andrey <andrey@kabylin.ru
 */

#include "ui/menuitems.h"
#include "utility/renderer.h"

void UIMenuItem::render() {

	if (menu && menu->visible)
		enter = true;

	UIButton::render();

	if (shortKey.size() != 0) {

		std::string text = wstr2str (shortKey);

		if (menu == nullptr) renderText (Align::Right, text, shortKey.size());
		else                 renderText (Align::Right, text, shortKey.size(), 12);

	}

}

void UITallMenuItem::render() {

	UIButton::render();

	int n  = 0; int tn = 0; int to = 0;
	if (enter) { n = 3; tn = 1; to = 2; }

	manager->atlasShader->unbind();

	/* Render Text */

	::renderText (manager->theme->font, &textColors[tn*3], absLeft+40, absTop+14, caption);
	::renderText (manager->theme->font, &descColors[tn*3], absLeft+40, absTop+30, desc);

	/* Render Image */

	manager->atlasMaskShader->bind();

	glActiveTexture   (GL_TEXTURE4);
	glBindTexture     (GL_TEXTURE_2D, texture->handle);

	glUniform1i       (manager->atlasMaskShader->locations["Texture"], 4);
	renderMaskElement (12, 14, absLeft+2, absTop+2, iWidths[13], iHeights[13], maskElement.get());

	manager->atlasShader->bind();

	/* Render Border */

	renderElement (13, absLeft+2, absTop+2, iWidths[13], iHeights[13], maskElement.get());

}
