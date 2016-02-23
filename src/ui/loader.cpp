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

std::array<int, 4> UILoader::readRect (DataMap::DataNode *elementNode, const std::string &paramName) {

	auto end = elementNode->params.end();

	auto param       = elementNode->params.find (paramName);
	auto paramLeft   = elementNode->params.find (paramName+"left");
	auto paramRight  = elementNode->params.find (paramName+"right");
	auto paramTop    = elementNode->params.find (paramName+"top");
	auto paramBottom = elementNode->params.find (paramName+"bottom");

	int v[4] = {0, 0, 0, 0};

	if (param != end) {

		int size = param->second.size();

		if (size == 1) {

			v[0] = param->second[0].intval;
			return {v[0], v[0], v[0], v[0]};

		} else if (size == 2) {

			v[0] = param->second[0].intval;
			v[1] = param->second[1].intval;

			return {v[0], v[1], v[0], v[1]};

		} else if (size == 3) {

			v[0] = param->second[0].intval;
			v[1] = param->second[1].intval;
			v[2] = param->second[2].intval;

			return {v[0], v[1], v[2], v[1]};

		} else if (size == 4) {

			v[0] = param->second[0].intval;
			v[1] = param->second[1].intval;
			v[2] = param->second[2].intval;
			v[3] = param->second[3].intval;

			return {v[0], v[1], v[2], v[3]};

		}

	}

	if (paramTop    != end) v[0] = paramTop   ->second[0].intval;
	if (paramRight  != end) v[1] = paramRight ->second[0].intval;
	if (paramBottom != end) v[2] = paramBottom->second[0].intval;
	if (paramLeft   != end) v[3] = paramLeft  ->second[0].intval;

	return {v[0], v[1], v[2], v[3]};

}

/**
 */

Align UILoader::readAlign (DataMap::DataNode *elementNode, const std::string &paramName) {

	auto end = elementNode->params.end();
	auto align = elementNode->params.find (paramName);

	if (align != end) {

		if (align->second[0].str == "none"  ) return Align::None;   else
		if (align->second[0].str == "left"  ) return Align::Left;   else
		if (align->second[0].str == "center") return Align::Center; else
		if (align->second[0].str == "right" ) return Align::Right;  else
		if (align->second[0].str == "client") return Align::Client; else
		if (align->second[0].str == "bottom") return Align::Bottom; else
		if (align->second[0].str == "all"   ) return Align::All;    else
		if (align->second[0].str == "top"   ) return Align::Top;    else
		if (align->second[0].str == "middle") return Align::Middle;

	}

	return Align::None;

}


/**
 */

std::unique_ptr<UIElement> UILoader::createElement (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = nullptr;

	if (elementNode->type == "panel" )      element = createPanel       (elementNode); else
	if (elementNode->type == "stacklayout") element = createStackLayout (elementNode); else
	if (elementNode->type == "button")      element = createButton      (elementNode);

	if (element == nullptr)
		return nullptr;

	// Processe generic parameters

	auto size          = elementNode->params.find ("size");
	auto location      = elementNode->params.find ("location");

	auto end           = elementNode->params.end();

	if (size != end) {
		element->width  = floor(size->second[0].num);
		element->height = floor(size->second[1].num);
	}

	if (location != end) {
		element->left = floor(location->second[0].num);
		element->top  = floor(location->second[1].num);
	}

	element->align         = readAlign (elementNode, "align");
	element->verticalAlign = readAlign (elementNode, "verticalalign");

	std::array<int, 4> marginRect  = readRect (elementNode, "margin");
	std::array<int, 4> paddingRect = readRect (elementNode, "padding");

	std::cout << marginRect[0] << std::endl;

	element->marginTop     = marginRect [0]; element->marginRight  = marginRect [1];
	element->marginBottom  = marginRect [2]; element->marginLeft   = marginRect [3];

	element->paddingTop    = paddingRect[0]; element->paddingRight = paddingRect[1];
	element->paddingBottom = paddingRect[2]; element->paddingLeft  = paddingRect[3];

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

	if (caption     != end)
		panel->caption = stringsRes->parseResource(caption->second[0].wstr);

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

std::unique_ptr<UIElement> UILoader::createStackLayout (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UIStackLayout> (manager);
	auto button = dynamic_cast<UIButton*>(element.get());

	return element;

}

/**
 */

std::unique_ptr<UIElement> UILoader::createButton (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UIButton> (manager, false);
	auto button = dynamic_cast<UIButton*>(element.get());

	auto caption = elementNode->params.find ("caption");
	auto end     = elementNode->params.end();

	if (caption != end)
		button->caption = stringsRes->parseResource (caption->second[0].wstr);

	return element;

}
