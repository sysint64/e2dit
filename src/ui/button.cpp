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

/**
 * Render Button
 *
 * @param x: Left Position
 * @param y: Top  Position
 */

void UIButton::render () {

	UIElement::render();

	//if (!enabled || (parent != nullptr && !parent->enabled))
	//	glUniform1f (manager->atlasShader->locations["Alpha"], manager->disabledAlpha);

	/* Tables Indices */
	int n  = 0; int tn = 0; int to = 0;

	if (enter) { n = 3; tn = 1; to = 2; }
	if (click) { n = 6; tn = 2; to = 4; }

	if (!withoutSkin) renderSkin();

	//if ( withoutSkin && !enter && !click)
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

			glActiveTexture (GL_TEXTURE2);
			glBindTexture   (GL_TEXTURE_2D, manager->icons.tex->handle);
			glUniform1i     (manager->atlasShader->locations["Texture"], 2);
			//
			glUniformMatrix4fv (manager->atlasShader->locations["MVP"]   , 1, GL_FALSE, &(iconElement->MVPMatrix[0][0]));
			glUniform2f        (manager->atlasShader->locations["Size"]  , manager->icons.width, manager->icons.height);
			glUniform2f        (manager->atlasShader->locations["Offset"], manager->icons.offsetsX[io[0]][io[1]],
																		   manager->icons.offsetsY[io[0]][io[1]]);
			//
			int iOffset = icoOffset;
			
			if (drawAlign == Align::Center) iOffset += iWidths[n]; else
			if (drawAlign == Align::Right ) iOffset += iWidths[n];

			iconElement->setPosition (glm::vec2 (left+2+iOffset, app->windowHeight-height+2-top));
			iconElement->setScale    (glm::vec2 (manager->icons.sizeIcon, manager->icons.sizeIcon));
			iconElement->render();

			glUniform1i (manager->atlasShader->locations["Texture"], manager->themeTexID);

		}

		/* Update Text Offset (left padding) */

		toffset = 18;
	}

	/* Show Second Icon */

	if (showIcon2) {
		
		if (icon2Offset[0] >= 0 && icon2Offset[1] >= 0) {

			glActiveTexture (GL_TEXTURE2);
			glBindTexture   (GL_TEXTURE_2D, manager->icons.tex->handle);
			glUniform1i     (manager->atlasShader->locations["Texture"], 2);
			
			glUniformMatrix4fv (manager->atlasShader->locations["MVP"], 1, GL_FALSE, &(iconElement2->MVPMatrix[0][0]));
			glUniform2f        (manager->atlasShader->locations["Size"]  , manager->icons.width, manager->icons.height);
			glUniform2f        (manager->atlasShader->locations["Offset"], manager->icons.offsetsX[icon2Offset[0]][icon2Offset[1]],
																		   manager->icons.offsetsY[icon2Offset[0]][icon2Offset[1]]);
			
			int iOffset = 0;
			
			if (drawAlign == Align::Center) iOffset += iWidths[n]; else
			if (drawAlign == Align::Right ) iOffset += iWidths[n];

			iconElement2->setPosition (glm::vec2 (left+20+iOffset, app->windowHeight-height+2-top));
			iconElement2->setScale    (glm::vec2 (manager->icons.sizeIcon, manager->icons.sizeIcon));
			iconElement2->render();

			glUniform1i (manager->atlasShader->locations["Texture"], manager->themeTexID);

		}

		/* Update Text Offset (left padding) */
		
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

	width = 100;
	int n  = 0; int tn = 0; int to = 0;

	if (enter) { n = 3; tn = 1; to = 2; }
	if (click) { n = 6; tn = 2; to = 4; }

	int cWidth = width-iWidths[n]-1;
	int poffset = 0;

	/* Some Fix for certain Draw Align */

	if (drawAlign == Align::Left)   left   += iWidths[n]+iWidths[n+2];
	if (drawAlign == Align::Center) cWidth -= iWidths[n]-iWidths[n]-iWidths[n+2]-iWidths[n+2];

	/* Draw Left Element */

	if (drawAlign == Align::Left || drawAlign == Align::All) {

		glUniformMatrix4fv (manager->atlasShader->locations["MVP"]   , 1, GL_FALSE, &(leftElement->MVPMatrix[0][0]));
		glUniform2f        (manager->atlasShader->locations["Size"]  , fWidths [n], fHeights [n]);
		glUniform2f        (manager->atlasShader->locations["Offset"], offsetsX[n], offsetsY[n]);
		
		leftElement->setPosition (glm::vec2 (left, app->windowHeight-iHeights[n]-top));
		leftElement->setScale    (glm::vec2 (iWidths[n], iHeights[n]));
		leftElement->render();

	}

	/* Draw Middle Element */

	glUniformMatrix4fv (manager->atlasShader->locations["MVP"]   , 1, GL_FALSE, &(middleElement->MVPMatrix[0][0]));
	glUniform2f        (manager->atlasShader->locations["Size"]  , fWidths [n+1], fHeights[n+1]);
	glUniform2f        (manager->atlasShader->locations["Offset"], offsetsX[n+1], offsetsY[n+1]);

	//if (dynamic_cast<UICheckButton*> (this) && parent != 0)
	//	poffset = 1;

	middleElement->setPosition (glm::vec2 (left  +iWidths[n+1]-poffset, app->windowHeight-iHeights[n+1]-top));
	middleElement->setScale    (glm::vec2 (cWidth-iWidths[n+2]+poffset, iHeights[n+1]));
	middleElement->render();

	/* Draw Right Element */

	if (drawAlign == Align::Right || drawAlign == Align::All) {

		glUniformMatrix4fv (manager->atlasShader->locations["MVP"]   , 1, GL_FALSE, &(rightElement->MVPMatrix[0][0]));
		glUniform2f        (manager->atlasShader->locations["Size"]  , fWidths [n+2], fHeights[n+2]);
		glUniform2f        (manager->atlasShader->locations["Offset"], offsetsX[n+2], offsetsY[n+2]);
		
		rightElement->setPosition (glm::vec2 (left+cWidth , app->windowHeight-iHeights[n+2]-top));
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
		
		leftElement->setPosition (glm::vec2 (left, app->windowHeight-iHeights[n]-top));
		leftElement->setScale    (glm::vec2 (iWidths[n], iHeights[n]));
		leftElement->render();

	}

	/* Draw Middle Element */

	glUniformMatrix4fv (manager->atlasShader->locations["MVP"]   , 1, GL_FALSE, &(middleElement->MVPMatrix[0][0]));
	glUniform2f        (manager->atlasShader->locations["Size"]  , fWidths [n+1], fHeights[n+1]);
	glUniform2f        (manager->atlasShader->locations["Offset"], offsetsX[n+1], offsetsY[n+1]);

	middleElement->setPosition (glm::vec2 (left  +iWidths[n+1]-poffset, app->windowHeight-iHeights[n+1]-top));
	middleElement->setScale    (glm::vec2 (cWidth-iWidths[n+2]+poffset, iHeights[n+1]));
	middleElement->render();

	/* Draw Right Element */

	if (drawAlign == Align::Right || drawAlign == Align::All) {

		glUniformMatrix4fv (manager->atlasShader->locations["MVP"]   , 1, GL_FALSE, &(rightElement->MVPMatrix[0][0]));
		glUniform2f        (manager->atlasShader->locations["Size"]  , fWidths [n+2], fHeights[n+2]);
		glUniform2f        (manager->atlasShader->locations["Offset"], offsetsX[n+2], offsetsY[n+2]);
		
		rightElement->setPosition (glm::vec2 (left+cWidth , app->windowHeight-iHeights[n+2]-top));
		rightElement->setScale    (glm::vec2 (iWidths[n+2], iHeights[n+2]));
		rightElement->render();

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
#include <iostream>
void UIButton::renderText (Align align, std::string text, int size, int offset) {

	/* Tables Indices */

	int n  = 0; int tn = 0; int to = 0;

	if (enter) { n = 3; tn = 1; to = 2; }
	if (click) { n = 6; tn = 2; to = 4; }

	// Draw Text
	float bounds[6];
	
	ftglGetFontBBox (manager->theme->font, text.c_str(), size, bounds);
	int twidth = bounds[3]-bounds[0];

	/* Calculate Text Left Position */

	int tx = left;

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

	glTranslatef    (tx+textOffsets[to], app->windowHeight-top-(height >> 1)-4+textOffsets[to+1], 0);
	ftglRenderFont  (manager->theme->font, text.c_str(), RENDER_ALL);

	glEnd2D();

	manager->atlasShader->bind();

}
