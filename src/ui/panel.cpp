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
	hsOffset =  (wrapperHeight*py)/100;

}

void UIPanel::addScrollYByPx (int pxVal) {

}

void UIPanel::setScrollYByPx (int pxVal) {

	vbOffset = pxVal;
	math::clamp (&vbOffset, 0, vbMax-vbSize);

	int py   =  (vbOffset*100)/vbMax;
	vsOffset =  (wrapperHeight*py)/100;

}

void UIPanel::setScrollXByPx (int pxVal) {

	hbOffset = pxVal;
	math::clamp (&hbOffset, 0, hbMax-hbSize);

	int py    = (hbOffset*100)/hbMax;
	hsOffset  = (wrapperWidth*py)/100;

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

		renderColorElement (absLeft, absTop, width, height, quadElement.get(), color);

		/* Switch Shaders */

		manager->colorShader->unbind();
		manager->atlasShader->bind();

	}

	/* Render Childs */

	int scx = absLeft;			        int scy = app->windowHeight-height-absTop+scrollElementHeight;
	int scw = width-scrollElementWidth; int sch = height/*-ElemsOffset*/;

	manager->pushScissor (scx, scy, scw, sch);
	UIElement::render();
	manager->popScissor();

	if (showScrollX || showScrollY)
		renderScroll();

}

void UIPanel::renderScroll() {

	int swv = iHeights[4];
	int swh = iHeights[4];

	scrollElementWidth  = 0;
	scrollElementHeight = 0;

	if (showScrollX && wrapperHeight != height) { swh = 0; scrollElementWidth  = iHeights[4]; }
	if (showScrollY && wrapperWidth  != width ) { swv = 0; scrollElementHeight = iHeights[4]; }

	int bottom       = absTop+height-iHeights[0];
	int scrollWidth  = width-iHeights[4]+swh;
	int scrollHeight = width-iHeights[4]+swh;

	/* Horizontal Scroll */

	if (wrapperWidth != width && showScrollX) {

		int n = 6;

		if (!manager->dialogOpened || inDialog) {

			hbMin = iWidths[6]+iWidths[8];
			hbMax = width-iHeights[4]+swh;

			int x  = (hbMax*100)/wrapperWidth;
			hbSize = (hbMax*x)/100;
			math::clamp (&hbSize, hbMin, hbMax);

			bool scrollEnter = pointInRect (app->mouseX, app->mouseY, hbOffset+absLeft, absTop+height-iHeights[6], hbSize, iHeights[6]);

			if (app->mouseButton == mouseLeft && !clicked && scrollEnter) {

				scrollHClick = true;
				clicked      = true;

			}

			n = scrollEnter || scrollHClick ? n+3 : n;
			if (clicked && !scrollHClick) n = 6;

			if (!splitClick && scrollHClick) {

				hbOffset = lhbOffset+app->mouseX-app->clickX;
				math::clamp (&hbOffset, 0, hbMax-hbSize);

				int py = (hbOffset*100)/hbMax;
				hsOffset = (wrapperWidth*py)/100;

			}

		}

		renderPartsElementH   (0,   1,   2, scrollBg [0].get(), scrollBg [1].get(), scrollBg [2].get(), absLeft, bottom, scrollWidth, true);
		renderPartsElementH   (n, n+1, n+2, scrollBtn[0].get(), scrollBtn[1].get(), scrollBtn[2].get(), absLeft+hbOffset, bottom, hbSize, true);

	}

	/* Vertical Scroll */

	if (wrapperHeight != height && showScrollY) {

		int n = 6;

		if (!manager->dialogOpened || inDialog) {

			vbMin = iWidths[6]+iWidths[8];
			vbMax = height-iHeights[4]+swv;

			int x  = (vbMax*100)/wrapperHeight;
			vbSize = (vbMax*x)/100;
			math::clamp (&vbSize, hbMin, vbMax);

			bool scrollEnter = pointInRect (app->mouseX, app->mouseY, absLeft+width-iHeights[6], absTop+vbOffset+headerSize, iHeights[6], vbSize);

			if (app->mouseButton == mouseLeft && !clicked && scrollEnter) {

				scrollVClick = true;
				clicked      = true;

			}

			n = scrollEnter || scrollVClick ? n+3 : n;
			if (clicked && !scrollVClick) n = 6;

			if (!splitClick && scrollVClick) {

				vbOffset = lhbOffset+app->mouseY-app->clickY;
				math::clamp (&vbOffset, 0, vbMax-vbSize);

				int py   = (vbOffset*100)/vbMax;
				vsOffset = (wrapperHeight*py)/100;

			}

		}

		renderPartsElementV90 (  5,   4, 3, scrollBg [3].get(), scrollBg [4].get(), scrollBg [5].get(), absLeft+width, absTop, scrollHeight, true);
		renderPartsElementV90 (n+2, n+1, n, scrollBtn[3].get(), scrollBtn[4].get(), scrollBtn[5].get(), absLeft+width, absTop+vbOffset, vbSize, true);

	}

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

	lhbOffset = hbOffset;
	lvbOffset = vbOffset;

}

void UIPanel::mouseMove (int x, int y, int button) {

	UIElement::mouseMove (x, y, button);

}

void UIPanel::mouseUp (int x, int y, int button) {

	UIElement::mouseUp (x, y, button);

	scrollHClick = false;
	scrollVClick = false;
	splitClick   = false;
	clicked      = false;

}

