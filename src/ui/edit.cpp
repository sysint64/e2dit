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

#include "ui/edit.h"

/**
 * Render Editable Field
 */

void UIEdit::render() {

	UIElement::render();
	updateAbsPos();

	cWidth = width-iWidths[3]-1;

	if (firstRender) {

		//stickPxPos  = iWidths[9];
		textPosX    = absLeft+iWidths[12]+iWidths[12];
		firstRender = false;

	}

	textPosX   = absLeft+iWidths[12]+iWidths[12];
	textPosY   = app->windowHeight-absTop-(iHeights[1] >> 1)-4;
	textPosX  += textOffset;

	if (trackMode) { /* Center text */
		textPosX = absLeft;
		int twidth;

		if (!focused) twidth = ftglGetTextWidth (manager->theme->font, before+text+after);
		else          twidth = ftglGetTextWidth (manager->theme->font, text);

		textPosX += (cWidth-twidth) >> 1;
	}

	if (drawAlign == Align::Left)
		textPosX += iWidths[12]+3;

	/* Blink Stick */

	if (focused) stickTime += app->deltaTime;
	if (stickTime > 0.0005f) {

		stickTime = 0;
		showStick = !showStick;

	}

	/* Render Skin */

	if (!withoutSkin) renderSkin();

	int twidth = ftglGetTextWidth (manager->theme->font, text.substr(0, stickChPos));
	stickPxPos = twidth;

	/* Scroll Text */

	if (stickPxPos+textPosX > absLeft+cWidth-iWidths[11]-iWidths[12])
		textOffset -= (stickPxPos+textPosX)-(absLeft+cWidth-iWidths[11]-iWidths[12]);

	if (stickPxPos+textPosX < absLeft+iWidths[12])
		textOffset += (absLeft+iWidths[12])-(stickPxPos+textPosX);

	/* Render Stick */

	if (showStick && focused)
		renderElement (12, textPosX+stickPxPos-1, absTop, iWidths[12], iHeights[12], stickElement.get());

	/* Render track arrows */

	if (trackMode) {
		int n = enter ? 1 : 0;
		int x;
		int y = absTop +iHeights[13+n]-(iHeights[13+n] >> 1);

		/* Left arrow */

		if (drawAlign == Align::All || drawAlign == Align::Left)
			x = absLeft+iWidths[13+n];

		if (drawAlign == Align::Right || drawAlign == Align::Center)
			x = absLeft+iWidths[13+n]+iWidths[0];

		renderElement (13+n, x, y, iWidths[13+n], iHeights[13+n], leftArrowElement.get());

		/* Right arrow */

		if (drawAlign == Align::All || drawAlign == Align::Left)
			x = absLeft+width-(iWidths[15+n] << 1)-1;

		if (drawAlign == Align::Right || drawAlign == Align::Center)
			x = absLeft+width-(iWidths[15+n] << 1)-iWidths[11]-1;

		renderElement (15+n, x, y, iWidths[15+n], iHeights[15+n], rightArrowElement.get());

	}

	/* Render Text */

	std::string s  =     wstr2str (text);
	std::string at = " "+wstr2str (after);
	std::string bt =     wstr2str (before)+" ";

	if (!focused)
		s = bt+s+at;

	manager->atlasShader->unbind();

	/* Draw Select Rect */

	if (selStart != selEnd && focused) {

		int twidth = ftglGetTextWidth (manager->theme->font, text.substr (0, selStart));
		int selx1, selx2;

		selx1  = textPosX+twidth+(iWidths[12] >> 1);
		twidth = ftglGetTextWidth(manager->theme->font, text.substr (0, selEnd));
		selx2  = textPosX+twidth+(iWidths[12] >> 1);

		if (selx1 > selx2) { // Swap

			int tmp = selx1;
			selx1   = selx2;
			selx2   = tmp;

		}

		/* Draw Quad */

		manager->colorShader->bind();

		renderColorElement (selx1-2, absTop+selectOffset[0], // x, y
							selx2-selx1+1, iHeights[9]-selectOffset[1]-selectOffset[0], // w, h
							selectElement.get(), selectColor);

		manager->colorShader->unbind();

	}

	/* Tables Indices */

	int n  = 0; int tn = 0; int to = 0;

	if (enter) { n = 3; tn = 1; to = 2; }
	if (click) { n = 6; tn = 2; to = 4; }

	/* Render */

	glBegin2D();

	glActiveTexture (GL_TEXTURE0);
	glColor3f       (textColors[n], textColors[n+1], textColors[n+2]);

	glTranslatef    (textPosX+textOffsets[to]/*+textOffset*/, textPosY+textOffsets[to+1], 0);
	ftglRenderFont  (manager->theme->font, s.c_str(), RENDER_ALL);

	glEnd2D();

	manager->atlasShader->bind();

}

