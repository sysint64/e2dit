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
#include "utility/math.h"

void UITooltip::render() {

	if (hidden && math::feq<float>(alpha, 0.f, 0.01f))
		return;

	float targetAlpha = hidden ? 0.f : 1.f;

	time += app->deltaTime;

	if (time >= 1.f/2000.f && !hidden)
		alpha = math::interpTo (alpha, targetAlpha, app->deltaTime, 15000.f);
	else if (hidden) {
		alpha = math::interpTo (alpha, targetAlpha, app->deltaTime, 15000.f);
	}

	height = leftAria[3];
	over = false;

	int xOffset, yOffset, outWidth;

	int x, y, n;
	int txo = 0;
	int tyo = 0;

	switch (align) {

		case Align::Left:

			xOffset  = abs(leftAria[0]-iOffsetsX[1]);
			yOffset  = abs(leftAria[1]-iOffsetsY[1]);
			outWidth = abs(iWidths[0]+iWidths[1]+iWidths[2])-(xOffset+leftAria[2]);

			x = target->absLeft-width+leftOffset[0];
			y = target->absTop-yOffset-(height >> 1)+(target->height >> 1)+leftOffset[1];
			n = 0;

			break;

		case Align::Right:

			xOffset  = abs(rightAria[0]-iOffsetsX[3]);
			yOffset  = abs(rightAria[1]-iOffsetsY[3]);
			outWidth = abs(iWidths[3]+iWidths[4]+iWidths[5])-(xOffset+rightAria[2]);

			x = target->absLeft+target->width+rightOffset[0];
			y = target->absTop-yOffset-(height >> 1)+(target->height >> 1)+rightOffset[1];
			n = 3;

			break;

		default:
			return;

	}

	glUniform1f (manager->atlasShader->locations["Alpha"], alpha);

	int twidth = ftglGetTextWidth (manager->theme->font14, hint);
	width = twidth+xOffset+outWidth+textOffsets[0]+10; // TODO: Remove hardcode - 10

	int tx = x+xOffset+textOffsets[0];
	int ty = y+yOffset+(height >> 1)+textOffsets[1];

	renderPartsElementH (n, n+1, n+2, leftElement.get(), middleElement.get(), rightElement.get(), x, y, width);

	manager->atlasShader->unbind();
	renderText (manager->theme->font14, &(textColors[0]), alpha, tx, ty, hint);
	manager->atlasShader->bind();

}
