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
 * Calculate Tex Coords for Icons
 */ 

template <int count>
UIIcons<count>::UIIcons (UIManager *manager, std::unique_ptr<Texture> tex, float sizeIcon) {
		
	this->sizeIcon = sizeIcon;
	this->tex      = std::move (tex);
	this->manager  = manager;

	/* Calc Size in Tex Coord System */

	width  = sizeIcon / this->tex->width;
	height = sizeIcon / this->tex->height;

	for (int i = 0; i < count; i++)
		for (int j = 0; j < count; j++) {

			/* Calculate Offsets in Texture Coordinate System */

			offsetsX[i][j] = (float)(9+i*sizeIcon+3*i) / this->tex->width;
			offsetsY[i][j] = (float)(9+j*sizeIcon+3*j) / this->tex->height;

		}

}

/**
 * Render Icon
 *
 * @param  x: Left Position of Icon
 * @param  y: Top  Position of Icon
 * @param ox: Offset by X on Icons Table
 * @param ox: Offset by Y on Icons Table
 */

template <int count>
void UIIcons<count>::render (int x, int y, int ox, int oy, BaseObject *iconElement) {

	glUniform1i        (manager->atlasShader->locations["Texture"], 2);
	
	glUniformMatrix4fv (manager->atlasShader->locations["MVP"]   , 1, GL_FALSE, &(iconElement->MVPMatrix[0][0]));
	glUniform2f        (manager->atlasShader->locations["Size"]  , width, height);
	glUniform2f        (manager->atlasShader->locations["Offset"], offsetsX[ox][oy],
																   offsetsY[ox][oy]);
	
	iconElement->setPosition (glm::vec2 (x, y));
	iconElement->setScale    (glm::vec2 (sizeIcon, sizeIcon));
	iconElement->render();

	glUniform1i (manager->atlasShader->locations["Texture"], manager->themeTexID);

}

template class UIIcons<ICONS_COUNT>;

/**
 * Constructor, Create Root Element and
 */

UIManager::UIManager (Shader *atlasShader, Shader *colorShader, UITheme *theme) :
	atlasShader (atlasShader), colorShader (colorShader), theme (theme)

{

	/* Create Root of Elements */

	root = std::make_unique<UIElement> (this);
	root->isRoot = true;

}

/**
 * Insert new UI Element in Root
 * @param el: new UI Element
 */

void UIManager::addElement (std::unique_ptr<UIElement> el) {

	root->addElement (std::move(el));

}

/**
 * Delete element el from manager and memory
 * @param el: UI Element
 */

void UIManager::deleteElement (std::unique_ptr<UIElement> el) {


}

/**
 * Delete element el from manager and memory by id
 * @param id: id of UI Element
 */

void UIManager::deleteElement (const int id) {



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
	glBindTexture   (GL_TEXTURE_2D, icons->tex->handle);

	/* Bind UI VBO Data Render */

	uiDataRender->bind();

	glUniform1i (atlasShader->locations["Texture"], themeTexID);
	glUniform1f (atlasShader->locations["Alpha"  ], 1.0f);

	cursor = CursorIco::Normal;

	/* Render Root */

	root->render();
	root->poll();

	atlasShader->unbind();
	app->cursor->set (cursor);

	/* Render Elements in Stack TODO: to come up with something else */

	/*for (auto item : drawStack) {

		item->render (item->left, item->top);

	}*/


}

/* Events */

/**
 * On Mouse Move Event
 *
 * @param x: Mouse X Coord
 * @param y: Mouse Y Coord
 * @param button: Mouse Button
 */

void UIManager::mouseMove (int x, int y, int button) {

	root->mouseMove (x, y, button);
	
}

/**
 * On Mouse Down Event
 *
 * @param x: Mouse X Coord
 * @param y: Mouse Y Coord
 * @param button: Mouse Button
 */

void UIManager::mouseDown (int x, int y, int button) {
	
	root->mouseDown (x, y, button);
	
}

/**
 * On Mouse Double Click Event
 *
 * @param x: Mouse X Coord
 * @param y: Mouse Y Coord
 * @param button: Mouse Button
 */

void UIManager::dblClick (int x, int y, int button) {
	
	root->dblClick (x, y, button);
	
}

/**
 * On Mouse Up Event
 *
 * @param x: Mouse X Coord
 * @param y: Mouse Y Coord
 * @param button: Mouse Button
 */

void UIManager::mouseUp (int x, int y, int button) {

	unfocus();
	root->mouseUp (x, y, button);
	
}

/**
 * Unfocus Elements in unfocusedElements
 */

void UIManager::unfocus() {

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
 * On Key Pressed Events
 * @param key: Code of Pressed key
 */

void UIManager::keyPressed (int key) {

	if (focusedElement != nullptr) {

		if (pressed (keyShift) && pressed (keyTab) && focusedElement->prev != nullptr) {
			
			focusedElement->prev->focus();
			unfocus();

		} else

		if (pressed (keyTab) && focusedElement->next != nullptr) {

			focusedElement->next->focus();
			unfocus();

		}

		if (pressed (keyEnter)) {

			focusedElement->keyClick = true;

		}

	}

	root->keyPressed (key);

}

/**
 * On Key Released Events
 * @param key: Code of Released key
 */

void UIManager::keyReleased (int key) {

	root->keyReleased (key);

}

/**
 * On Key Enered Events
 * @param key: Code of Entered key
 */

void UIManager::textEntered (int key) {
	
	root->textEntered (key);

}

/**
 * On Resized Event
 *
 * @param width : New Screen Width
 * @param height: New Screen Height
 */

void UIManager::resized (int width, int height) {
	
	root->resized (width, height);

}
