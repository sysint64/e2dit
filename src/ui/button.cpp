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
#include "ui/grouped.h"

#include "utility/config.h"
#include "utility/renderer.h"

/**
 * Render Button
 */

void UIButton::render() {

	if (drawChilds)
		UIElement::render();

	updateAbsPos();

	if (tooltip)
		tooltip->setHidden (!enter);

	//if (tooltip) tooltip->render();

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

//			if (drawAlign == Align::Center) iOffset += iWidths[n]; else
//			if (drawAlign == Align::Right ) iOffset += iWidths[n];

			manager->icons->render (absLeft+iconLeft+iOffset, app->windowHeight-height-iconTop+2-absTop,
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

			manager->icons->render (absLeft+iconLeft+18+iOffset, app->windowHeight-height-iconTop+2-absTop,
			                        icon2Offset[0], icon2Offset[1], iconElement2.get());

		}

		/* Update Text Offset (absLeft padding) */

		toffset = 38;
	}

	if (!noRenderText)
		renderText (textAlign, caption, toffset);

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

void UIButton::renderText (const Align align, const std::wstring &text, const int offset) {

	/* Tables Indices */

	int n = 0; int tn = 0; int to = 0;

	if (enter) { n = 3; tn = 1; to = 2; }
	if (click || checked) { n = 6; tn = 2; to = 4; }

	/* Render Text */

	int textPosX;
	int textPosY = absTop+(height >> 1)+(manager->theme->fontHeight >> 1)-2-textOffsets[to+1];
	int twidth   = ftglGetTextWidth (manager->theme->font, text);

	int additionalOffset = 0;

	if (showIcon)  additionalOffset += 9; // TODO: remove hardcode
	if (showIcon2) additionalOffset += 9; //:

	switch (align) {

		case Align::Left:
			textPosX = iWidths[n]+paddingLeft+offset;
			break;

		case Align::Center:
			textPosX = (width-twidth)/2+additionalOffset;
			break;

		case Align::Right:
			textPosX = width-twidth-iWidths[n+2]-paddingRight-offset;
			break;

		default:
			textPosX = iWidths[n]+paddingLeft;

	}

	manager->atlasShader->unbind();
	::renderText (manager->theme->font, &textColors[n], absLeft+textPosX, textPosY, text);
	manager->atlasShader->bind();

}

void UIButton::mouseDown (int x, int y, int button) {

	UIElement::mouseDown (x, y, button);

	if (!allowCheck)
		return;

	UICheckedElements *grouped = dynamic_cast<UICheckedElements*>(parent);

	if (grouped && !grouped->multiSelect) {

		if (!enter)
			return;

		grouped->checkElement (this);

		if (onClick != nullptr)
			onClick (this);

	} else if (enter) checked = !checked;

}

void UIButton::mouseUp (int x, int y, int button) {

	UIElement::mouseUp (x, y, button);

	if (allowCheck || !enter)
		return;

	if (onClick != nullptr)
		onClick (this);

}
