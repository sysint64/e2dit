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

#include "ui/checkbox.h"
#include "ui/listmenu.h"
#include "ui/dropmenu.h"
#include "ui/image.h"
#include "ui/label.h"
#include "ui/grouped.h"
#include "ui/menuitems.h"
#include "ui/treelist.h"
#include "ui/tabs.h"
#include "ui/toolbar.h"
#include "ui/dialog.h"
#include "ui/loader.h"
#include "ui/stack_layout.h"
#include "ui/dialogs/color_dialog.h"

#include "utility/strings_res.h"
#include "resources.h"
#include "settings.h"
#include <boost/algorithm/string.hpp>

Core::Core (sf::Window *window) {

	this->window = window;
	app->screenCamera = std::make_unique<gapi::Camera>(app->screenWidth, app->screenHeight);

	//uiManager = std::make_shared<Manager>();
	auto font12 = ftglCreateTextureFont ((app->resPath+"/fonts/ttf-dejavu/DejaVuSans.ttf").c_str());
	auto font14 = ftglCreateTextureFont ((app->resPath+"/fonts/ttf-dejavu/DejaVuSans.ttf").c_str());
	auto font16 = ftglCreateTextureFont ((app->resPath+"/fonts/ttf-dejavu/DejaVuSans.ttf").c_str());

	ftglSetFontFaceSize (font12, 12, 12);
	ftglSetFontCharMap  (font12, ft_encoding_unicode);

	ftglSetFontFaceSize (font14, 14, 14);
	ftglSetFontCharMap  (font14, ft_encoding_unicode);

	ftglSetFontFaceSize (font16, 16, 16);
	ftglSetFontCharMap  (font16, ft_encoding_unicode);

	R::init (app);

	//skin    = std::make_unique<Texture> (app->resPath+"/ui/skins/light/controls.png");
	uiTheme = std::make_unique<ui::Theme> (app->resPath+"/ui/skins/light/controls.e2t", DataMap::ReadType::Text, R::Textures::skin.get(), font12);
	uiTheme->fontHeight   = 12;
	uiTheme->font12Height = 12;
	uiTheme->font14Height = 14;
	uiTheme->font16Height = 16;
	uiTheme->font12 = font12;
	uiTheme->font14 = font14;
	uiTheme->font16 = font16;

	uiManager->window = window;
	uiManager->theme  = uiTheme.get();

	uiManager->atlasShader     = R::Shaders::atlasShader    .get();
	uiManager->atlasMaskShader = R::Shaders::atlasMaskShader.get();
	uiManager->colorShader     = R::Shaders::colorShader    .get();
//	uiManager   = std::make_unique<Manager> (window,
//	                                           R::Shaders::atlasMaskShader.get(),
//	                                           R::Shaders::atlasShader    .get(),
//	                                           R::Shaders::colorShader    .get(),
//	                                           uiTheme.get());

	uiManager->uiDataRender = new gapi::SpriteData(false, false, true);

	std::unique_ptr<gapi::Texture> iconsTex = std::make_unique<gapi::Texture> (app->resPath+"/ui/icons/icons.png");
	uiManager->icons = std::make_unique<ui::UIMainIcons> (uiManager, std::move (iconsTex), 18.f, 9, 3);

	iconsTex = std::make_unique<gapi::Texture> (app->resPath+"/ui/icons/main_toolbar_icons.png");
	uiManager->toolIcons = std::make_unique<ui::UIToolIcons> (uiManager, std::move (iconsTex), 48.f, 1, 1);

	stringsRes = std::make_unique<StringRes>();

	stringsRes->addResource (app->resPath+"/strings/en/main.e2t");
	stringsRes->addResource (app->resPath+"/strings/en/menu.e2t");
	stringsRes->addResource (app->resPath+"/strings/en/dialogs/color_dialog.e2t");

	//std::unique_ptr<ui::Loader> loader = std::make_unique<ui::Loader> (uiManager, stringsRes.get(), "test.e2t");
	ui::Loader(uiManager, stringsRes.get(), "test.e2t");
	colorDialog = std::make_unique<ui::ColorDialog> (uiManager, stringsRes.get(), "dialogs/color_dialog.e2t");

	//

	editor->initUI();

	settings->shortKeys["Shift Ctrl Up"] = new Command([this]() {
		editor->splitView(ui::Orientation::Horizontal);
	});

	settings->shortKeys["Shift Ctrl Left"] = new Command([this]() {
		editor->splitView(ui::Orientation::Vertical);
	});

	//StringRes testRes;
	//testRes.addResource ("../res/strings/eng/menu.e2t");
	//puts ("Parse resource:");
	//puts (testRes.parseResource("Hello \"@menu.test\" Test from Item1: \"@menu.item1.caption\"").c_str());
	//puts (testRes.parseResource("test @menu.test teest").c_str());

	//uiManager->icons.tex = std::make_unique<Texture> ("res/ui/icons.png");

	/*

	std::unique_ptr<Checkbox> cb1 = std::make_unique<Checkbox> (uiManager.get());
	cb1->top = 100;
	cb1->left = 50;

	//

	std::unique_ptr<Tab> tab = std::make_unique<Tab>(uiManager.get());
	tab->caption = L"Tab 1";
	tab->width = 80;

	std::unique_ptr<Tab> tab3 = std::make_unique<Tab>(uiManager.get());
	tab3->caption = L"Tab 3";
	tab3->width = 80;

	std::unique_ptr<Tab> tab2 = std::make_unique<Tab>(uiManager.get());
	tab2->caption = L"Tab 2";
	tab2->width = 80;

	std::unique_ptr<Grouped> tabs = std::make_unique<Grouped> (uiManager.get(), false, 2);
	auto _tabs = tabs.get();

	_tabs->left = 5;
	_tabs->top  = 5;
	_tabs->width = 400;
	_tabs->style = "tabs";

	tab2->showIcon = true;
	tab2->iconOffset[0] = 8;
	tab2->iconOffset[1] = 9;

	auto _tab = tab.get();

	_p4  ->addElement (std::move(tabs));
	_tabs->addElement (std::move(tab));
	_tabs->addElement (std::move(tab2));
	_tabs->addElement (std::move(tab3));

	_tabs->checkElement (_tab);

	/*std::unique_ptr<Image> image1 = std::make_unique<Image> (uiManager.get());
	image1->left = 250;
	image1->top  = 150;
	image1->loadImage ("../set.png");
	image1->cropImage (16, 290, 350, 258);
	_p3->addElement (std::move(image1));*/

	/*std::unique_ptr<Label> label1 = std::make_unique<Label> (uiManager.get());
	label1->left = 10;
	label1->top  = 40;
	label1->caption = L"Hello World!";
	_p3->addElement (std::move(label1));

	std::unique_ptr<Dialog> dialog = std::make_unique<Dialog> (uiManager.get());

	dialog->left   = 100;
	dialog->top    = 100;
	dialog->width  = 300;
	dialog->height = 200;

	uiManager->addElement (std::move (dialog));*/

}

/* */

void Core::render() {
	editor->render();

	app->screenCamera->update();
	uiManager->render();
	colorDialog->render();
	app->screenCamera->update();
}

void Core::step() {
}

void Core::handleShortKeys (sf::Event::KeyEvent event) {
	std::string hash = "";

	if (event.shift)   hash += "Shift ";
	if (event.alt)     hash += "Alt ";
	if (event.control) hash += "Ctrl ";

	hash += keyboard::map[event.code];
	boost::trim_right(hash);

	if (settings->shortKeys.find(hash) == settings->shortKeys.end())
		return;

	settings->shortKeys[hash]->execute();
}
