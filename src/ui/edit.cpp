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

void UIEdit::render () {
	
	UIElement::render();

	cWidth = width-iWidths[3]-1;

	if (firstRender) {

		//stickPxPos  = iWidths[9];
		textPosX    = left+iWidths[12]+iWidths[12];
		firstRender = false;

	}

	textPosX   = left+iWidths[12]+iWidths[12];
	textPosY   = app->windowHeight-top-(iHeights[1] >> 1)-4;
	textPosX  += textOffset;

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

	if (stickPxPos+textPosX > left+cWidth-iWidths[11]-iWidths[12])
		textOffset -= (stickPxPos+textPosX)-(left+cWidth-iWidths[11]-iWidths[12]);

	if (stickPxPos+textPosX < left+iWidths[12])
		textOffset += (left+iWidths[12])-(stickPxPos+textPosX);

	/* Render Stick */

	if (showStick && focused) {
		
		glUniformMatrix4fv (manager->atlasShader->locations["MVP"]   , 1, GL_FALSE, &(stickElement->MVPMatrix[0][0]));
		glUniform2f        (manager->atlasShader->locations["Size"]  , fWidths [12], fHeights[12]);
		glUniform2f        (manager->atlasShader->locations["Offset"], offsetsX[12], offsetsY[12]);
		
		stickElement->setPosition (glm::vec2 (textPosX+stickPxPos, app->windowHeight-iHeights[12]-top));
		stickElement->setScale    (glm::vec2 (iWidths[12], iHeights[12]));
		stickElement->render();
		
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

		glUniformMatrix4fv (manager->colorShader->locations["MVP"]  , 1, GL_FALSE, &(selectElement->MVPMatrix[0][0]));
		glUniform4fv       (manager->colorShader->locations["Color"], 1, &selectColor[0]); 

		selectElement->setPosition (glm::vec2 (selx1-1, app->windowHeight-top+selectOffset[0]-iHeights[9]));
		selectElement->setScale    (glm::vec2 (selx2-selx1+1, iHeights[9]-selectOffset[1]-selectOffset[0]));
		selectElement->render();

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

	int poffset = 0; int aleft = left;

	/* Some Fix for certain Draw Align */

	if (drawAlign == Align::Left)   aleft  += iWidths[n]+iWidths[n+2];
	if (drawAlign == Align::Center) cWidth -= iWidths[n]-iWidths[n]-iWidths[n+2]-iWidths[n+2];

	/* Draw Left Element */

	if (drawAlign == Align::Left || drawAlign == Align::All) {

		glUniformMatrix4fv (manager->atlasShader->locations["MVP"]   , 1, GL_FALSE, &(leftElement->MVPMatrix[0][0]));
		glUniform2f        (manager->atlasShader->locations["Size"]  , fWidths [n], fHeights[n]);
		glUniform2f        (manager->atlasShader->locations["Offset"], offsetsX[n], offsetsY[n]);
		
		leftElement->setPosition (glm::vec2 (aleft, app->windowHeight-iHeights[n]-top));
		leftElement->setScale    (glm::vec2 (iWidths[n], iHeights[n]));
		leftElement->render();

	}

	/* Draw Middle Element */

	glUniformMatrix4fv (manager->atlasShader->locations["MVP"]   , 1, GL_FALSE, &(middleElement->MVPMatrix[0][0]));
	glUniform2f        (manager->atlasShader->locations["Size"]  , fWidths [n+1], fHeights[n+1]);
	glUniform2f        (manager->atlasShader->locations["Offset"], offsetsX[n+1], offsetsY[n+1]);

	//if (dynamic_cast<UICheckButton*> (this) && parent != 0)
	//	poffset = 1;

	middleElement->setPosition (glm::vec2 (aleft  +iWidths[n+1]-poffset, app->windowHeight-iHeights[n+1]-top));
	middleElement->setScale    (glm::vec2 (cWidth-iWidths[n+2]+poffset, iHeights[n+1]));
	middleElement->render();

	/* Draw Right Element */

	if (drawAlign == Align::Right || drawAlign == Align::All) {

		glUniformMatrix4fv (manager->atlasShader->locations["MVP"]   , 1, GL_FALSE, &(rightElement->MVPMatrix[0][0]));
		glUniform2f        (manager->atlasShader->locations["Size"]  , fWidths [n+2], fHeights[n+2]);
		glUniform2f        (manager->atlasShader->locations["Offset"], offsetsX[n+2], offsetsY[n+2]);
		
		rightElement->setPosition (glm::vec2 (aleft+cWidth , app->windowHeight-iHeights[n+2]-top));
		rightElement->setScale    (glm::vec2 (iWidths[n+2], iHeights[n+2]));
		rightElement->render();

	}

	/* Draw Focused */

	if (!focused)
		return;

	n = 9;

	/* Draw Left Element */

	if (drawAlign == Align::Left || drawAlign == Align::All) {

		glUniformMatrix4fv (manager->atlasShader->locations["MVP"]   , 1, GL_FALSE, &(leftElement->MVPMatrix[0][0]));
		glUniform2f        (manager->atlasShader->locations["Size"]  , fWidths [n], fHeights [n]);
		glUniform2f        (manager->atlasShader->locations["Offset"], offsetsX[n], offsetsY[n]);
		
		leftElement->setPosition (glm::vec2 (aleft, app->windowHeight-iHeights[n]-top));
		leftElement->setScale    (glm::vec2 (iWidths[n], iHeights[n]));
		leftElement->render();

	}

	/* Draw Middle Element */

	glUniformMatrix4fv (manager->atlasShader->locations["MVP"]   , 1, GL_FALSE, &(middleElement->MVPMatrix[0][0]));
	glUniform2f        (manager->atlasShader->locations["Size"]  , fWidths [n+1], fHeights[n+1]);
	glUniform2f        (manager->atlasShader->locations["Offset"], offsetsX[n+1], offsetsY[n+1]);

	middleElement->setPosition (glm::vec2 (aleft  +iWidths[n+1]-poffset, app->windowHeight-iHeights[n+1]-top));
	middleElement->setScale    (glm::vec2 (cWidth-iWidths[n+2]+poffset, iHeights[n+1]));
	middleElement->render();

	/* Draw Right Element */

	if (drawAlign == Align::Right || drawAlign == Align::All) {

		glUniformMatrix4fv (manager->atlasShader->locations["MVP"]   , 1, GL_FALSE, &(rightElement->MVPMatrix[0][0]));
		glUniform2f        (manager->atlasShader->locations["Size"]  , fWidths [n+2], fHeights[n+2]);
		glUniform2f        (manager->atlasShader->locations["Offset"], offsetsX[n+2], offsetsY[n+2]);
		
		rightElement->setPosition (glm::vec2 (aleft+cWidth , app->windowHeight-iHeights[n+2]-top));
		rightElement->setScale    (glm::vec2 (iWidths[n+2], iHeights[n+2]));
		rightElement->render();

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

/* Events */

void UIEdit::mouseDown (int x, int y, int button) {

	if (!enter || !focused)
		return;

	setStickPos (x, y);
	selStart = selEnd = stickChPos;

}

void UIEdit::mouseUp (int x, int y, int button) {

	if (enter) {

		showStick = true;
		stickTime = 0;

		if (focused)
			return;
		
		focus();

		lastStickChPos = stickChPos;
		stickChPos = text.size();
		
		selStart = 0;
		selEnd   = stickChPos;

	}

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

	int selMin = min (selStart, selEnd);
	int selMax = max (selStart, selEnd);

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

	int selMin = min (selStart, selEnd);
	int selMax = max (selStart, selEnd);

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
