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
	
	skin    = std::make_unique<Texture> ("res/ui/dark/controls.png");
	uiTheme = std::make_unique<UITheme> ("res/ui/dark/controls.e2t", ReadType::Text, skin.get(), font);

	std::vector<std::string> atlasLocations; atlasLocations += "MVP", "Size", "Offset", "Texture", "Alpha";
	std::vector<std::string> colorLocations; colorLocations += "MVP", "Color";

	atlasShader = std::make_unique<Shader> ("res/glsl/GL3/tex_atlas.glsl", atlasLocations);
	colorShader = std::make_unique<Shader> ("res/glsl/GL3/colorize.glsl" , colorLocations);

	uiManager   = std::make_unique<UIManager> (atlasShader.get(), colorShader.get(), uiTheme.get());
	uiManager->uiDataRender = new SpriteData (false, false, true);

	std::unique_ptr<Texture> iconsTex = std::make_unique<Texture> ("res/ui/icons.png");
	uiManager->icons = std::make_unique<UIMainIcons> (uiManager.get(), std::move (iconsTex), 18.f);

	//uiManager->icons.tex = std::make_unique<Texture> ("res/ui/icons.png");

	std::unique_ptr<UIButton> b1 = std::make_unique<UIButton> (uiManager.get());
	std::unique_ptr<UIButton> b2 = std::make_unique<UIButton> (uiManager.get());
	std::unique_ptr<UIButton> b3 = std::make_unique<UIButton> (uiManager.get());

	b1->caption = L"Test";
	b1->left  = 0;
	b1->top   = 0;
	b1->width = 200;
	b1->showIcon = true;
	b1->showIcon2 = true;
	b1->iconOffset[0] = 1;
	b1->iconOffset[1] = 1;
	b1->icon2Offset[0] = 3;
	b1->icon2Offset[1] = 2;
	//uiManager->addElement (std::move(b1));

	b2->caption = L"Test";
	b2->left  = 300;
	b2->top   = 100;
	b2->width = 200;
	b2->showIcon = true;
	b2->showIcon2 = true;
	b2->iconOffset[0] = 2;
	b2->iconOffset[1] = 1;
	b2->icon2Offset[0] = 3;
	b2->icon2Offset[1] = 3;
	//uiManager->addElement (std::move(b2));

	b3->caption = L"Test";
	b3->left = 700;
	b3->top  = 100;
	b3->width = 200;

	b3->showIcon = true;
	b3->showIcon2 = true;
	b3->iconOffset[0] = 3;
	b3->iconOffset[1] = 5;

	b3->icon2Offset[0] = 3;
	b3->icon2Offset[1] = 4;

	//uiManager->addElement (std::move(b3));
	
	/* */

	std::unique_ptr<UIEdit> e1 = std::make_unique<UIEdit> (uiManager.get());
	e1->text  = L"Test";
	e1->left  = 100;
	e1->top   = 200;
	e1->width = 200;
	e1->trackMode = true;
	e1->trackRange = glm::vec2(-10, 20);

	//uiManager->addElement (std::move(e1));

	std::unique_ptr<UIPanel> p1 = std::make_unique<UIPanel> (uiManager.get());

	p1->left   = 200;
	p1->top    = 10;
	p1->width  = 600;
	p1->height = 600;
	p1->background = UIPanel::Background::Action;

	auto _p1 = p1.get();

	uiManager->addElement (std::move(p1));

	_p1->addElement (std::move(b1));
	_p1->addElement (std::move(b2));
	_p1->addElement (std::move(b3));
	_p1->addElement (std::move(e1));

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
