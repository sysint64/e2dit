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

#include "ui/label.h"

void UILabel::render() {

	updateAbsPos();
	height = manager->theme->fontHeight;
	int textPosX = absLeft+textOffsets[0];
	int twidth;

	switch (textAlign) {

		case Align::Center:
			twidth    = ftglGetTextWidth (manager->theme->font, caption);
			textPosX += (width-twidth) >> 1;
			break;

		case Align::Right:
			twidth    = ftglGetTextWidth (manager->theme->font, caption);
			textPosX += width-twidth;
			break;

	}

	manager->atlasShader->unbind();
	renderText (manager->theme->font, textColors, textPosX, absTop+textOffsets[1]+height, caption);
	manager->atlasShader->bind();

}
