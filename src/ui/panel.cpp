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

void UIPanel::addScrollXByPx (int pxVal) { scrollX += pxVal; resized (0, 0); }
void UIPanel::addScrollYByPx (int pxVal) { scrollY += pxVal; resized (0, 0); }
void UIPanel::setScrollXByPx (int pxVal) { scrollX  = pxVal; resized (0, 0); }
void UIPanel::setScrollYByPx (int pxVal) { scrollY  = pxVal; resized (0, 0); }

/* By Percent */

void UIPanel::addScrollXByPct (int pctVal) {

	hbOffset += round((float)(hbMax-hbSize)*(float)pctVal/100.f);;
	math::clamp (&hbOffset, 0, hbMax-hbSize);

	float px = ((float)hbOffset*100.f)/(float)hbMax;
	scrollX  =  round(((float)wrapperWidth*px)/100.f);

}

void UIPanel::addScrollYByPct (int pctVal) {

	vbOffset += round((float)(vbMax-vbSize)*(float)pctVal/100.f);
	math::clamp (&vbOffset, 0, vbMax-vbSize);

	float py = ((float)vbOffset*100.f)/(float)vbMax;
	scrollY  = round(((float)wrapperHeight*py)/100.f);

}

void UIPanel::setScrollYByPct (int pctVal) {

	vbOffset = round((float)(vbMax-vbSize)*(float)pctVal/100.f);
	math::clamp (&vbOffset, 0, vbMax-vbSize);

	float py = ((float)vbOffset*100.f)/(float)vbMax;
	scrollY  = round(((float)wrapperHeight*py)/100.f);

}

void UIPanel::setScrollXByPct (int pctVal) {

	hbOffset = round((float)(hbMax-hbSize)*(float)pctVal/100.f);;
	math::clamp (&hbOffset, 0, hbMax-hbSize);

	float px = ((float)hbOffset*100.f)/(float)hbMax;
	scrollX  = round(((float)wrapperWidth*px)/100.f);

}

void UIPanel::scrollToElement (UIElement *el) {

	if (test == nullptr)
		return;

	if (wrapperWidth == width || !showScrollX)
		return;

	if (wrapperHeight == height || !showScrollY)
		return;

	setScrollXByPx (test->left);
	setScrollYByPx (test->top);

	resized (0, 0);

}

void UIPanel::updateAlign() {

	if (align == Align::None)
		return;

	int x0 = 0; int mWidth  = 0; int mLeft = 0;
	int y0 = 0; int mHeight = 0; int mTop  = 0;

	/* Find Border */

	for (const auto &kvp : elements) {

		UIElement *el = kvp.second.get();

		if (!el->visible || el->align == Align::None)
			continue;

		switch (el->align) {

			case Align::Top    : y0 += el->height; mTop  += el->height; break;
			case Align::Left   : x0 += el->width ; mLeft += el->width;  break;

			case Align::Bottom : mHeight += el->height; break;
			case Align::Right  : mWidth  += el->width;  break;

			default: continue;

		}

	}

	/* Update Position and Size depending on the align */

	switch (align) {

		case Align::Client :

			width  = parent->width -mWidth -x0-parent->scrollElementWidth;
			height = parent->height-mHeight-y0-parent->scrollElementHeight;

			left   = parent->left+x0;
			top    = parent->top +y0;

			break;

		case Align::Top :

			width  = parent->width-mWidth-x0-parent->scrollElementWidth;

			left   = parent->left+x0;
			top    = parent->top +y0;

			break;

		case Align::Bottom :

			width  = parent->width-mWidth-x0-parent->scrollElementWidth;

			left   = parent->left+x0;
			top    = parent->top +y0+parent->height-height-mTop-mHeight;

			break;

		case Align::Left :

			height = parent->height-mHeight-y0-parent->scrollElementHeight;

			left   = parent->left+x0;
			top    = parent->top +y0;

			break;

		case Align::Right :

			height = parent->height-mHeight-y0;

			left   = parent->left+parent->width-mWidth-width;
			top    = parent->top +y0;

			break;

		default: return;

	}

}

void UIPanel::setCursor() {

	if (!allowResize) return;
	if (align == Align::Top || align == Align::Bottom) {

		if (pointInRect(app->mouseX, app->mouseY, splitX, splitY-4, splitW, 8) || splitClick) {

			manager->cursor = CursorIco::VDoubleArrow;
			splitEnter = true;

		}

	} else if (align == Align::Left || align == Align::Right) {

		if (pointInRect(app->mouseX, app->mouseY, splitX-4, splitY, 8, splitW) || splitClick) {

			manager->cursor = CursorIco::HDoubleArrow;
			splitEnter = true;

		}

	}

}

