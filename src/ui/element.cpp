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

void ui::UIElement::dblClick (int x, int y, int button) {

	if (!visible) return;

	for (const auto &kvp : elements) {

		kvp.second->dblClick (x, y, button);

	}

	if (onDblClick == nullptr || !enabled || ! enter)
		return;

	onDblClick (this, x, y, button);

}

void ui::UIElement::mouseDown (int x, int y, int button) {

	if (!visible) return;

	/* */

	for (const auto &kvp : elements) {

		kvp.second->mouseDown (x, y, button);

	}

	if (onMouseDown == nullptr || !enabled || ! enter)
		return;

	onMouseDown (this, x, y, button);

}

void ui::UIElement::mouseWheel (int dx, int dy) {

	if (!visible) return;

	/* */

	for (const auto &kvp : elements) {

		kvp.second->mouseWheel (dx, dy);

	}

}

void ui::UIElement::mouseUp (int x, int y, int button) {

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

void ui::UIElement::setCursor() {

	if (!enter) return;
	manager->cursor = cursor;

}

void ui::UIElement::checkFocus() {

	/*for (const auto &it : elements) {

		if (it.second->click) {

			it.second->focus();
			return;

		}

		it.second->checkFocus();

	}*/

}

void ui::UIElement::renderElement (int idx, int x, int y, int w, int h, gapi::BaseObject *el) const {

	if (math::feq<float>(el->rotation, math::pi/2.f, math::pi/180.f)) el->setPosition (glm::vec2(x+h, app->windowHeight-w-y)); else
	if (math::feq<float>(el->rotation,-math::pi/2.f, math::pi/180.f)) el->setPosition (glm::vec2(x-h, app->windowHeight+h-y)); else
	if (math::feq<float>(el->rotation, math::pi    , math::pi/180.f)) el->setPosition (glm::vec2(x+w, app->windowHeight+h-y)); else
	                                                                  el->setPosition (glm::vec2(x  , app->windowHeight-h-y));

	el->setScale (glm::vec2(w, h));
	el->updateModelMatrix();

	glUniformMatrix4fv (manager->atlasShader->locations["MVP"], 1, GL_FALSE, &(el->MVPMatrix[0][0]));
	glUniform2f        (manager->atlasShader->locations["Size"]  , fWidths [idx], fHeights[idx]);
	glUniform2f        (manager->atlasShader->locations["Offset"], offsetsX[idx], offsetsY[idx]);

	el->render();

}

void ui::UIElement::transformElement (int x, int y, int w, int h, gapi::BaseObject *el) const {

	el->setPosition (glm::vec2(x, app->windowHeight-h-y));
	el->setScale    (glm::vec2(w, h));
	el->updateModelMatrix();

}

void ui::UIElement::renderMaskElement (int maskidx, int idx, int x, int y, int w, int h, gapi::BaseObject *el) const {

	el->setPosition (glm::vec2(x, app->windowHeight-h-y));
	el->setScale    (glm::vec2(w, h));
	el->updateModelMatrix();

	glUniformMatrix4fv (manager->atlasMaskShader->locations["MVP"], 1, GL_FALSE, &(el->MVPMatrix[0][0]));

	/* Image */

	glUniform2f (manager->atlasMaskShader->locations["Size"]  , fWidths [idx], fHeights[idx]);
	glUniform2f (manager->atlasMaskShader->locations["Offset"], offsetsX[idx], offsetsY[idx]);

	/* Mask */

	glUniform2f (manager->atlasMaskShader->locations["MaskSize"]  , fWidths [maskidx], fHeights[maskidx]);
	glUniform2f (manager->atlasMaskShader->locations["MaskOffset"], offsetsX[maskidx], offsetsY[maskidx]);

	el->render();

}

void ui::UIElement::renderColorElement (int x, int y, int w, int h, gapi::BaseObject *el, float *color) const {

	el->setPosition (glm::vec2(x, app->windowHeight-y-h));
	el->setScale    (glm::vec2(w, h));
	el->updateModelMatrix();

	glUniformMatrix4fv (manager->colorShader->locations["MVP"]  , 1, GL_FALSE, &(el->MVPMatrix[0][0]));
	glUniform4fv       (manager->colorShader->locations["Color"], 1, color);

	el->render();

}

void ui::UIElement::renderPartsElementH (int il, int ic, int ir,
                                     gapi::BaseObject *el, gapi::BaseObject *ec, gapi::BaseObject *er,
                                     int x, int y, int w, bool ignoreDrawAlign) const
{

	int cw = w-iWidths[il]-iWidths[ir];

	switch (drawAlign) {

		case Align::Left :

			renderElement (il, x, y, iWidths[il], iHeights[il], el);
			renderElement (ic, x+iWidths[il], y, cw+iWidths[ir]+1, iHeights[ic], ec);

			break;

		case Align::Right :

			renderElement (ic, x, y, cw+iWidths[il], iHeights[ic], ec);
			renderElement (ir, x+iWidths[il]+cw, y, iWidths[ir], iHeights[ir], er);

			break;

		case Align::Center :
			renderElement (ic, x, y, cw+iWidths[il]+iWidths[ir]+1, iHeights[ic], ec);
			break;

	}

	if (ignoreDrawAlign || drawAlign == Align::All) {

		renderElement (il, x, y, iWidths[il], iHeights[il], el);
		renderElement (ic, x+iWidths[il], y, cw, iHeights[ic], ec);
		renderElement (ir, x+iWidths[il]+cw, y, iWidths[ir], iHeights[ir], er);

	}

}

void ui::UIElement::renderPartsElementH (int il, int ic, int ir,
                                     gapi::BaseObject *el, gapi::BaseObject *ec, gapi::BaseObject *er,
                                     int x, int y, int w, int h, bool ignoreDrawAlign) const
{

	const int cw = w-iWidths[il]-iWidths[ir];

	renderElement (il, x, y, iWidths[il], h, el);
	renderElement (ic, x+iWidths[il], y, cw, h, ec);
	renderElement (ir, x+iWidths[il]+cw, y, iWidths[ir], h, er);

}

void ui::UIElement::renderPartsElementV90 (int it, int im, int ib,
                                       gapi::BaseObject *et, gapi::BaseObject *em, gapi::BaseObject *eb,
                                       int x, int y, int h, bool ignoreDrawAlign) const
{

	const int mh = h-iWidths[it]-iWidths[ib];

	renderElement (it, x, y, iWidths[it], iHeights[it], et);
	renderElement (im, x, y+iWidths[it], mh, iHeights[it], em);
	renderElement (ib, x, y+iWidths[it]+mh, iWidths[ib], iHeights[ib], eb);

}

void ui::UIElement::mouseMove (int x, int y, int button) {

	if (!enabled/* || !enter*/)
		return;

	for (const auto &kvp : elements) {

		kvp.second->mouseMove (x, y, button);

	}

}

void ui::UIElement::keyPressed (int key) {

	if (!visible) return;

	/* Poll Event in child elements */

	for (const auto &kvp : elements) {

		kvp.second->keyPressed (key);

	}

}

void ui::UIElement::keyReleased (int key) {

	keyClick = false;

	for (const auto &kvp : elements) {

		kvp.second->keyReleased (key);

	}

}

void ui::UIElement::textEntered (int key) {

	keyClick = false;

	for (const auto &kvp : elements) {

		kvp.second->textEntered (key);

	}

}

void ui::UIElement::resized (int width, int height) {

	for (const auto &kvp : elements) {

		kvp.second->resized (width, height);

	}

}

void ui::UIElement::progress() {

	if (!visible || !enabled)
		return;

	for (const auto &kvp : elements) {

		kvp.second->progress();

	}

}

void ui::UIElement::focus() {

	if (manager->dialogOpened && !inDialog)
		return;

	if (manager->focusedElement != this && manager->focusedElement != nullptr)
		manager->focusedElement->unfocus();

	manager->focusedElement = this;
	focused = true;

	if (onFocus != nullptr)
		onFocus(this);

}

void ui::UIElement::unfocus() {

	manager->unfocusedElements.push_back (this);

}

void ui::UIElement::updateAbsPos() {

	glm::vec2 res (0, 0);
	UIElement *lastParent = parent;

	while (lastParent != nullptr) {

		res.x += lastParent->left-lastParent->scrollX+lastParent->paddingLeft+lastParent->marginLeft;
		res.y += lastParent->top -lastParent->scrollY+lastParent->paddingTop +lastParent->marginTop;

		lastParent = lastParent->parent;

	}

	absLeft = left+res.x+marginLeft;
	absTop  = top +res.y+marginTop;

}

/* Render & poll *************************************************************/

void ui::UIElement::render() {

	if (allowAlign)
		updateAlign();

	wrapperWidthClamped  = width;  wrapperWidth  = 0;
	wrapperHeightClamped = height; wrapperHeight = 0;

	if (isRoot) {

		over = true;

		left    = 0; top    = 0;
		absLeft = 0; absTop = 0;

		width  = app->windowWidth;
		height = app->windowHeight;

	}

	/* Render Elements */

	for (const auto &kvp : elements) {

		UIElement *el = kvp.second.get();

		if (!el->visible)
			continue;

		// #TODO:50 Move to UIPanel

		el->render();

		if (el->allowAlign)
			el->updateVerticalAlign();

		if (el->align != Align::Top && el->align != Align::Bottom)
			wrapperWidth  = math::max (wrapperWidth , el->left+el->width+paddingLeft+paddingRight);

		if (el->align != Align::Left && el->align != Align::Right)
			wrapperHeight = math::max (wrapperHeight, el->top +el->height+paddingTop+paddingBottom);

	}

	wrapperWidthClamped  = math::max (wrapperWidthClamped , wrapperWidth);
	wrapperHeightClamped = math::max (wrapperHeightClamped, wrapperHeight);

}

void ui::UIElement::poll() {


}

void ui::UIElement::updateAlign() {

	if (align == Align::None)
		return;

	// Update Position and Size depending on the align

	switch (align) {

		case Align::ParentWidth:

			if (alignSize) {
				width  = parent->width-parent->scrollElementWidth;
				width -= parent->paddingLeft+parent->paddingRight;

				left = 0;
			}

			break;

		case Align::ParentHeight:

			if (alignSize) {
				height  = parent->height-parent->scrollElementHeight;
				height -= parent->paddingTop+parent->paddingBottom;

				top = 0;
			}

			break;

		case Align::Client:

			if (alignSize) {
				width   = parent->width -parent->scrollElementWidth;
				height  = parent->height-parent->scrollElementHeight;

				width  -= parent->paddingLeft+parent->paddingRight;
				height -= parent->paddingTop+parent->paddingBottom;

				left = 0; top  = 0;
			}

			break;

		case Align::Center:

			left = ((parent->width) >> 1) - (width >> 1);
			break;

		case Align::Left:

			if (alignSize) {
				height = parent->height-parent->scrollElementHeight;
				top = 0;
			}

			left = 0;
			break;

		case Align::Right:

			if (alignSize) {
				height = parent->height;
				top = 0;
			}

			left  = parent->width-width-marginRight;
			left -= parent->paddingLeft+parent->paddingRight;

			break;

		// if Allow Align Size
		case Align::Bottom:

			if (alignSize) {
				width = parent->width-parent->scrollElementWidth;
				left = 0;
				top  = parent->height-height;
			}

			break;

		case Align::Top:

			if (alignSize) {
				width = parent->width-parent->scrollElementWidth;
				left = 0;
				top = 0;
			}

			break;

	}

}

void ui::UIElement::updateVerticalAlign() {

	switch (verticalAlign) {

		case Align::Bottom:

			top = parent->height-height - parent->paddingTop;
			break;

		case Align::Top:

			top = 0;
			break;

		case Align::Middle:

			top = ((parent->height) >> 1) - (height >> 1);
			break;

	}

}

/* Manage Elements */

void ui::UIElement::addElement (std::unique_ptr<UIElement> el) {

	/* FIXME: Allow add if not parent at element */

	if (!isRoot && !parent)
		Application::getInstance()->log.ewrite ("Can't add element, element don't has parent!");

	el->parent = this;
	el->id = manager->lastId;
	el->depth = depth+1;

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

	if (overlay)
		el->overlay = true;

	manager->elementsStack.push_back (el.get());
	elements[manager->lastId] = std::move (el);
	manager->lastId++;

}

void ui::UIElement::deleteElement (std::unique_ptr<UIElement> el) {

	elements.erase (el->id);

}

void ui::UIElement::deleteElement (const int  id) {

	if (elements[id] == nullptr)
		return;

	elements.erase (id);

}

ui::UIElement *ui::UIElement::getElement (const int id) {
	return elements[id].get();
}

ui::UIElement *ui::UIElement::findElement (const std::string &name) {

	for (auto const &kvp : elements) {
		UIElement *element = kvp.second.get();

		if (!element)
			continue;

		if (element->name == name)
			return element;

		if (element->elements.size() > 0) {
			UIElement *findElement = element->findElement (name);

			if (findElement)
				return findElement;
		}

	}

	return nullptr;

}

std::unique_ptr<ui::UIElement> ui::UIElement::takeElement (const int id) {

	auto el = std::move (elements[id]);
	elements.erase (id);
	el->parent = nullptr;

	return el;

}
