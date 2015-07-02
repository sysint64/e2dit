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

#include "ui/element.h"
#include "ui/manager.h"

/**
 *
 */

void UIElement::dblClick (int x, int y, int button) {

	if (!visible) return;

	for (const auto &kvp : elements) {

		kvp.second->dblClick (x, y, button);

	}

	if (onDblClick == nullptr || !enabled || ! enter)
		return;

	onDblClick (this, x, y, button);

}

void UIElement::mouseDown (int x, int y, int button) {

	if (!visible) return;

	/* */

	for (const auto &kvp : elements) {

		kvp.second->mouseDown (x, y, button);

	}

	if (onMouseDown == nullptr || !enabled || ! enter)
		return;

	onMouseDown (this, x, y, button);

}

void UIElement::mouseWheel (int delta) {

	if (!visible) return;

	/* */

	for (const auto &kvp : elements) {

		kvp.second->mouseWheel (delta);

	}

}

void UIElement::mouseUp (int x, int y, int button) {

	if (!enabled/* || !enter*/)
		return;

	click = false;

	if (enter && onClick != nullptr)
		onClick (this);

	/* */

	for (const auto &kvp : elements) {

		kvp.second->mouseUp (x, y, button);

	}

}

void UIElement::setCursor() {

	if (!enter) return;
	manager->cursor = cursor;

}

void UIElement::checkFocus() {

	/*for (const auto &it : elements) {

		if (it.second->click) {

			it.second->focus();
			return;

		}

		it.second->checkFocus();

	}*/

}

void UIElement::renderElement (int idx, int x, int y, int w, int h, BaseObject *el) {

	if (math::feq<float>(el->rotation, math::pi/2.f, math::pi/180.f)) el->setPosition (glm::vec2(x+h, app->windowHeight-w-y)); else
	if (math::feq<float>(el->rotation,-math::pi/2.f, math::pi/180.f)) el->setPosition (glm::vec2(x-h, app->windowHeight+h-y)); else
	if (math::feq<float>(el->rotation, math::pi    , math::pi/180.f)) el->setPosition (glm::vec2(x+w, app->windowHeight+h-y)); else
																	  el->setPosition (glm::vec2(x  , app->windowHeight-h-y));

	el->setScale (glm::vec2(w, h));
	el->updateModelMatrix();

	glUniformMatrix4fv (manager->atlasShader->locations["MVP"], 1, GL_FALSE, &(el->MVPMatrix[0][0]));
	glUniform2f		   (manager->atlasShader->locations["Size"]  , fWidths [idx], fHeights[idx]);
	glUniform2f		   (manager->atlasShader->locations["Offset"], offsetsX[idx], offsetsY[idx]);

	el->render();

}

void UIElement::renderColorElement (int x, int y, int w, int h, BaseObject *el, float *color) {

	el->setPosition (glm::vec2(x, app->windowHeight-y-h));
	el->setScale    (glm::vec2(w, h));
	el->updateModelMatrix();

	glUniformMatrix4fv (manager->colorShader->locations["MVP"]  , 1, GL_FALSE, &(el->MVPMatrix[0][0]));
	glUniform4fv       (manager->colorShader->locations["Color"], 1, color);

	el->render();

}

void UIElement::renderPartsElementH (int il, int ic, int ir,
									 BaseObject *el, BaseObject *ec, BaseObject *er,
									 int x, int y, int w, bool ignoreDrawAlign)
{

	int cw;

	if (!ignoreDrawAlign) {

		switch (drawAlign) {

			case Align::Left   : cw = w-iWidths[ir];
			case Align::Right  : cw = w-iWidths[il];
			case Align::Center : cw = w;
			default            : cw = w-iWidths[il]-iWidths[ir];

		}

	} else {

		cw = w-iWidths[il]-iWidths[ir];

	}

	if (drawAlign == Align::Left  || drawAlign == Align::All)
		renderElement (il, x, y, iWidths[il], iHeights[il], el);

	if (drawAlign == Align::Right || drawAlign == Align::All)
		renderElement (ir, x+iWidths[il]+cw, y, iWidths[ir], iHeights[ir], er);

	renderElement (ic, x+iWidths[il], y, cw, iHeights[ic], ec);

}

void UIElement::renderPartsElementH (int il, int ic, int ir,
									 BaseObject *el, BaseObject *ec, BaseObject *er,
									 int x, int y, int w, int h, bool ignoreDrawAlign)
{

	int cw = w-iWidths[il]-iWidths[ir];

	renderElement (il, x, y, iWidths[il], h, el);
	renderElement (ic, x+iWidths[il], y, cw, h, ec);
	renderElement (ir, x+iWidths[il]+cw, y, iWidths[ir], h, er);

}


