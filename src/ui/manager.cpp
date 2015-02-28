/*
 * E2DIT - is a 2D Map Editor to create a levels for 2d games
 * Copyright (C) 2015 Kabylin Andrey <andrey@kabylin.ru>

 * This file is part of E2DIT-UI.

 * E2DIT-UI is free software: you can redistribute it and/or modify
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

#include "ui/manager.h"

/**
 * Constructor
 */



/**
 * Insert new UI Element and set unique id
 * @param el: new UI Element
 */

void UIManager::addElement (std::shared_ptr<UIElement> el) {

	el->manager = this;
	el->id = lastId;
	elements[lastId] = el;

	lastId++;

}

/**
 * Delete element el from manager and memory
 * @param el: UI Element
 */

void UIManager::deleteElement (std::shared_ptr<UIElement> el) {

	//elements[el->id] = nullptr;
	//delete el;
	elements.erase (el->id);

}

/**
 * Delete element el from manager and memory by id
 * @param id: id of UI Element
 */

void UIManager::deleteElement (const int id) {

	if (elements[id] == nullptr)
		return;

	elements.erase (id);
	//delete elements[id];
	//elements[id] = nullptr;

}

/**
 * Push new scissor to stack
 *
 * @param sx: Scissor Position X
 * @param sy: Scissor Position Y
 * @param sw: Scissor Width
 * @param sh: Scissor Height
 */

void UIManager::pushScissor (int sx, int sy, int sw, int sh) {

	if (scissorStack.size() == 0)
		glEnable (GL_SCISSOR_TEST);

	scissorStack.push_back(glm::vec4(sx, sy, sx+sw, sy+sh));
	setScissor();

}

/**
 * Pop scissor fomr stack
 */

void UIManager::popScissor() {

	scissorStack.pop_back();

	if (scissorStack.size() == 0) glDisable (GL_SCISSOR_TEST);
	else setScissor();

}

/**
 * Set OpenGL Scissor by last scissor from stack
 */

void UIManager::setScissor () {

	int scx = scissorStack.back()[0]; int scy = scissorStack.back()[1];
	int scw = scissorStack.back()[2]; int sch = scissorStack.back()[3];

	if (scissorStack.size() >= 2)
	for (int i = 0; i < scissorStack.size(); i++) {

		if (scx < scissorStack[i][0]) scx = scissorStack[i][0];
		if (scy < scissorStack[i][1]) scy = scissorStack[i][1];

		if (scw > scissorStack[i][2]) scw = scissorStack[i][2];
		if (sch > scissorStack[i][3]) sch = scissorStack[i][3];

	}

	glScissor (scx, scy, scw-scx, sch-scy);

}

/**
 * Render UI Elements
 */

void UIManager::render() {

	/* Bind Theme Skin */

	atlasShader->bind();

	glActiveTexture (GL_TEXTURE3);
	glBindTexture   (GL_TEXTURE_2D, theme->skin->handle);

	glActiveTexture (GL_TEXTURE2);
	glBindTexture   (GL_TEXTURE_2D, theme->skin->handle);

	/* Bind UI VBO Data Render */

	uiDataRender->bind();

	glUniform1i (atlasShader->locations["Texture"], themeTexID);
	glUniform1f (atlasShader->locations["Alpha"  ], 1.0f);

	/* Progress Event */

	for (int i = 0; i < elements.size(); i++) {

		std::shared_ptr<UIElement> el = elements[i];
		el->progress();

		/* Render UI Elements */

		if (el->visible) {
			//
			el->render (el->left, el->top);
		}
	}

	/* Render Elements in Stack TODO: to come up with something else */

	for (auto item : drawStack) {

		item->render (item->left, item->top);

	}

	drawStack.clear();
	atlasShader->unbind();

	if (dialogOpened || freezUI)
		return;

	/* */

	for (const auto &kvp : elements) {

		std::shared_ptr<UIElement> el = kvp.second;

		if (!el->visible) continue;

		if (!pointInRect (app->mouseX, app->mouseY, el->left,
						  el->top, el->width, el->height))
		{
			
			el->enter = false;
			el->click = false;

			continue;

		}

		if (!el->enabled || (el->parent != 0 && !el->parent->enabled)) {

			el->enter = false;
			el->click = false;

			continue;
		}

		el->enter = true;
		
		if (app->mouseButton == mouseLeft) el->click = true;
		else el->click = false;

	}

}

/* Events */

/**
 *
 */

void UIManager::mouseMove (int x, int y, int button) {
	
	for (const auto &kvp : elements) {

		kvp.second->mouseMove (x, y, button);

	}
	
}

/**
 *
 */

void UIManager::mouseDown (int x, int y, int button) {
	
	for (const auto &kvp : elements) {

		kvp.second->mouseDown (x, y, button);

	}
	
}

/**
 *
 */

void UIManager::dblClick (int x, int y, int button) {
	
	for (const auto &kvp : elements) {

		kvp.second->dblClick (x, y, button);

	}
	
}

/**
 *
 */

void UIManager::mouseUp (int x, int y, int button) {
	
	for (const auto &kvp : elements) {

		kvp.second->mouseUp (x, y, button);

	}

	/* Unfocuse all Elements */

	for (auto el : unfocusedElements) {

		if (!el->focused)
			continue;

		el->focused = false;

		if (el->onUnfocused != nullptr)
			el->onUnfocused (el);
	}

	unfocusedElements.clear();
	
}

/**
 *
 */

void UIManager::keyPressed (int key) {

	for (const auto &kvp : elements) {

		kvp.second->keyPressed (key);

	}

}

/**
 *
 */

void UIManager::resized (int width, int height) {
	
	for (const auto &kvp : elements) {

		kvp.second->resized (width, height);

	}

}

