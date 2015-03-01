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

#include "core.h"

using namespace boost::assign;

Core::Core() {

	app->screenCamera = std::make_unique<Camera>(app->screenWidth, app->screenHeight);

	//uiManager = std::make_shared<UIManager>();
	auto font = ftglCreateTextureFont ("res/fonts/ttf-dejavu/DejaVuSans.ttf");

	ftglSetFontFaceSize (font, 12, 12);
	ftglSetFontCharMap  (font, ft_encoding_unicode);
	
	skin    = std::make_shared<Texture>  ("res/ui/dark/controls.png");
	uiTheme = std::make_shared<UITheme>  ("res/ui/dark/controls.e2t", ReadType::Text, skin.get(), font);

	std::vector<std::string> atlasLocations; atlasLocations += "MVP", "Size", "Offset", "Texture", "Alpha";
	std::vector<std::string> colorLocations; colorLocations += "MVP", "Color";

	atlasShader = std::make_shared<Shader> ("res/glsl/GL3/tex_atlas.glsl", atlasLocations);
	colorShader = std::make_shared<Shader> ("res/glsl/GL3/colorize.glsl" , colorLocations);

	uiManager   = std::make_shared<UIManager> (atlasShader.get(), colorShader.get(), uiTheme.get());
	uiManager->uiDataRender = new SpriteData (false, false, true);

	button      = std::make_shared<UIButton>  (uiManager.get());
	button->caption = L"Test";
	uiManager->addElement (button);

	//uiTheme = std::make_shared<UITheme> ("controls.e2t", ReadType::Text, skin.get(), font);
	//DataMap test ("controls.e2t", ReadType::Text);

	//std::cout << uiTheme->element["general"].params["font"][0].str << std::endl;
	//std::cout << std::endl << uiTheme->element["scrollhorizontal"].params["left"][3].num << std::endl;

}

Core::~Core() {
}

/* */

void Core::render() {

	app->screenCamera->update();
	uiManager->render();

}

void Core::step() {

}
