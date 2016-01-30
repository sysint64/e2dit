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
 * Author: Kabylin Andrey <andrey@kabylin.ru
 */

#include "ui/loader.h"
#include "ui/all.h"

/**
 */

void UILoader::placeElements (DataMap::DataNode *rootNode, UIElement *uiParent) {

	for (const auto &it : rootNode->childs) {

		auto el = createElement (it.get());

		if (el == nullptr)
			continue;

		auto elPtr = el.get();

		uiParent->addElement (std::move(el));
		placeElements (it.get(), elPtr);

	}

}

/**
 */

std::unique_ptr<UIElement> UILoader::createElement (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = nullptr;

	if (elementNode->type == "panel" ) element = createPanel  (elementNode); else
	if (elementNode->type == "button") element = createButton (elementNode);

	if (element == nullptr)
		return nullptr;

	// Processe generic parameters

	auto size     = elementNode->params.find ("size");
	auto location = elementNode->params.find ("location");
	auto align    = elementNode->params.find ("align");
	auto end      = elementNode->params.end();

	if (size != end) {
		element->width  = floor(size->second[0].num);
		element->height = floor(size->second[1].num);
	}

	if (location != end) {
		element->left = floor(location->second[0].num);
		element->top  = floor(location->second[1].num);
	}

	if (align != end) {

		if (align->second[0].str == "none"  ) element->align = Align::None;
		if (align->second[0].str == "left"  ) element->align = Align::Left;
		if (align->second[0].str == "center") element->align = Align::Center;
		if (align->second[0].str == "right" ) element->align = Align::Right;
		if (align->second[0].str == "client") element->align = Align::Client;
		if (align->second[0].str == "bottom") element->align = Align::Bottom;
		if (align->second[0].str == "all"   ) element->align = Align::All;
		if (align->second[0].str == "top"   ) element->align = Align::Top;

	}

	return element;

}

/**
 */

std::unique_ptr<UIElement> UILoader::createPanel (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UIPanel> (manager);
	auto panel = dynamic_cast<UIPanel*>(element.get());

	auto allowResize = elementNode->params.find ("allowresize");
	auto allowHide   = elementNode->params.find ("allowhide");
	auto allowDrag   = elementNode->params.find ("allowdrag");
	auto blackSplit  = elementNode->params.find ("blacksplit");
	auto showSplit   = elementNode->params.find ("showsplit");
	auto caption     = elementNode->params.find ("caption");
	auto background  = elementNode->params.find ("background");
	auto end         = elementNode->params.end();

	if (allowResize != end) panel->allowResize = allowResize->second[0].boolean;
	if (allowHide   != end) panel->allowHide   = allowHide  ->second[0].boolean;
	if (allowDrag   != end) panel->allowDrag   = allowDrag  ->second[0].boolean;
	if (blackSplit  != end) panel->blackSplit  = blackSplit ->second[0].boolean;
	if (showSplit   != end) panel->showSplit   = showSplit  ->second[0].boolean;
	if (caption     != end) panel->caption     = caption    ->second[0].wstr;

	if (background != end) {

		if (background->second[0].str == "light")
			panel->background = UIPanel::Background::Light;

		if (background->second[0].str == "dark")
			panel->background = UIPanel::Background::Dark;

		if (background->second[0].str == "action")
			panel->background = UIPanel::Background::Action;

	}

	return element;

}

/**
 */

std::unique_ptr<UIElement> UILoader::createButton (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UIButton> (manager, false);
	auto button = dynamic_cast<UIButton*>(element.get());

	auto caption = elementNode->params.find ("caption");
	auto end     = elementNode->params.end();

	if (caption != end) button->caption = caption->second[0].wstr;

	return element;

}
