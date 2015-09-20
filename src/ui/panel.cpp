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

void UIPanel::addScrollXByPx (int pxVal) { scrollX += pxVal; }
void UIPanel::addScrollYByPx (int pxVal) { scrollY += pxVal; }
void UIPanel::setScrollXByPx (int pxVal) { scrollX  = pxVal; }
void UIPanel::setScrollYByPx (int pxVal) { scrollY  = pxVal; }

/* By Percent */

void UIPanel::addScrollXByPct (int pctVal) {

	hbOffset += round(static_cast<float>(hbMax-hbSize)*static_cast<float>(pctVal)/100.f);;
	math::clamp (&hbOffset, 0, hbMax-hbSize);

	float px = (static_cast<float>(hbOffset)*100.f)/static_cast<float>(hbMax);
	scrollX  =  round((static_cast<float>(wrapperWidth)*px)/100.f);

}

void UIPanel::addScrollYByPct (int pctVal) {

	vbOffset += round(static_cast<float>(vbMax-vbSize)*static_cast<float>(pctVal)/100.f);
	math::clamp (&vbOffset, 0, vbMax-vbSize);

	float py = (static_cast<float>(vbOffset)*100.f)/static_cast<float>(vbMax);
	scrollY  = round((static_cast<float>(wrapperHeight)*py)/100.f);

}

void UIPanel::setScrollYByPct (int pctVal) {

	vbOffset = round(static_cast<float>(vbMax-vbSize)*static_cast<float>(pctVal)/100.f);
	math::clamp (&vbOffset, 0, vbMax-vbSize);

	float py = (static_cast<float>(vbOffset)*100.f)/static_cast<float>(vbMax);
	scrollY  = round((static_cast<float>(wrapperHeight)*py)/100.f);

}

void UIPanel::setScrollXByPct (int pctVal) {

	hbOffset = round(static_cast<float>(hbMax-hbSize)*static_cast<float>(pctVal)/100.f);;
	math::clamp (&hbOffset, 0, hbMax-hbSize);

	float px = (static_cast<float>(hbOffset)*100.f)/static_cast<float>(hbMax);
	scrollX  = round((static_cast<float>(wrapperWidth)*px)/100.f);

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

	updateScroll();

}

void UIPanel::updateAlign() {

	if (align == Align::None)
		return;

	int x0 = 0; int mWidth  = 0; int mLeft = 0;
	int y0 = 0; int mHeight = 0; int mTop  = 0;

	/* Find Border */

	for (const auto &kvp : parent->elements) {

		UIElement *el = kvp.second.get();

		if (el == this)
			break;

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

			left   = x0;
			top    = y0;

			break;

		case Align::Top :

			width  = parent->width-mWidth-x0-parent->scrollElementWidth;

			left   = x0;
			top    = y0;

			break;

		case Align::Bottom :

			width  = parent->width-mWidth-x0-parent->scrollElementWidth;

			left   = x0;
			top    = y0+parent->height-height-mTop-mHeight;

			break;

		case Align::Left :

			height = parent->height-mHeight-y0-parent->scrollElementHeight;

			left   = x0;
			top    = y0;

			break;

		case Align::Right :

			height = parent->height-mHeight-y0;

			left   = parent->width-mWidth-width;
			top    = y0;

			break;

		default: return;

	}

}

void UIPanel::setCursor() {

	if (!allowResize || !open || scrollHClick || scrollVClick) return;
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

	manager->disablePollMap[id] = splitEnter;

}

void UIPanel::progress() {



}

/* Render Panel */

