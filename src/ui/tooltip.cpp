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

#include "ui/tooltip.h"

void UITooltip::render() {

	height = leftAria[3];

	int xOffset = abs(leftAria[0]-iOffsetsX[1]);
	int yOffset = abs(leftAria[1]-iOffsetsY[1]);
	int outWidth = abs(iWidths[0]+iWidths[1]+iWidths[2])-(xOffset+leftAria[2]);

	int twidth = ftglGetTextWidth (manager->theme->font, hint);
	width = twidth+xOffset+outWidth+textOffsets[0];

	int x = target->absLeft-width;
	int y = target->absTop-yOffset-(height >> 1)+(target->height >> 1)-1;
	int tx = x+xOffset+textOffsets[0];
	int ty = y+yOffset+(height >> 1)+textOffsets[1];

	renderPartsElementH (0, 1, 2, leftElement.get(), middleElement.get(), rightElement.get(), x, y, width);

	manager->atlasShader->unbind();
	renderText (manager->theme->font, &(textColors[0]), tx, ty, hint);
	manager->atlasShader->bind();

}
