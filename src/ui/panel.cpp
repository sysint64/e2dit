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

#include "ui/panel.h"
#include <iostream>

/* Scroll */
/* By Pixel */

void UIPanel::addScrollXByPx (int pxVal) {

	hbOffset = hbOffset+pxVal;
	math::clamp (&hbOffset, 0, hbMax-hbSize);
	
	int py   =  (hbOffset*100)/hbMax;
	hsOffset =  (contentHeight*py)/100;

}

void UIPanel::addScrollYByPx (int pxVal) {

}

void UIPanel::setScrollYByPx (int pxVal) {

	vbOffset = pxVal;
	math::clamp (&vbOffset, 0, vbMax-vbSize);

	int py   =  (vbOffset*100)/vbMax;
	vsOffset =  (contentHeight*py)/100;

}

void UIPanel::setScrollXByPx (int pxVal) {

	hbOffset = pxVal;
	math::clamp (&hbOffset, 0, hbMax-hbSize);

	int py    = (hbOffset*100)/hbMax;
	hsOffset  = (contentWidth*py)/100;

}

/* By Percent */

void UIPanel::addScrollXByPct (int pctVal) {

}

void UIPanel::addScrollYByPct (int pctVal) {

}

void UIPanel::setScrollYByPct (int pctVal) {

}

void UIPanel::setScrollXByPct (int pctVal) {

}

/* Render Panel */

void UIPanel::render() {

	updateAbsPos();

	if (background != Background::Transparent) {

		/* Switch Shaders */

		manager->atlasShader->unbind();
		manager->colorShader->bind();

		/* Draw Quad */

		float *color;

		switch (background) {

			case Background::Light  : color = &backgroundLight [0]; break;
			case Background::Dark   : color = &backgroundDark  [0]; break;
			case Background::Action : color = &backgroundAction[0]; break;
			default					: color = &backgroundLight [0]; break;

		}

		glUniformMatrix4fv (manager->colorShader->locations["MVP"]  , 1, GL_FALSE, &(quadElement->MVPMatrix[0][0]));
		glUniform4fv       (manager->colorShader->locations["Color"], 1, color); 

		quadElement->setPosition (glm::vec2 (absLeft , app->windowHeight-height-absTop));
		quadElement->setScale    (glm::vec2 (width, height));
		quadElement->render();

		/* Switch Shaders */

		manager->colorShader->unbind();
		manager->atlasShader->bind();

	}

	/* Render Childs */

	UIElement::render();

}

/* Events */

void UIPanel::keyPressed (int key) {

	UIElement::keyPressed (key);

}

void UIPanel::dblClick (int x, int y, int button) {

	UIElement::dblClick (x, y, button);

}

void UIPanel::mouseDown (int x, int y, int button) {

	UIElement::mouseDown (x, y, button);

}

void UIPanel::mouseMove (int x, int y, int button) {

	UIElement::mouseMove (x, y, button);

}

void UIPanel::mouseUp (int x, int y, int button) {

	UIElement::mouseUp (x, y, button);

}

