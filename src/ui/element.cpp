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
	if (onMouseDown == nullptr || !enabled || ! enter)
		return;

	onDblClick (this, x, y, button);

}

void UIElement::mouseDown (int x, int y, int button) {

	if (!visible) return;
	if (onMouseDown == nullptr || !enabled || ! enter)
		return;

	onMouseDown (this, x, y, button);

}

void UIElement::mouseUp (int x, int y, int button) {

	if (!enabled || !enter)
		return;

	click = false;
	enter = false;

	if (onClick != nullptr)
		onClick (this);

}

void UIElement::keyPressed (int key) {

	if (!visible) return;
	if (onKeyPressed == nullptr || !enabled)
	
	onKeyPressed (this, key);
	
}

void UIElement::focus() {

	if (manager->dialogOpened && !inDialog)
		return;

	if (manager->focusedElement != this && manager->focusedElement != nullptr) {
		
		manager->focusedElement->unfocus();

	}

	manager->focusedElement = this;
	focused = true;

}

void UIElement::unfocus() {

	manager->unfocusedElements.push_back (this);

}

/* Render & poll *************************************************************/

void UIElement::render() {

	absLeft = top;
	absTop  = left;

	/* Render Elements */

	for (int i = 0; i < elements.size(); i++) {

		UIElement *el = elements[i].get();

		if (el->visible)
			el->render();

	}

}

void UIElement::poll() {

	/* Progress Event */

	for (int i = 0; i < elements.size(); i++) {

		UIElement *el = elements[i].get();
		el->progress();
		el->poll();

	}

	/*  */

	/* State */

	for (int i = 0; i < elements.size(); i++) {

		//std::shared_ptr<UIElement> el = kvp.second;
		UIElement *el = elements[i].get();
		bool click = false;

		if (el == nullptr)
			continue;

		if (!el->visible) continue;
		if (app->mouseButton == mouseLeft && !el->wasClick) {

			//el->wasClick = true;
			click = true;

		}

		if (!pointInRect (app->mouseX, app->mouseY, el->left,
						  el->top, el->width, el->height))
		{
			
			el->enter = false;
			el->click = false;

			continue;

		}

		if (!el->enabled) {

			el->enter = false;
			el->click = false;

			continue;
		}

		el->enter = true;
		manager->cursor = el->cursor;
		
		if (click) {
			
			el->click = true;
			el->focus();

		} else {
			
			el->click = false;

		}

	}

}

/* Manage Elements ***********************************************************/

/**
 *
 */

void UIElement::addElement (std::unique_ptr<UIElement> el) {

	el->parent = this;
	el->id = manager->lastId;
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