/* Render Panel */

void UIPanel::render() {

	splitEnter = false;

	updateAlign();
	updateAbsPos();
	updateScroll();

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

	/* Render Scroll */

	int swv = iHeights[4]-scrollElementHeight;
	int swh = iHeights[4]-scrollElementWidth;

	int bottom       = absTop+height-iHeights[0];
	int scrollWidth  = width -iHeights[4]+swh;
	int scrollHeight = height-iHeights[4]+swv;

	if (wrapperWidth != width && showScrollX) {

		int n = scrollHEnter || scrollHClick ? 9 : 6;
		if (!scrollHClick) n = 6;

		renderPartsElementH (0,   1,   2, scrollBg [0].get(), scrollBg [1].get(), scrollBg [2].get(), absLeft, bottom, scrollWidth, true);
		renderPartsElementH (n, n+1, n+2, scrollBtn[0].get(), scrollBtn[1].get(), scrollBtn[2].get(), absLeft+hbOffset, bottom, hbSize, true);

	}

	/* Vertical */

	if (wrapperHeight != height && showScrollY) {

		int n = scrollVEnter || scrollVClick ? 9 : 6;
		if (!scrollVClick) n = 6;

		renderPartsElementV90 (  5,   4, 3, scrollBg [3].get(), scrollBg [4].get(), scrollBg [5].get(), absLeft+width-scrollElementWidth, absTop, scrollHeight, true);
		renderPartsElementV90 (n+2, n+1, n, scrollBtn[3].get(), scrollBtn[4].get(), scrollBtn[5].get(), absLeft+width-scrollElementWidth, absTop+vbOffset, vbSize, true);

	}

	/* */

	manager->pushScissor (absLeft, absTop, width-scrollElementWidth, height-scrollElementHeight);

	if (splitClick) {

		/* Resize Panel */

		switch (align) {

			case Align::Top    : height = lastHeight+app->mouseY-app->clickY; break;
			case Align::Bottom : height = lastHeight-app->mouseY+app->clickY; break;
			case Align::Left   : width  = lastWidth +app->mouseX-app->clickX; break;
			case Align::Right  : width  = lastWidth -app->mouseX+app->clickX; break;

			default : ;

		}

	}

	/* Render Childs */

	UIElement::render();
	manager->popScissor();

	if (showScrollX || showScrollY) pollScroll();
	if (allowResize)				renderSplit();

}

void UIPanel::renderSplit() {

	int n = blackSplit ? 12 : 13;

	/* Set Split Position and Size */

	switch (align) {

		case Align::Top :

			splitX = absLeft; splitY = absTop+height;
			splitW = width;   splitH = iHeights[n];
			split->setRotation (math::pi);

			break;

		case Align::Bottom :

			splitX = absLeft; splitY = absTop;
			splitW = width;   splitH = iHeights[n];

			break;

		case Align::Left :

			splitX = absLeft+width; splitY = absTop;
			splitW = height;        splitH = iHeights[n];
			split->setRotation (-math::pi/2.f);

			break;

		case Align::Right :

			splitX = absLeft; splitY = absTop;
			splitW = height;  splitH = iHeights[n];
			split->setRotation (math::pi/2.f);

			break;

		default : return;

	}

	/* Render */

	renderElement (n, splitX, splitY, splitW, splitH, split.get());

}