void UIElement::renderPartsElementV90 (int it, int im, int ib,
									   BaseObject *et, BaseObject *em, BaseObject *eb,
									   int x, int y, int h, bool ignoreDrawAlign)
{

	int mh = h-iWidths[it]-iWidths[ib];

	renderElement (it, x, y, iWidths[it], iHeights[it], et);
	renderElement (im, x, y+iWidths[it], mh, iHeights[it], em);
	renderElement (ib, x, y+iWidths[it]+mh, iWidths[ib], iHeights[ib], eb);

}

void UIElement::mouseMove (int x, int y, int button) {

	if (!enabled/* || !enter*/)
		return;

	for (const auto &kvp : elements) {

		kvp.second->mouseMove (x, y, button);

	}

}

void UIElement::keyPressed (int key) {

	if (!visible) return;
	//if (onKeyPressed == nullptr || !enabled)
		//return

	//onKeyPressed (this, key);

	/* Poll Event in child elements */

	for (const auto &kvp : elements) {

		kvp.second->keyPressed (key);

	}

}

void UIElement::keyReleased (int key) {

	keyClick = false;

	for (const auto &kvp : elements) {

		kvp.second->keyReleased (key);

	}

}

void UIElement::textEntered (int key) {

	keyClick = false;

	for (const auto &kvp : elements) {

		kvp.second->textEntered (key);

	}

}

void UIElement::resized (int width, int height) {

	for (const auto &kvp : elements) {

		kvp.second->resized (width, height);

	}

}

void UIElement::focus() {

	if (manager->dialogOpened && !inDialog)
		return;

	if (manager->focusedElement != this && manager->focusedElement != nullptr)
		manager->focusedElement->unfocus();

	manager->focusedElement = this;
	focused = true;

}

void UIElement::unfocus() {

	manager->unfocusedElements.push_back (this);

}

void UIElement::updateAbsPos() {

	glm::vec2 res (0, 0);
	UIElement *lastParent = parent;

	while (lastParent != nullptr) {

		res.x += lastParent->left;
		res.y += lastParent->top;
		lastParent = lastParent->parent;

	}

	absLeft = left+res.x-parent->scrollX;
	absTop  = top +res.y-parent->scrollY;

}

/* Render & poll *************************************************************/

void UIElement::render() {

	wrapperWidthClamped  = width;  wrapperWidth  = 0;
	wrapperHeightClamped = height; wrapperHeight = 0;

	if (isRoot) {

		over = true;
		//left = 0; top  = 0;
		//absLeft = 0; absTop = 0;
		width  = app->windowWidth;
		height = app->windowHeight;

	}

	/* Render Elements */

	for (const auto &kvp : elements) {

		UIElement *el = kvp.second.get();
		el->over = pointInRect (app->mouseX, app->mouseY, el->absLeft, el->absTop, el->width, el->height);

		// TODO: Move to UIPanel

		if (el->align != Align::Top && el->align != Align::Bottom)
			wrapperWidth  = math::max (wrapperWidth , el->left+el->width);

		if (el->align != Align::Left && el->align != Align::Right)
			wrapperHeight = math::max (wrapperHeight, el->top +el->height);

		if (el->align != Align::Top && el->align != Align::Bottom)
			wrapperWidthClamped  = math::max (wrapperWidthClamped , el->left+el->width);

		if (el->align != Align::Left && el->align != Align::Right)
			wrapperHeightClamped = math::max (wrapperHeightClamped, el->top +el->height);

		//

		if (el->visible)
			el->render();

	}

}

void UIElement::poll() {


}

/* Manage Elements ***********************************************************/

/**
 *
 */

void UIElement::addElement (std::unique_ptr<UIElement> el) {

	/* FIXME: Allow add if not parent at element */

	if (!isRoot && parent == nullptr)
		Application::getInstance()->log.ewrite ("Can't add element, element don't has parent!");

	el->parent = this;
	el->id = manager->lastId;

	/* Make Loop */

	if (firstEl == nullptr) {

		firstEl  = el.get();
		el->next = el.get();
		el->prev = el.get();

	} else

	/* Add neighbors */

	if (lastEl != nullptr) {

		lastEl->next = el.get();
		el->prev = lastEl;
		el->next = firstEl; /* Loop */
		firstEl->prev = el.get();

	}

	lastEl = el.get();

	/* Add Element to map */

	manager->elementsStack.push_back (el.get());
	elements[manager->lastId] = std::move (el);
	manager->lastId++;

}

/**
 *
 */

void UIElement::deleteElement (std::unique_ptr<UIElement> el) {

	elements.erase (el->id);

}

/**
 *
 */

void UIElement::deleteElement (const int  id) {

	if (elements[id] == nullptr)
		return;

	elements.erase (id);

}

/**
 *
 */

UIElement *UIElement::getElement (const int id) {

	return elements[id].get();

}

/**
 *
 */

std::unique_ptr<UIElement> UIElement::takeElement (const int id) {

	auto el = std::move (elements[id]);
	elements.erase (id);
	el->parent = nullptr;

	return el;

}