void UIPanel::render() {

	splitEnter = false;

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
			default                 : color = &backgroundLight [0]; break;

		}

		renderColorElement (absLeft, absTop, width, height, quadElement.get(), color);

		/* Switch Shaders */

		manager->colorShader->unbind();
		manager->atlasShader->bind();

	}

	/* Render Header */

	if (allowResize || showSplit) calculateSplit();
	if (allowHide) {

		int n = headerEnter ? 17 : 16;
		paddingTop = headerHeight;

		headerEnter = pointInRect (app->mouseX, app->mouseY, absLeft, absTop, width, iHeights[n]);
		renderElement (n, absLeft, absTop, width, iHeights[n], header.get());

		/* Draw Arrow */

		n = open ? 14 : 15;
		renderElement (n, absLeft+6, absTop+(headerHeight >> 1)-(iHeights[n] >> 1), iWidths[n], iHeights[n], expandArrow.get());

		/* Draw Text */

		manager->atlasShader->unbind();
		renderText (manager->theme->font, textColor, absLeft+12+iWidths[n], absTop+(headerHeight >> 1)+(manager->theme->fontHeight >> 1)-2, caption);
		manager->atlasShader->bind();

	}

	if (!open) {

		updateAlign();
		return;

	}

	/* Render Scroll */

	int swv = iHeights[4]-scrollElementHeight;
	int swh = iHeights[4]-scrollElementWidth;

	int bottom       = absTop+height-iHeights[0];
	int scrollWidth  = width -iHeights[4]+swh;
	int scrollHeight = height-iHeights[4]+swv;

	if (wrapperWidth > width && showScrollX) {

		//int n = scrollHEnter || scrollHClick ? 9 : 6;
		int n = 6;

		if (scrollHEnter) n = 9;
		if (scrollHClick) n = 18;

		renderPartsElementH (0,   1,   2, scrollBg [0].get(), scrollBg [1].get(), scrollBg [2].get(), absLeft, bottom, scrollWidth, true);
		renderPartsElementH (n, n+1, n+2, scrollBtn[0].get(), scrollBtn[1].get(), scrollBtn[2].get(), absLeft+hbOffset, bottom, hbSize, true);

	}

	/* Vertical */

	if (wrapperHeight > height && showScrollY) {

		//int n = scrollVEnter || scrollVClick ? 9 : 6;
		int n = 6;

		if (scrollVEnter) n = 9;
		if (scrollVClick) n = 18;

		renderPartsElementV90 (  5,   4, 3, scrollBg [3].get(), scrollBg [4].get(), scrollBg [5].get(), absLeft+width-scrollElementWidth, absTop+paddingTop, scrollHeight-paddingTop, true);
		renderPartsElementV90 (n+2, n+1, n, scrollBtn[3].get(), scrollBtn[4].get(), scrollBtn[5].get(), absLeft+width-scrollElementWidth, absTop+vbOffset+paddingTop, vbSize-paddingTop, true);

	}

	/* */

	manager->pushScissor (absLeft, absTop+paddingTop, width-scrollElementWidth, height-scrollElementHeight-paddingTop);

	if (splitClick) {

		/* Resize Panel */

		switch (align) {

			case Align::Top    : height = lastHeight+app->mouseY-app->clickY; break;
			case Align::Bottom : height = lastHeight-app->mouseY+app->clickY; break;
			case Align::Left   : width  = lastWidth +app->mouseX-app->clickX; break;
			case Align::Right  : width  = lastWidth -app->mouseX+app->clickX; break;

			default : ;

		}

		if (align == Align::Top  || align == Align::Bottom) math::clamp (&height, minSize, maxSize);
		if (align == Align::Left || align == Align::Right ) math::clamp (&width , minSize, maxSize);

	}

	/* Render Childs */

	UIElement::render();
	manager->popScissor();

	if (showScrollX || showScrollY) pollScroll();

	/* Render Split */

	if (allowResize || showSplit) {

		int n = blackSplit ? 12 : 13;
		renderElement (n, splitX, splitY, splitW, splitH, split.get());

	}

	updateAlign();

}

void UIPanel::calculateSplit() {

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

}