void UIPanel::pollScroll() {

	int swv = iHeights[4];
	int swh = iHeights[4];

	scrollElementWidth  = 0;
	scrollElementHeight = 0;

	if (showScrollX && wrapperHeight != height) { swh = 0; scrollElementWidth  = iHeights[4]; }
	if (showScrollY && wrapperWidth  != width ) { swv = 0; scrollElementHeight = iHeights[4]; }

	/* Horizontal Scroll */

	if (wrapperWidth != width && showScrollX) {

		if (!manager->dialogOpened || inDialog) {

			hbMin = iWidths[6]+iWidths[8];
			hbMax = width-iHeights[4]+swh;

			float x = ((float)hbMax*100.f)/(float)wrapperWidth;
			hbSize  = ceil(((float)hbMax*x)/100.f);
			math::clamp (&hbSize, hbMin, hbMax);

			scrollHEnter = pointInRect (app->mouseX, app->mouseY, hbOffset+absLeft, absTop+height-iHeights[6], hbSize, iHeights[6]);

			if (!splitClick && scrollHClick) {

				hbOffset = lhbOffset+app->mouseX-app->clickX;
				math::clamp (&hbOffset, 0, hbMax-hbSize);

				float py  = ((float)hbOffset*100.f)/(float)hbMax;
				scrollX = ceil(((float)wrapperWidth*py)/100.f);

			}

		}

	}

	/* Vertical Scroll */

	if (wrapperHeight != height && showScrollY) {

		int n = 6;

		if (!manager->dialogOpened || inDialog) {

			vbMin = iWidths[6]+iWidths[8];
			vbMax = height-iHeights[4]+swv;

			float x  = ((float)vbMax*100.f)/(float)wrapperHeight;
			vbSize = ceil(((float)vbMax*x)/100.f);
			math::clamp (&vbSize, vbMin, vbMax);

			scrollVEnter = pointInRect (app->mouseX, app->mouseY, absLeft+width-iHeights[6], absTop+vbOffset+headerSize, iHeights[6], vbSize);
			n = scrollVEnter || scrollVClick ? n+3 : n;

			if (clicked && !scrollVClick) n = 6;
			if (!splitClick && scrollVClick) {

				vbOffset = lvbOffset+app->mouseY-app->clickY;
				math::clamp (&vbOffset, 0, vbMax-vbSize);

				float py = ((float)vbOffset*100.f)/(float)vbMax;
				scrollY  = ceil(((float)wrapperHeight*py)/100.f);

			}

		}

	}

}

/* Events */

void UIPanel::mouseDown (int x, int y, int button) {

	UIElement::mouseDown (x, y, button);

	lhbOffset = hbOffset;
	lvbOffset = vbOffset;

	scrollHClick = scrollHEnter;
	scrollVClick = scrollVEnter;

	if (splitEnter) {

		splitClick = true;
		lastWidth  = width;
		lastHeight = height;

	}

}

void UIPanel::mouseUp (int x, int y, int button) {

	UIElement::mouseUp (x, y, button);

	scrollHClick = false;
	scrollVClick = false;

	if (splitClick) {

		splitClick = false;
		//setTimeout (0.1f, [this]() { this->resized (0, 0); }); // update scroll

	}

}

void UIPanel::mouseWheel (int delta) {

	UIElement::mouseWheel (delta);

	if (!over || (manager->underMouse->canScroll && manager->underMouse != this))
		return;

	if (pressed(keyShift)) {

		if (wrapperWidth == width || !showScrollX)
			return;

		addScrollXByPx (-delta*scrollDelta);

	} else {

		if (wrapperHeight == height || !showScrollY)
			return;

		addScrollYByPx (-delta*scrollDelta);

	}

}

/* Update scroll buttons positions */

void UIPanel::updateScroll() {

	/* Update Scroll Sizes */

	if (wrapperWidth == 0 || wrapperHeight == 0)
		return;

	int swv = iHeights[4]-scrollElementHeight;
	int swh = iHeights[4]-scrollElementWidth;

	/* Horizontal */

	if (wrapperWidth != width && showScrollX) {

		hbMin = iWidths[6]+iWidths[8];
		hbMax = width-iHeights[4]+swh;

		float px = (100.f*(float)scrollX)/(float)wrapperWidth;
		hbOffset = ceil((px*hbMax)/100.f);
		math::clamp (&hbOffset, 0, hbMax-hbSize);

		if (hbOffset == 0 )           scrollX = 0;
		if (hbOffset == hbMax-hbSize) scrollX = wrapperWidth-width;

	} else {

		scrollX = 0;

	}

	/* Vertical */

	if (wrapperHeight != height && showScrollY) {

		vbMin = iWidths[6]+iWidths[8];
		vbMax = height-iHeights[4]+swv;

		float py = (100.f*(float)scrollY)/(float)wrapperHeight;
		vbOffset = round((py*(float)vbMax)/100.f);
		math::clamp (&vbOffset, 0, vbMax-vbSize);

		if (vbOffset == 0)            scrollY = 0;
		if (vbOffset == vbMax-vbSize) scrollY = wrapperHeight-height;

	} else {

		scrollY = 0;

	}

}
