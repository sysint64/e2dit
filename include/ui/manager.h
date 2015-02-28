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
#include "ui/theme.h"
#include "renderer/shader.h"
#include "renderer/texture.h"
#include "renderer/data_render.h"
#include "renderer/base_object.h"
#include "utility/application.h"
#include "utility/math.h"
#include "utility/input.h"

#define ICONS_COUNT 11

struct {

	std::shared_ptr<Texture> tex;

	float offsetsX[ICONS_COUNT][ICONS_COUNT];
	float offsetsY[ICONS_COUNT][ICONS_COUNT];
	float width; float height;
	float sizeIcon;

} typedef UIIcons;

struct {

	std::shared_ptr<Texture> tex;

	float offsetsX[ICONS_COUNT][ICONS_COUNT];
	float offsetsY[ICONS_COUNT][ICONS_COUNT];
	float width; float height;
	float sizeIcon;

} typedef UIToolIcons;

/* */

class UIManager {
private:
	int lastId = 0;
	Application *app = Application::getInstance();

public:

	/* UI State */
	
	bool dialogOpened = false;
	bool freezUI	  = false;

	/* Icons */

	UIIcons     icons;
	UIToolIcons toolIcons;

	/* Render */

	DataRender *uiDataRender;
	static const int   themeTexID = 2;
	float disabledAlpha = 0.65f;

	/* */

	std::map<int, std::shared_ptr<UIElement>> elements;
	std::vector<glm::vec4>    scissorStack;
	std::vector<UIElement*>   drawStack;
	std::vector<UIElement*>   unfocusedElements;

	/* */

	UIElement *focusedElement;
	Shader    *atlasShader;
	Shader    *colorShader;
	UITheme	  *theme;

	 UIManager (Shader *atlasShader, Shader *colorShader, UITheme theme);
	~UIManager();

	void render();

	/* Manager */

	void addElement    (std::shared_ptr<UIElement> el);
	void deleteElement (std::shared_ptr<UIElement> el);
	void deleteElement (const int  id);

	/* Scissor */

	void pushScissor (int sx, int sy, int sw, int sh);
	void popScissor();
	void setScissor();

	/* Events */

	void dblClick    (int x, int y, int button);

	void mouseDown   (int x, int y, int button);
	void mouseMove   (int x, int y, int button);
	void mouseUp     (int x, int y, int button);

	void keyDown     (int key);
	void resized     (int width, int height);
	void keyPressed  (int key);
	void textEntered (int key);

	void step();
};

#endif
