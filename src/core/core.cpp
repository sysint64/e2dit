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

#include "utility/strings_res.h"

Core::Core() {

	app->screenCamera = std::make_unique<Camera>(app->screenWidth, app->screenHeight);

	//uiManager = std::make_shared<UIManager>();
	auto font = ftglCreateTextureFont ("../res/fonts/ttf-dejavu/DejaVuSans.ttf");

	ftglSetFontFaceSize (font, 12, 12);
	ftglSetFontCharMap  (font, ft_encoding_unicode);

	skin    = std::make_unique<Texture> ("../res/ui/skins/light/controls.png");
	uiTheme = std::make_unique<UITheme> ("../res/ui/skins/light/controls.e2t", DataMap::ReadType::Text, skin.get(), font);
	uiTheme->fontHeight = 12;

	std::vector<std::string> atlasMaskLocations {"MVP", "Size", "Offset", "Texture", "Alpha", "Mask", "MaskOffset", "MaskSize"};
	std::vector<std::string> atlasLocations     {"MVP", "Size", "Offset", "Texture", "Alpha"};
	std::vector<std::string> colorLocations     {"MVP", "Color"};

	atlasMaskShader = std::make_unique<Shader> ("../res/glsl/GL2/tex_atlas_mask.glsl", atlasMaskLocations);
	atlasShader     = std::make_unique<Shader> ("../res/glsl/GL2/tex_atlas.glsl"     , atlasLocations);
	colorShader     = std::make_unique<Shader> ("../res/glsl/GL2/colorize.glsl"      , colorLocations);

	uiManager   = std::make_unique<UIManager> (atlasMaskShader.get(), atlasShader.get(), colorShader.get(), uiTheme.get());
	uiManager->uiDataRender = new SpriteData (false, false, true);

	std::unique_ptr<Texture> iconsTex = std::make_unique<Texture> ("../res/ui/icons/icons.png");
	uiManager->icons = std::make_unique<UIMainIcons> (uiManager.get(), std::move (iconsTex), 18.f, 9, 3);

	iconsTex = std::make_unique<Texture> ("../res/ui/icons/main_toolbar_icons.png");
	uiManager->toolIcons = std::make_unique<UIToolIcons> (uiManager.get(), std::move (iconsTex), 48.f, 1, 1);

	std::unique_ptr<UILoader> loader = std::make_unique<UILoader> (uiManager.get(), "../res/ui/layouts/test.e2t");

	StringRes testRes;
	testRes.addResource ("../res/strings/eng/menu.e2t");
	puts ("Parse resource:");
	puts (testRes.parseResource("Hello \"@menu.test\" Test from Item1: \"@menu.item1.caption\"").c_str());
	//puts (testRes.parseResource("test @menu.test teest").c_str());

	//uiManager->icons.tex = std::make_unique<Texture> ("res/ui/icons.png");

	/*
	std::unique_ptr<UIPanel> pmenu = std::make_unique<UIPanel> (uiManager.get());
	pmenu->height = 25;
	pmenu->background = UIPanel::Background::Dark;
	pmenu->align = Align::Top;

	uiManager->addElement (std::move (pmenu));

	std::unique_ptr<UIToolbar> toolbar = std::make_unique<UIToolbar> (uiManager.get());
	auto _toolbar = toolbar.get();
	uiManager->addElement (std::move(toolbar));

	std::unique_ptr<UIToolbarTab> tooltab = std::make_unique<UIToolbarTab> (uiManager.get());
	tooltab->width = 150;
	tooltab->iconOffset[0] = 3;
	tooltab->iconOffset[1] = 4;
	tooltab->showIcon = true;
	tooltab->caption = L"Tab1";

	std::unique_ptr<UIToolbarTab> tooltab2 = std::make_unique<UIToolbarTab> (uiManager.get());
	tooltab2->width = 150;
	tooltab2->iconOffset[0] = 6;
	tooltab2->iconOffset[1] = 4;
	tooltab2->showIcon = true;
	tooltab2->caption = L"Tab2";

	std::unique_ptr<UIToolbarTab> tooltab3 = std::make_unique<UIToolbarTab> (uiManager.get());
	tooltab3->width = 150;
	tooltab3->iconOffset[0] = 10;
	tooltab3->iconOffset[1] = 6;
	tooltab3->showIcon = true;
	tooltab3->caption = L"Tab3";

	auto _tooltab  = tooltab .get();
	auto _tooltab2 = tooltab2.get();
	auto _tooltab3 = tooltab3.get();

	_toolbar->addElement (std::move (tooltab ));
	_toolbar->addElement (std::move (tooltab2));
	_toolbar->addElement (std::move (tooltab3));

	_toolbar->checkElement (_tooltab2);

	std::unique_ptr<UIToolbarItem> toolitem = std::make_unique<UIToolbarItem> (uiManager.get(), _toolbar);
	toolitem->iconOffset[0] = 1;
	toolitem->iconOffset[1] = 1;
	toolitem->caption = L"My Tool";

	std::unique_ptr<UIToolbarItem> toolitem2 = std::make_unique<UIToolbarItem> (uiManager.get(), _toolbar);
	toolitem2->iconOffset[0] = 2;
	toolitem2->iconOffset[1] = 1;
	toolitem2->caption = L"My Tool";

	std::unique_ptr<UIToolbarItem> toolitem3 = std::make_unique<UIToolbarItem> (uiManager.get(), _toolbar);
	toolitem3->iconOffset[0] = 1;
	toolitem3->iconOffset[1] = 3;
	toolitem3->caption = L"My Tool";

	std::unique_ptr<UIToolbarItem> toolitem4 = std::make_unique<UIToolbarItem> (uiManager.get(), _toolbar);
	toolitem4->iconOffset[0] = 2;
	toolitem4->iconOffset[1] = 2;
	toolitem4->caption = L"My Tool";

	std::unique_ptr<UIToolbarSplit> toolsplit = std::make_unique<UIToolbarSplit> (uiManager.get());

	_tooltab->addElement (std::move (toolitem));
	_tooltab->addElement (std::move (toolitem2));
	_tooltab->addElement (std::move (toolitem3));
	_tooltab->addElement (std::move (toolsplit));
	_tooltab->addElement (std::move (toolitem4));

	//uiManager->addElement (std::move (tooltab));

	std::unique_ptr<UIButton> b1 = std::make_unique<UIButton> (uiManager.get(), false);
	std::unique_ptr<UIButton> b2 = std::make_unique<UIButton> (uiManager.get(), true);
	std::unique_ptr<UIButton> b3 = std::make_unique<UIButton> (uiManager.get(), true);

	std::unique_ptr<UIGrouped> ge1 = std::make_unique<UIGrouped> (uiManager.get());

	std::unique_ptr<UITallMenuItem> tmi1 = std::make_unique<UITallMenuItem> (uiManager.get());

	tmi1->width = 200;
	tmi1->top = 100;
	tmi1->left = 30;

	tmi1->caption = L"Test";
	tmi1->desc    = L"./xx.jpg";

	//tmi1->loadImage ("../res/p2.jpg");

	UIGrouped * _ge1 = ge1.get();
	ge1->width = 400;

	ge1->top = 5;
	ge1->left = 30;

	b1->caption = L"Test";
	b1->left  = 30;
	b1->top   = 60;
	//b1->drawAlign = Align::Left;
	b1->width = 200;
	b1->showIcon = true;
	b1->showIcon2 = true;
	b1->iconOffset[0] = 1;
	b1->iconOffset[1] = 1;
	b1->icon2Offset[0] = 3;
	b1->icon2Offset[1] = 2;
	//b1->autoSize = true;
	//uiManager->addElement (std::move(b1));

	b2->caption = L"Test";
	b2->left  = 300;
	b2->top   = 60;
	b2->width = 200;
	b2->showIcon = true;
	b2->showIcon2 = true;
	b2->iconOffset[0] = 2;
	b2->iconOffset[1] = 1;
	b2->icon2Offset[0] = 3;
	b2->icon2Offset[1] = 3;
	b2->autoSize = true;
	//uiManager->addElement (std::move(b2));

	b3->caption = L"Test";
	b3->left = 700;
	b3->top  = 60;
	b3->width = 200;
	b3->autoSize = true;
	b3->showIcon = true;
	b3->showIcon2 = true;
	b3->iconOffset[0] = 3;
	b3->iconOffset[1] = 5;

	b3->icon2Offset[0] = 3;
	b3->icon2Offset[1] = 4;

	//uiManager->addElement (std::move(b3));

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
	p1->top    = 100;
	p1->width  = 500;
	p1->height = 400;
	p1->background = UIPanel::Background::Dark;
	//p1->showScrollX = false;

	//p1->scrollToElement(e1.get());
	//p1->test = e1.get();
	p1->align = Align::Right;
	p1->allowResize = true;
	p1->blackSplit = true;

	auto _p1 = p1.get();

	uiManager->addElement (std::move(p1));

	//_p1->addElement (std::move(b1));
	//_p1->addElement (std::move(b2));
	//_p1->addElement (std::move(e1));
	//_p1->addElement (std::move(b3));

	std::unique_ptr<UIPanel> p2 = std::make_unique<UIPanel> (uiManager.get());

	auto _p2 = p2.get();

	p2->left   = 50;
	p2->top    = 50;
	p2->width  = 50;
	p2->height = 250;
	//p2->background = UIPanel::Background::Dark;
	p2->align = Align::Top;
	p2->allowResize = true;
	p2->allowHide = true;

	std::unique_ptr<UIPanel> p4 = std::make_unique<UIPanel> (uiManager.get());

	auto _p4 = p4.get();

	p4->height = 27;
	//p2->background = UIPanel::Background::Dark;
	p4->align = Align::Top;
	p4->background = UIPanel::Background::Dark;
	p4->allowResize = false;
	p4->allowHide = false;

	_p1->addElement (std::move(p2));
	_p1->addElement (std::move(p4));
	//uiManager->addElement (std::move(p2));
	//_p2->addElement (std::move(b1));
	_p2->addElement (std::move(ge1));
	//_p2->addElement (std::move(tmi1));
	//_p2->addElement (std::move(e1));

	auto _b1 = b1.get();
	auto _b3 = b3.get();

	//_ge1->addElement (std::move (b1));
	_ge1->addElement (std::move (b2));
	_ge1->addElement (std::move (b3));

	_ge1->checkElement (_b3);
	//_ge1->checkElement (_b1);

	std::unique_ptr<UIPanel> p3 = std::make_unique<UIPanel> (uiManager.get());

	auto _p3 = p3.get();

	p3->left   = 50;
	p3->top    = 50;
	p3->width  = 50;
	p3->height = 250;
	//p2->background = UIPanel::Background::Dark;
	p3->align = Align::Top;
	p3->allowResize = true;
	p3->allowHide = true;
	p3->caption = L"Drag panel";

	_p1->addElement (std::move(p3));
	_p3->addElement (std::move(b1));

	std::unique_ptr<UICheckbox> cb1 = std::make_unique<UICheckbox> (uiManager.get());
	cb1->top = 100;
	cb1->left = 50;

	_p3->addElement (std::move(cb1));

	std::unique_ptr<UIListMenu> menu1 = std::make_unique<UIListMenu> (uiManager.get());
	menu1->width = 200;
	menu1->height = 50;
	menu1->top = 150;
	menu1->left = 50;
	auto _menu1 = menu1.get();

	_p3->addElement (std::move(menu1));

	std::unique_ptr<UIMenuItem> mi1 = std::make_unique<UIMenuItem> (uiManager.get());
	std::unique_ptr<UIMenuItem> mi2 = std::make_unique<UIMenuItem> (uiManager.get());
	std::unique_ptr<UIMenuItem> mi3 = std::make_unique<UIMenuItem> (uiManager.get());
	std::unique_ptr<UIMenuHr>   hr1 = std::make_unique<UIMenuHr>   (uiManager.get());

	mi1->caption = L"Menu Item 1";
	mi2->caption = L"Menu Item 2";
	mi3->caption = L"Menu Item 3";

	mi2->shortKey = L"Ctrl C";

	mi1->iconOffset[0] = 3;
	mi1->iconOffset[1] = 5;

	auto _mi1 = mi1.get();

	_menu1->addElement (std::move(mi1));
	_menu1->addElement (std::move(mi2));
	_menu1->addElement (std::move(hr1));
	_menu1->addElement (std::move(mi3));

	std::unique_ptr<UIListMenu> menu2 = std::make_unique<UIListMenu> (uiManager.get());
	menu2->width = 200;
	menu2->height = 50;
	menu2->top = 150;
	menu2->left = 50;
	auto _menu2 = menu2.get();

	//_p3->addElement (std::move(menu2));
	_mi1->setMenu (std::move(menu2));

	std::unique_ptr<UIMenuItem> mi4 = std::make_unique<UIMenuItem> (uiManager.get());
	std::unique_ptr<UIMenuItem> mi5 = std::make_unique<UIMenuItem> (uiManager.get());
	std::unique_ptr<UIMenuItem> mi6 = std::make_unique<UIMenuItem> (uiManager.get());

	mi4->caption = L"Menu Item 4";
	mi5->caption = L"Menu Item 5";
	mi6->caption = L"Menu Item 6";

	mi5->shortKey = L"Ctrl V";
	auto _mi5 = mi5.get();

	_menu2->addElement (std::move(mi4));
	_menu2->addElement (std::move(mi5));
	_menu2->addElement (std::move(mi6));

	std::unique_ptr<UIListMenu> menu3 = std::make_unique<UIListMenu> (uiManager.get());
	menu3->width = 200;
	menu3->height = 50;
	menu3->top = 150;
	menu3->left = 50;
	auto _menu3 = menu3.get();

	//_p3->addElement (std::move(menu2));
	//_mi5->setMenu (std::move(menu3));

	std::unique_ptr<UIDropMenu> dm1 = std::make_unique<UIDropMenu> (uiManager.get());

	dm1->left = 250;
	dm1->top  = 50;
	dm1->width = 180;

	auto _dm1 = dm1.get();
	_p3->addElement (std::move(dm1));
	_dm1->setMenu (std::move(menu3));

	std::unique_ptr<UIMenuItem> mi7 = std::make_unique<UIMenuItem> (uiManager.get());
	std::unique_ptr<UIMenuItem> mi8 = std::make_unique<UIMenuItem> (uiManager.get());
	std::unique_ptr<UIMenuItem> mi9 = std::make_unique<UIMenuItem> (uiManager.get());

	mi7->caption = L"Menu Item 7";
	mi8->caption = L"Menu Item 8";
	mi9->caption = L"Menu Item 9";

	mi9->shortKey = L"Ctrl V";

	_menu3->addElement (std::move(mi7));
	_menu3->addElement (std::move(mi8));
	_menu3->addElement (std::move(mi9));

	//- Tree List Demo -//

	std::unique_ptr<UITreeList> treeList = std::make_unique<UITreeList> (uiManager.get());

	//treeList->drawLines = false;
	treeList->width = 200;
	treeList->left = 40;
	treeList->top  = 85;

	auto _treeList = treeList.get();
	_p2->addElement (std::move (treeList));

	//- Tree List Nodes -//

	std::unique_ptr<UITreeListNode> tlNode1 = std::make_unique<UITreeListNode> (uiManager.get(), _treeList);
	auto _tlNode1 = tlNode1.get();

	tlNode1->caption = L"Root Node 1"; tlNode1->showIcon = true;
	tlNode1->iconOffset[0] = 4;        tlNode1->iconOffset[1] = 1;
	tlNode1->allowHide = false;

	//

	std::unique_ptr<UITreeListNode> tlNode2 = std::make_unique<UITreeListNode> (uiManager.get(), _treeList);
	auto _tlNode2 = tlNode2.get();

	tlNode2->caption = L"Child Node 2"; tlNode2->showIcon = true;
	tlNode2->iconOffset[0] = 4;       tlNode2->iconOffset[1] = 2;

	//

	std::unique_ptr<UITreeListNode> tlNode3 = std::make_unique<UITreeListNode> (uiManager.get(), _treeList);
	auto _tlNode3 = tlNode3.get();

	tlNode3->caption = L"Child Node 3"; tlNode3->showIcon = true;
	tlNode3->iconOffset[0] = 4;       tlNode3->iconOffset[1] = 3;

	//

	std::unique_ptr<UITreeListNode> tlNode4 = std::make_unique<UITreeListNode> (uiManager.get(), _treeList);
	auto _tlNode4 = tlNode4.get();

	tlNode4->caption = L"Child Node 4"; tlNode4->showIcon = true;
	tlNode4->iconOffset[0] = 4;       tlNode4->iconOffset[1] = 4;

	//

	std::unique_ptr<UITreeListNode> tlNode5 = std::make_unique<UITreeListNode> (uiManager.get(), _treeList);
	auto _tlNode5 = tlNode5.get();

	tlNode5->caption = L"Child Node 5"; tlNode5->showIcon = true;
	tlNode5->iconOffset[0] = 4;       tlNode5->iconOffset[1] = 5;

	//

	std::unique_ptr<UITreeListNode> tlNode6 = std::make_unique<UITreeListNode> (uiManager.get(), _treeList);
	auto _tlNode6 = tlNode6.get();

	tlNode6->caption = L"Child Node 6"; tlNode6->showIcon = true;
	tlNode6->iconOffset[0] = 4;       tlNode6->iconOffset[1] = 6;

	//

	_treeList->addElement (std::move (tlNode1));
	_tlNode1 ->addElement (std::move (tlNode2));
	_tlNode1 ->addElement (std::move (tlNode3));
	_tlNode1 ->addElement (std::move (tlNode4));
	_tlNode3 ->addElement (std::move (tlNode5));
	_tlNode5 ->addElement (std::move (tlNode6));
	_tlNode2->open = false;

	//

	std::unique_ptr<UITab> tab = std::make_unique<UITab>(uiManager.get());
	tab->caption = L"Tab 1";
	tab->width = 80;

	std::unique_ptr<UITab> tab3 = std::make_unique<UITab>(uiManager.get());
	tab3->caption = L"Tab 3";
	tab3->width = 80;

	std::unique_ptr<UITab> tab2 = std::make_unique<UITab>(uiManager.get());
	tab2->caption = L"Tab 2";
	tab2->width = 80;

	std::unique_ptr<UIGrouped> tabs = std::make_unique<UIGrouped> (uiManager.get(), false, 2);
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

	/*std::unique_ptr<UIImage> image1 = std::make_unique<UIImage> (uiManager.get());
	image1->left = 250;
	image1->top  = 150;
	image1->loadImage ("../set.png");
	image1->cropImage (16, 290, 350, 258);
	_p3->addElement (std::move(image1));*/

	/*std::unique_ptr<UILabel> label1 = std::make_unique<UILabel> (uiManager.get());
	label1->left = 10;
	label1->top  = 40;
	label1->caption = L"Hello World!";
	_p3->addElement (std::move(label1));

	std::unique_ptr<UIDialog> dialog = std::make_unique<UIDialog> (uiManager.get());

	dialog->left   = 100;
	dialog->top    = 100;
	dialog->width  = 300;
	dialog->height = 200;

	uiManager->addElement (std::move (dialog));*/

}

Core::~Core() {
}

/* */

void Core::render() {

	app->screenCamera->update();
	uiManager->render();
	app->screenCamera->update();

}

void Core::step() {

}