/**
 * Render Button Skin
 */

void UIEdit::renderSkin() {

	/* Tables Indices */

	int n  = 0; int tn = 0; int to = 0;

	if (enter) { n = 3; tn = 1; to = 2; }
	if (click) { n = 6; tn = 2; to = 4; }


	renderPartsElementH (n, n+1, n+2, leftElement.get(), middleElement.get(), rightElement.get(), absLeft, absTop, width);

	if (focused) {

		n = 9;
		renderPartsElementH (n, n+1, n+2, leftElement.get(), middleElement.get(), rightElement.get(), absLeft, absTop, width);

	}

}

void UIEdit::updateSelect() {

	if (pressed (keyShift)) {

		if (selStart == selEnd)
			selStart = lastStickChPos;

		selEnd = stickChPos;

	} else {

		selEnd = selStart = stickChPos;

	}

}

/**
 * Set Slick Position by Cursor Coordinate
 *
 * @param x: Cursor Position X
 * @param y: Cursor Position Y
 */

void UIEdit::setStickPos (int x, int y) {

	int bmax   = x-textPosX; // Get max text width from begin string to Mouse X
	int twidth = ftglGetTextWidth (manager->theme->font, text);

	showStick = true;
	stickTime = 0;

	if (x > textPosX+twidth) {

		stickChPos = text.size();
		return;

	}

	/**
	 * Find optimal position
	 * Traverse all sizes of slice of text from [0..1, 0..2, ... , 0..text.size()-1]
	 * And get max size with condition: size <= bmax
	 */

	for (int i = 0; i < text.size(); i++) {

		std::wstring slice        = text.substr (0, i);
		std::string  decodedSlice = wstr2str (slice);
		int          sliceLength  = slice.size();

		float bounds[6];

		ftglGetFontBBox (manager->theme->font, decodedSlice.c_str(), sliceLength, bounds);

		if (text[sliceLength-1] == ' ') bounds[3] += 3; // FIXME: Why 3 ?
		if (bounds[3]-bounds[0] > bmax) {

			stickChPos = i;
			break;

		}

	}

}

void UIEdit::setCursor() {

}

/* Events */

void UIEdit::mouseDown (int x, int y, int button) {

	if (!enter || !focused)
		return;

	setStickPos (x, y);
	selStart = selEnd = stickChPos;

}

void UIEdit::addVal(const int acc) {

	int val = std::stoi(text);
	val += acc;
	math::clamp (&val, trackRange[0], trackRange[1]);
	text = std::to_wstring(val);

}

void UIEdit::focus() {

	UIElement::focus();

	lastStickChPos = stickChPos;
	stickChPos = text.size();

	selStart = 0;
	selEnd   = stickChPos;

}

void UIEdit::mouseUp (int x, int y, int button) {

	if (!enter)
		return;

	if (trackMode && pointInRect(x, y, absLeft, absTop, 14, 20)) {

		addVal(-1);
		focus();

		if (onChange)
			onChange(this);

		return;

	} else

	if (trackMode && pointInRect(x, y, absLeft+width-14, absTop, 14, 20)) {

		addVal(1);
		focus();

		if (onChange)
			onChange(this);

		return;

	}

	showStick = true;
	stickTime = 0;

	/*if (focused)
		return;

	focus();*/

}

void UIEdit::mouseMove (int x, int y, int button) {

	if (!enter || button != mouseLeft)
		return;

	setStickPos (x, y);
	selEnd = stickChPos;

}

