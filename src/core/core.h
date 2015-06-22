/*
 * E2DIT - is a 2D Map Editor to create a levels for 2d games
 * Copyright (C) 2015 Kabylin Andrey <andrey@kabylin.ru>

 * This file is part of E2DIT.

 * E2DIT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * E2DIT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with E2DIT.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Kabylin Andrey <andrey@kabylin.ru>
 */

#ifndef E2DIT_CORE_H
#define E2DIT_CORE_H

#include "utility/application.h"
#include <glm/glm.hpp>

#include "ui/manager.h"
#include "ui/button.h"
#include "ui/edit.h"
#include "ui/panel.h"
#include "ui/theme.h"

#include "renderer/sprite.h"
#include "renderer/shader.h"
#include "renderer/texture.h"

#include "boost/assign/std/vector.hpp"

class Application;
class Core {
public:

	Application *app = Application::getInstance();

	/* UI */

	std::unique_ptr<UIManager> uiManager;
	std::unique_ptr<UITheme>   uiTheme;
	std::unique_ptr<Texture>   skin;

	std::unique_ptr<Shader>    atlasShader;
	std::unique_ptr<Shader>    colorShader;

	std::unique_ptr<UIButton>  button;
	std::unique_ptr<UIPanel>  panel;
	std::unique_ptr<UIEdit>    edit;

	/* Constructor */

	 Core();
	~Core();

	/* Events */

	void onKeyPressed  (int key);
	void onKeyReleased (int key);
	void onTextEntered (unsigned int key);
	void onMouseMove   (int x, int y);
	void onMouseDown   (int x, int y, int button);
	void onMouseUp     (int x, int y, int button);
	void onDblClick    (int x, int y, int button);
	void onMouseWheel  (int x, int y, int button);
	void onResize      (int width, int height);

	/* */

	void render();
	void step();

};

#endif
