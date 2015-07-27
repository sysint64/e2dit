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
#include "ui/cursor.h"

#include "renderer/shader.h"
#include "renderer/texture.h"
#include "renderer/data_render.h"
#include "renderer/base_object.h"

#include "utility/application.h"
#include "utility/math.h"
#include "utility/input.h"
#include "utility/ui.h"

#define ICONS_COUNT			11
#define TOOL_ICONS_COUNT	4

class UIManager;

/**
 * @param count: Count of Icons
 * @param size : Size of Tile Icon
 */

template <int count>
class UIIcons {
public:

	std::unique_ptr<Texture> tex;

	float offsetsX[count][count];
	float offsetsY[count][count];
	float width; float height;
	float sizeIcon;

	UIManager *manager;

	UIIcons (UIManager *manager, std::unique_ptr<Texture> tex, float sizeIcon);
	void render (int x, int y, int ox, int oy, BaseObject *iconElement);

};

typedef UIIcons<ICONS_COUNT> UIMainIcons;

/**
 * Manager of UI Elements
 */

class UIManager {
private:
	Application *app = Application::getInstance();
	std::vector<std::string> debugStrings;

public:

	int lastId = 0;

	/* UI State */

	bool dialogOpened = false;
	bool freezUI	  = false;
	bool focused      = false;
	bool lockCursor   = false;

	/* Icons */

	std::unique_ptr<UIMainIcons> icons;
	std::unique_ptr<UIIcons<4 >> toolIcons;

	/* Render */

	DataRender *uiDataRender;
	static const int themeTexID = 3;
	float disabledAlpha = 0.65f;

	/* */

	std::vector<glm::vec4>  scissorStack;
	std::vector<UIElement*> drawStack;
	std::vector<UIElement*> unfocusedElements;
	std::vector<UIElement*> elementsStack; // For poll elements

	std::unique_ptr<UIElement> root;

	/* */

	UIElement *focusedElement = nullptr;
	UIElement *underMouse     = nullptr;
	Shader    *atlasShader;
	Shader    *colorShader;
	UITheme	  *theme;

	CursorIco cursor = CursorIco::Normal;
	UIManager (Shader *atlasShader, Shader *colorShader, UITheme *theme);

	void render();

	/* Manage */

	void addElement    (std::unique_ptr<UIElement> el);
	void deleteElement (std::unique_ptr<UIElement> el);
	void deleteElement (const int  id);
	void unfocus();
	void poll();

	/* Scissor */

	void pushScissor (int sx, int sy, int sw, int sh);
	void popScissor();
	void setScissor();

	/* Events */

	void dblClick    (int x, int y, int button);
	void mouseDown   (int x, int y, int button);
	void mouseUp     (int x, int y, int button);
	void mouseMove   (int x, int y, int button);
	void mouseWheel  (int dx, int dy);

	void keyDown     (int key);
	void resized     (int width, int height);
	void keyPressed  (int key);
	void keyReleased (int key);
	void textEntered (int key);

	void step();

	/* Debug */

	void putString (std::string str, float r = 1.f, float g = 1.f, float b = 1.f);
};

#endif