void UIEdit::dblClick (int x, int y, int button) {

	if (!enter || button != mouseLeft)
		return;

	int start = stickChPos;

	if (stickChPos == text.size()) start = stickChPos-1;

	for (int i = start; i <= text.size(); i++) {

		selEnd = i;
		bool next = true;

		for (int j = 0; j < countof (splitChars); j++) {

			if (text[i] == splitChars[j]) {

				next = false;
				break;

			}

		}

		if (!next) {

			selEnd = i;
			break;

		}

	}

	for (int i = start; i >= 0; i--) {

		selStart = i;
		bool next = true;

		for (int j = 0; j < countof (splitChars); j++) {

			if (text[i] == splitChars[j]) {

				next = false;
				break;

			}

		}

		if (!next) {

			selStart = i+1;
			break;

		}

	}

	if (selStart == text.size()) selStart = text.size()-1;
	stickChPos = selEnd;

}

void UIEdit::keyPressed (int key) {

	if (!focused)
		return;

	showStick = true;
	stickTime = 0;

	/* Find Start & End Select Char Poses */

	int selMin = math::min (selStart, selEnd);
	int selMax = math::max (selStart, selEnd);

	/* Split text parts */

	std::wstring splitLeft;
	std::wstring splitRight;

	/* Clipboard */

	switch (key) {

		/* Clipboard */

		case keyC : if (pressed (keyCtrl)) copyText();  break;
		case keyV : if (pressed (keyCtrl)) pasteText(); break;

		/* Erase */

		case keyDelete :

			if (selStart == selEnd) {

				if (stickChPos >= text.size())
					break;

				splitLeft  = text.substr (0           , stickChPos );
				splitRight = text.substr (stickChPos+1, text.size());

			} else {

				splitLeft  = text.substr (0       , selMin);
				splitRight = text.substr (selMax+1, text.size());

				lastStickChPos = stickChPos;
				stickChPos     = selEnd = selStart = selMin;

			}

			text = splitLeft+splitRight;
			break;

		case keyBackspace :

			if (selStart == selEnd) {

				if (stickChPos == 0)
					break;

				splitLeft  = text.substr (0         , stickChPos-1);
				splitRight = text.substr (stickChPos, text.size() );

				lastStickChPos = stickChPos;
				stickChPos--;

			} else {

				splitLeft  = text.substr (0       , selMin);
				splitRight = text.substr (selMax+1, text.size());

				lastStickChPos = stickChPos;
				stickChPos     = selEnd = selStart = selMin;

			}

			text = splitLeft+splitRight;
			break;

		/* Navigate */

		case keyLeftArrow :

			if (stickChPos == 0)
				break;

			lastStickChPos = stickChPos;
			stickChPos--;

			updateSelect();
			break;

		case keyRightArrow :

			if (stickChPos == text.size())
				break;

			lastStickChPos = stickChPos;
			stickChPos++;

			updateSelect();
			break;

		case keyHome :

			lastStickChPos = stickChPos;
			stickChPos     = 0;

			updateSelect();
			break;

		case keyEnd :

			lastStickChPos = stickChPos;
			stickChPos     = text.size();

			updateSelect();
			break;

	}

}

void UIEdit::textEntered (int key) {

	if (!focused)
		return;

	showStick = true;
	stickTime = 0;

	wchar_t putChar = key;

	/* Skip Special Chars */

	unsigned char b1 = key;
	unsigned char b2 = key >> 8;

	if (!(((b2 != 0) || (b1 >= 0x20)) && (b1 <= 0x7E))) // LOL
		return;

	/* Split text parts */

	std::wstring splitLeft;
	std::wstring splitRight;

	/* Find Start & End Select Char Poses */

	int selMin = math::min (selStart, selEnd);
	int selMax = math::max (selStart, selEnd);

	if (selStart == selEnd) {

		splitLeft  = text.substr (0, stickChPos);
		splitRight = text.substr (stickChPos, text.size());

	} else {

		splitLeft  = text.substr (0, selMin);
		splitRight = text.substr (selMax, text.size());

		lastStickChPos = stickChPos;
		stickChPos     = selEnd = selStart = selMin;

	}

	text  = splitLeft;
	text += putChar;
	text += splitRight;

	lastStickChPos = stickChPos;
	stickChPos++;

	if (onTextEntered != nullptr)
		onTextEntered (this, key);

	if (onChange != nullptr)
		onChange (this);

}

/* Work With Text */

void UIEdit::copyText() {

}

void UIEdit::pasteText() {

}

float UIEdit::parseExpr() {
	return 0;
}
