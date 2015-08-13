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

#include "ui/button.h"

#include "utility/config.h"
#include "utility/renderer.h"

/**
 * Render Button
 */

void UIButton::render () {

	UIElement::render();
	updateAbsPos();

	//if (!enabled || (parent != nullptr && !parent->enabled))
	//	glUniform1f (manager->atlasShader->locations["Alpha"], manager->disabledAlpha);

	/* Tables Indices */

	int n  = 0; int tn = 0; int to = 0;

	if (enter) { n = 3; tn = 1; to = 2; }
	if (click || checked) { n = 6; tn = 2; to = 4; }

	if (!withoutSkin) renderSkin();

	//if (withoutSkin && !enter && !click)
	//	glUniform1f (manager->atlasShader->locations["Alpha"], manager->disabledAlpha);

	/* Convert UTF String to bytes sequence */

	std::string text = wstr2str (caption);
	int toffset = 0;

	/* Show First Icon */

	if (showIcon) {

		int io[2];

		if (showClickIcon && click) {

			io[0] = clickIconOffset[0];
			io[1] = clickIconOffset[1];

		} else {

			io[0] = iconOffset[0];
			io[1] = iconOffset[1];

		}

		if (withoutSkin && !enabled) {

			io[0] = clickIconOffset[0];
			io[1] = clickIconOffset[1];

		} else {

			io[0] = iconOffset[0];
			io[1] = iconOffset[1];

		}

		if (io[0] >= 0 && io[1] >= 0) {

			int iOffset = icoOffset;

			if (drawAlign == Align::Center) iOffset += iWidths[n]; else
			if (drawAlign == Align::Right ) iOffset += iWidths[n];

			manager->icons->render (absLeft+2+iOffset, app->windowHeight-height+2-absTop,
			                        iconOffset[0], iconOffset[1], iconElement.get());

		}

		/* Update Text Offset (absLeft padding) */

		toffset = 18;
	}

	/* Show Second Icon */

	if (showIcon2) {

		if (icon2Offset[0] >= 0 && icon2Offset[1] >= 0) {

			int iOffset = 0;

			if (drawAlign == Align::Center) iOffset += iWidths[n]; else
			if (drawAlign == Align::Right ) iOffset += iWidths[n];

			manager->icons->render (absLeft+20+iOffset, app->windowHeight-height+2-absTop,
			                        icon2Offset[0], icon2Offset[1], iconElement2.get());

		}

		/* Update Text Offset (absLeft padding) */

		toffset = 38;
	}

	if (!noRenderText)
		renderText (textAlign, text, caption.size(), toffset);

	/* Reset Alpha */

	//if (!enabled || (parent != 0 && !parent->enabled))
	//	glUniform1f (manager->atlasShader->locations["Alpha"], 1.f);

}

/**
 * Render Button Skin
 */

void UIButton::renderSkin() {

	/* Tables Indices */

	int n  = 0; int tn = 0; int to = 0;

	if (enter) { n = 3; tn = 1; to = 2; }
	if (click || checked) { n = 6; tn = 2; to = 4; }

	renderPartsElementH (n, n+1, n+2, leftElement.get(), middleElement.get(), rightElement.get(), absLeft, absTop, width);

	if (focused) {

		n = 9;
		renderPartsElementH (n, n+1, n+2, leftElement.get(), middleElement.get(), rightElement.get(), absLeft+focusOffsets[0], absTop+focusOffsets[1], width+focusOffsets[2]);

	}

}

/**
 * Render Text
 *
 * @param align: Text Align
 * @param text: Rendered Text
 * @param size: Count of Chars
 * @param offset: Offset Render
 */

void UIButton::renderText (const Align align, const std::string &text, const int size, const int offset) {

	/* Tables Indices */

	int n = 0; int tn = 0; int to = 0;

	if (enter) { n = 3; tn = 1; to = 2; }
	if (click || checked) { n = 6; tn = 2; to = 4; }

	// Draw Text
	float bounds[6];

	ftglGetFontBBox (manager->theme->font, text.c_str(), size, bounds);
	int twidth = bounds[3]-bounds[0];

	/* Calculate Text Left Position */

	int tx = absLeft;

	/* Text align : Left */

	if (align == Align::Left) {

		tx += iWidths[n];

		if (drawAlign != Align::Left && drawAlign != Align::All) tx += iWidths[n+2];
		if (offset != 0) tx += offset;

	} else

	/* Text align : Right */

	if (align == Align::Right) {

		     if (drawAlign == Align::Center) tx += (width-twidth)-iWidths[n+2];
		else if (drawAlign == Align::Right ) tx += (width-twidth)-iWidths[n+2]-iWidths[n];
		else if (drawAlign == Align::Left  ) tx += (width-twidth)-iWidths[n+2]-iWidths[n]-iWidths[n];
		else                                 tx += (width-twidth)-iWidths[n+2]-textOffsets[to]-textOffsets[to];

		if (offset != 0) tx -= offset;

	} else

	/* Text align : Center */

	if (align == Align::Center) {

		     if (drawAlign == Align::Center) tx += (width+iWidths[n+2]+iWidths[n]-twidth) >> 1;
		else if (drawAlign == Align::Right ) tx += (width+iWidths[n+2]-twidth) >> 1;
		else if (drawAlign == Align::Left  ) tx += (width-iWidths[n+0]-twidth) >> 1;
		else                                 tx += (width-twidth) >> 1;

		if (offset != 0) tx += offset >> 1;

	}

	/* Render Text */

	manager->atlasShader->unbind();

	glBegin2D();

	glActiveTexture (GL_TEXTURE0);
	glColor3f       (textColors[n], textColors[n+1], textColors[n+2]);

	glTranslatef    (tx+textOffsets[to], app->windowHeight-absTop-(height >> 1)-4+textOffsets[to+1], 0);
	ftglRenderFont  (manager->theme->font, text.c_str(), RENDER_ALL);

	glEnd2D();

	manager->atlasShader->bind();

}

void UIButton::mouseDown (int x, int y, int button) {

	UIElement::mouseDown (x, y, button);

	if (!allowCheck) return;
	if (enter) checked = !checked;
	
}