void UIPanel::pollScroll() {

	if (!open) return;

	int swv = iHeights[4];
	int swh = iHeights[4];

	scrollElementWidth  = 0;
	scrollElementHeight = 0;

	if (showScrollX && wrapperHeight > height) { swh = 0; scrollElementWidth  = iHeights[4]; }
	if (showScrollY && wrapperWidth  > width ) { swv = 0; scrollElementHeight = iHeights[4]; }

	/* Horizontal Scroll */

	if (wrapperWidth > width && showScrollX) {

		if (!manager->dialogOpened || inDialog) {

			hbMin = iWidths[6]+iWidths[8];
			hbMax = width-iHeights[4]+swh;

			float x = (static_cast<float>(hbMax)*100.f)/(float)wrapperWidth;
			hbSize  = ceil((static_cast<float>(hbMax)*x)/100.f);
			math::clamp (&hbSize, hbMin, hbMax);

			scrollHEnter = pointInRect (app->mouseX, app->mouseY, hbOffset+absLeft, absTop+height-iHeights[6], hbSize, iHeights[6]);

			if (!splitClick && scrollHClick) {

				hbOffset = lhbOffset+app->mouseX-app->clickX;
				math::clamp (&hbOffset, 0, hbMax-hbSize);

				float py  = (static_cast<float>(hbOffset)*100.f)/static_cast<float>(hbMax);
				scrollX = ceil((static_cast<float>(wrapperWidth)*py)/100.f);

			}

		}

	}

	/* Vertical Scroll */

	if (wrapperHeight > height && showScrollY) {

		int n = 6;

		if (!manager->dialogOpened || inDialog) {

			vbMin = iWidths[6]+iWidths[8];
			vbMax = height-iHeights[4]+swv;

			float x  = (static_cast<float>(vbMax)*100.f)/static_cast<float>(wrapperHeight);
			vbSize = ceil((static_cast<float>(vbMax)*x)/100.f);
			math::clamp (&vbSize, vbMin, vbMax);

			scrollVEnter = pointInRect (app->mouseX, app->mouseY, absLeft+width-iHeights[6], absTop+vbOffset, iHeights[6], vbSize);
			n = scrollVEnter || scrollVClick ? n+3 : n;

			if (clicked && !scrollVClick) n = 6;
			if (!splitClick && scrollVClick) {

				vbOffset = lvbOffset+app->mouseY-app->clickY;
				math::clamp (&vbOffset, 0, vbMax-vbSize);

				float py = (static_cast<float>(vbOffset)*100.f)/static_cast<float>(vbMax);
				scrollY  = ceil((static_cast<float>(wrapperHeight)*py)/100.f);

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

	if (splitEnter && open) {

		splitClick = true;
		lastWidth  = width;
		lastHeight = height;

	}

	for (const auto &kvp : parent->elements) {

		UIElement *el = kvp.second.get();
		UIPanel *panel = dynamic_cast<UIPanel*>(el);

		if (!panel)
			continue;

		if (panel->splitClick)
			return;

	}

	if (!allowHide || !headerEnter)
		return;

	/* Open/Close Panel */

	open = !open;

	if (!open) {

		lastHeight = height;
		height = headerHeight;

	} else {

		height = lastHeight;

	}

}

void UIPanel::mouseUp (int x, int y, int button) {

	UIElement::mouseUp (x, y, button);

	scrollHClick = false;
	scrollVClick = false;

	if (splitClick) {

		splitClick = false;

	}

}

void UIPanel::mouseWheel (int dx, int dy) {

	if (!open) return;

	UIElement::mouseWheel (dx, dy);
	UIElement *el = manager->underMouse;

	bool underCanScrollX;
	bool underCanScrollY;
	auto lastParent = el;

	if (!over) return;
	while (true) {

		if (lastParent->isRoot) break;
		if (lastParent == this) break;

		underCanScrollX = lastParent->canScrollX();
		underCanScrollY = lastParent->canScrollY();

		lastParent = lastParent->parent;

	}

	if (pressed(keyShift)) { // Inverse

		if (wrapperHeight > height && showScrollY && !underCanScrollY) addScrollYByPx (-dx*scrollDelta);
		if (wrapperWidth  > width  && showScrollX && !underCanScrollX) addScrollXByPx (-dy*scrollDelta);

	} else {

		if (wrapperHeight > height && showScrollY && !underCanScrollY) addScrollYByPx (-dy*scrollDelta);
		if (wrapperWidth  > width  && showScrollX && !underCanScrollX) addScrollXByPx (-dx*scrollDelta);

	}

}

/* Update scroll buttons positions */

void UIPanel::updateScroll() {

	/* Update Scroll Sizes */

	if (wrapperWidth == 0 && wrapperHeight == 0)
		return;

	int swv = iHeights[4]-scrollElementHeight;
	int swh = iHeights[4]-scrollElementWidth;

	/* Horizontal */

	if (wrapperWidth > width && showScrollX) {

		hbMin = iWidths[6]+iWidths[8];
		hbMax = width-iHeights[4]+swh;

		float px = (100.f*static_cast<float>(scrollX))/static_cast<float>(wrapperWidth);
		hbOffset = ceil((px*hbMax)/100.f);

		math::clamp (&hbOffset, 0, hbMax-hbSize);
		math::clamp (&scrollX , 0, maxScrollX());

	} else {

		scrollX = 0;

	}

	/* Vertical */

	if (wrapperHeight > height && showScrollY) {

		vbMin = iWidths[6]+iWidths[8];
		vbMax = height-iHeights[4]+swv;

		float py = (100.f*static_cast<float>(scrollY))/static_cast<float>(wrapperHeight);
		vbOffset = round((py*static_cast<float>(vbMax))/100.f);

		math::clamp (&vbOffset, 0, vbMax-vbSize);
		math::clamp (&scrollY , 0, maxScrollY());

	} else {

		scrollY = 0;

	}

}
