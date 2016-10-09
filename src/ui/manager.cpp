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

/* Calculate Tex Coords for Icons */

template <int count>
ui::Icons<count>::Icons (Manager *manager, std::unique_ptr<gapi::Texture> tex, float sizeIcon, int margin, int spacing) {

	this->sizeIcon = sizeIcon;
	this->tex      = std::move (tex);
	this->manager  = manager;

	/* Calc Size in Tex Coord System */

	width  = sizeIcon / this->tex->width;
	height = sizeIcon / this->tex->height;

	for (int i = 0; i < count; ++i)
		for (int j = 0; j < count; ++j) {

			/* Calculate Offsets in Texture Coordinate System */

			offsetsX[i][j] = (float)(margin+i*sizeIcon+spacing*i) / this->tex->width;
			offsetsY[i][j] = (float)(margin+j*sizeIcon+spacing*j) / this->tex->height;

		}

}

template <int count>
void ui::Icons<count>::render (int x, int y, int ox, int oy, gapi::BaseObject *iconElement) {

	glUniform1i (manager->atlasShader->locations["Texture"], texBindId);
	glUniform2f (manager->atlasShader->locations["Size"]   , width, height);
	glUniform2f (manager->atlasShader->locations["Offset"] , offsetsX[ox][oy],
	                                                         offsetsY[ox][oy]);

	iconElement->setPosition (glm::vec2 (x, y));
	iconElement->setScale    (glm::vec2 (sizeIcon, sizeIcon));

	iconElement->updateModelMatrix();
	glUniformMatrix4fv (manager->atlasShader->locations["MVP"], 1, GL_FALSE, &(iconElement->MVPMatrix[0][0]));
	iconElement->render();

	glUniform1i (manager->atlasShader->locations["Texture"], manager->themeTexID);

}

template class ui::Icons<ICONS_COUNT>;
template class ui::Icons<TOOL_ICONS_COUNT>;

ui::Manager::Manager (sf::Window *window,
                      gapi::Shader *atlasMaskShader,
                      gapi::Shader *atlasShader,
                      gapi::Shader *colorShader,
                      Theme *theme)
		: window(window),
		  atlasMaskShader(atlasMaskShader),
		  atlasShader(atlasShader),
		  colorShader(colorShader),
		  theme(theme)
{
	/* Create Root of Elements */
	root = std::make_unique<Widget> (this);
	root->isRoot = true;
}

ui::Manager *ui::Manager::getInstance() {
	static Manager *manager = nullptr;

	if (manager != nullptr)
		return manager;

	manager = new Manager();
	return manager;
}

ui::Widget *ui::Manager::findElement (const std::string &name) {
	auto element = root->findElement (name);

	if (!element)
		app->log.ewrite ("Error: can't find symbol with name %s\n", name.c_str());

	return element;
}

void ui::Manager::addElement (std::unique_ptr<Widget> el) {
	root->addElement (std::move(el));
}

void ui::Manager::deleteElement (std::unique_ptr<Widget> el) {


}

void ui::Manager::deleteElement (const int id) {



}

/* Push new scissor to stack */

void ui::Manager::pushScissor (int sx, int sy, int sw, int sh) {

	if (scissorStack.size() == 0)
		glEnable (GL_SCISSOR_TEST);

	scissorStack.push_back(glm::vec4(sx, app->windowHeight-sy-sh, sx+sw, app->windowHeight-sy));
	setScissor();

}

/* Pop scissor from stack */

void ui::Manager::popScissor() {

	scissorStack.pop_back();

	if (scissorStack.size() == 0) glDisable (GL_SCISSOR_TEST);
	else setScissor();

}

/* Set OpenGL Scissor by last scissor from stack */

