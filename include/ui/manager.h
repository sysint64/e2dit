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

#ifndef H_E2DIT_UI_MANAGER
#define H_E2DIT_UI_MANAGER

#include <vector>
#include <glm/glm.hpp>
#include "ui/element.h"
#include "renderer/shader.h"
#include "renderer/data_render.h"
#include "renderer/base_object.h"

/* */

class UIManager {
private:
	int lastId = 0;

public:
	static DataRender *uiDataRender;

	std::map<int, UIElement*> elements;
	std::vector<glm::vec4>  scissorStack;

	UIElement *focusedElement;
	Shader    *texShader;
	Shader    *colorShader;

	 UIManager();
	~UIManager();

	void render();

	inline void addElement (UIElement *el) {

		el->manager = this;
		el->id = lastId;
		elements[lastId] = el;

		lastId++;

	}

	inline void deleteElement (UIElement *el) {

		elements[el->id] = nullptr;
		//delete el;

	}

	void pushScissor (int sx, int sy, int sw, int sh);
	void popScissor();
	void setScissor();

	/* Events */

	void dblClick    (int x, int y, int Shift);

	void mouseDown   (int x, int y, int Shift);
	void mouseMove   (int x, int y, int Shift);
	void mouseUp     (int x, int y, int Shift);

	void keyDown     (int key);
	void resized     (int width, int height);
	void keyPressed  (int key);
	void textEntered (int key);

	void step();
};

#endif