void ui::Manager::setScissor() {

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

void ui::Manager::poll() {

	//if (freezUI)
	//	return;

	/* Reset states */

	for (int i = elementsStack.size()-1; i >= 0; --i) {

		auto el = elementsStack[i];

		if (el == nullptr)
			continue;

		el->setCursor();
		el->enter = false;

		if (!el->visible)
			continue;

		int w = el->overWidth  >= 0 ? el->overWidth  : el->width;
		int h = el->overHeight >= 0 ? el->overHeight : el->height;

		el->over = el->parent->over && pointInRect (app->mouseX, app->mouseY, el->absLeft, el->absTop, w, h);

	}

	/* State */

	underMouse = nullptr;

	for (auto const &kvp : disablePollMap) {

		if (kvp.second)
			return;

	}

	Widget *found = nullptr;

	for (int i = elementsStack.size()-1; i >= 0; --i) {

		auto el = elementsStack[i];

		if (found != nullptr && !el->overlay)
			continue;

		if (el == nullptr || !el->over || !el->visible)
			continue;

		if (found != nullptr) {
			found->enter = false;
			found->click = false;
		}

		if (pointInRect (app->mouseX, app->mouseY, el->absLeft, el->absTop,
		                 el->width, el->height) && el->enabled)
		{
			el->enter = true;
			underMouse = el;
		}

		el->click = (el->click || el->focused) && el->enter && app->mouseButton == mouseLeft;

		if (el->enter)
			found = el;

	}

}

void ui::Manager::render() {

	/* Bind Theme Skin */

	atlasMaskShader->bind();

	glUniform1i (atlasMaskShader->locations["Mask" ], themeTexID);
	glUniform1f (atlasMaskShader->locations["Alpha"], 1.0f);

	atlasShader->bind();

	glActiveTexture (GL_TEXTURE3);
	glBindTexture   (GL_TEXTURE_2D, theme->skin->handle);

	glActiveTexture (GL_TEXTURE2);
	glBindTexture   (GL_TEXTURE_2D, icons->tex->handle);

	glActiveTexture (GL_TEXTURE5);
	glBindTexture   (GL_TEXTURE_2D, toolIcons->tex->handle);

	icons    ->texBindId = 2;
	toolIcons->texBindId = 5;

	/* Bind UI VBO Data Render */

	uiDataRender->bind();

	glUniform1i (atlasShader->locations["Texture"], themeTexID);
	glUniform1f (atlasShader->locations["Alpha"  ], 1.0f);

	cursor = CursorIco::Normal;

	/* Render Root */

	root->render();
	bool overlayPoll = false;

	for (const auto &element : overlayElements) {
		if (element && element->visible) {
			element->over = true;
			element->render();
		}
	}

	poll();
	root->progress();

	atlasShader->unbind();
	app->cursor->set (cursor);

}

/* Events */

void ui::Manager::mouseMove (int x, int y, int button) {

	root->mouseMove (x, y, button);

}

void ui::Manager::mouseDown (int x, int y, int button) {

	for (int i = elementsStack.size()-1; i >= 0; --i) {

		auto el = elementsStack[i];

		if (el == nullptr) // #TODO: Remove empty element from stack
			continue;

		if (el->enter) {

			el->click = true;
			break;
		}

	}

	root->mouseDown (x, y, button);

}

void ui::Manager::mouseWheel (int dx, int dy) {

	root->mouseWheel (dx, dy);

}

void ui::Manager::dblClick (int x, int y, int button) {

	root->dblClick (x, y, button);

}

void ui::Manager::mouseUp (int x, int y, int button) {

	for (int i = elementsStack.size()-1; i >= 0; --i) {

		auto el = elementsStack[i];

		if (el == nullptr) // #TODO: Remove empty element from stack
			continue;

		if (el->enter) {

			el->focus();
			break;

		}

	}

	root->mouseUp (x, y, button);
	int length = root->elements.size();
	blur();

}

/* Unfocus Elements in unfocusedElements */

void ui::Manager::blur() {

	for (auto el : unfocusedElements) {

		if (!el->focused)
			continue;

		el->focused = false;

		if (el->onBlur != nullptr)
			el->onBlur (el);
	}

	unfocusedElements.clear();

}

void ui::Manager::keyPressed (int key) {

	if (focusedElement != nullptr) {

		if (keyboard::pressed (keyShift) && keyboard::pressed (keyTab) && focusedElement->prev != nullptr) {

			focusedElement->prev->focus();
			blur();

		} else

		if (keyboard::pressed (keyTab) && focusedElement->next != nullptr) {

			focusedElement->next->focus();
			blur();

		}

		if (keyboard::pressed (keyEnter)) {

			focusedElement->keyClick = true;

		}

	}

	root->keyPressed (key);

}

void ui::Manager::keyReleased (int key) {
	root->keyReleased (key);
}

void ui::Manager::textEntered (int key) {

	root->textEntered (key);

}

void ui::Manager::resized (int width, int height) {
	root->resized (width, height);
}
