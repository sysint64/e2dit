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
#include "resources.h"

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

std::array<int, 4> UILoader::readRect (DataMap::DataNode *elementNode, const std::string &paramName, const bool autoFill) {

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

			if (autoFill) return {v[0], v[0], v[0], v[0]};
			else          return {v[0], v[1], v[2], v[3]};

		} else if (size == 2) {

			v[0] = param->second[0].intval;
			v[1] = param->second[1].intval;

			if (autoFill) return {v[0], v[1], v[0], v[1]};
			else          return {v[0], v[1], v[2], v[3]};

		} else if (size == 3) {

			v[0] = param->second[0].intval;
			v[1] = param->second[1].intval;
			v[2] = param->second[2].intval;

			if (autoFill) return {v[0], v[1], v[2], v[1]};
			else          return {v[0], v[1], v[2], v[3]};

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

	auto end   = elementNode->params.end();
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

std::wstring UILoader::readCaption (DataMap::DataNode *elementNode, const std::string &paramName) {

	auto caption = elementNode->params.find (paramName);
	auto end     = elementNode->params.end();

	if (caption != end)
		return stringsRes->parseResource (caption->second[0].wstr);

	return L"";

}

/**
 */

void UILoader::readIcon (UIButton *element, DataMap::DataNode *elementNode) {

	auto showIcon    = elementNode->params.find ("showicon");
	auto showIcon2   = elementNode->params.find ("showicon2");
	auto iconOffset  = elementNode->params.find ("iconoffset");
	auto icon2Offset = elementNode->params.find ("icon2offset");
	auto end         = elementNode->params.end();

	if (showIcon  != end) element->showIcon  = showIcon ->second[0].boolean;
	if (showIcon2 != end) element->showIcon2 = showIcon2->second[0].boolean;

	if (iconOffset != end) {
		element->iconOffset[0] = iconOffset->second[0].intval;
		element->iconOffset[1] = iconOffset->second[1].intval;
	}

	if (icon2Offset != end) {
		element->icon2Offset[0] = icon2Offset->second[0].intval;
		element->icon2Offset[1] = icon2Offset->second[1].intval;
	}

}

/**
 */

std::array<float, 4> UILoader::readColor (DataMap::DataNode *elementNode, const std::string &paramName) {

	float c[4] = {0.f, 0.f, 0.f, 1.f};

	auto color = elementNode->params.find (paramName);
	auto end   = elementNode->params.end();

	if (color != end && color->second.size() >= 3) {

		c[0] = color->second[0].num / 255.f;
		c[1] = color->second[1].num / 255.f;
		c[2] = color->second[2].num / 255.f;

		if (color->second.size() >= 4)
			c[3] = color->second[3].num;

	}

	return { c[0], c[1], c[2], c[3] };

}

/**
 */

std::unique_ptr<UIElement> UILoader::createElement (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = nullptr;

	if (elementNode->name == "panel" )      element = createPanel       (elementNode); else
	if (elementNode->name == "stacklayout") element = createStackLayout (elementNode); else
	if (elementNode->name == "button")      element = createButton      (elementNode); else
	if (elementNode->name == "toolbar")     element = createToolbar     (elementNode); else
	if (elementNode->name == "toolbartab")  element = createToolbarTab  (elementNode); else
	if (elementNode->name == "toolbaritem") element = createToolbarItem (elementNode); else
	if (elementNode->name == "colorpanel")  element = createColorPanel  (elementNode); else
	if (elementNode->name == "image")       element = createImage       (elementNode); else
	if (elementNode->name == "label")       element = createLabel       (elementNode); else
	if (elementNode->name == "grouped")     element = createGrouped     (elementNode); else
	if (elementNode->name == "dialog")      element = createDialog      (elementNode);

	if (element == nullptr)
		return nullptr;

	// Processe generic parameters

	auto size     = elementNode->params.find ("size");
	auto location = elementNode->params.find ("location");
	auto name     = elementNode->params.find ("name");
	auto left     = elementNode->params.find ("left");
	auto top      = elementNode->params.find ("top");
	auto width    = elementNode->params.find ("width");
	auto height   = elementNode->params.find ("height");
	auto end      = elementNode->params.end();

	if (size != end) {
		element->width  = size->second[0].intval;
		element->height = size->second[1].intval;
	}

	if (location != end) {
		element->left = location->second[0].intval;
		element->top  = location->second[1].intval;
	}

	if (name   != end) element->name   = name  ->second[0].str;
	if (left   != end) element->left   = left  ->second[0].intval;
	if (top    != end) element->top    = top   ->second[0].intval;
	if (width  != end) element->width  = width ->second[0].intval;
	if (height != end) element->height = height->second[0].intval;

	//

	element->align         = readAlign (elementNode, "align");
	element->verticalAlign = readAlign (elementNode, "verticalalign");

	std::array<int, 4> marginRect  = readRect (elementNode, "margin");
	std::array<int, 4> paddingRect = readRect (elementNode, "padding");

	std::cout << marginRect[0] << std::endl;

	element->marginTop     = marginRect [0]; element->marginRight  = marginRect [1];
	element->marginBottom  = marginRect [2]; element->marginLeft   = marginRect [3];

	element->paddingTop    = paddingRect[0]; element->paddingRight = paddingRect[1];
	element->paddingBottom = paddingRect[2]; element->paddingLeft  = paddingRect[3];

	if (elementNode->name == "colorpanel")
		dynamic_cast<UIColorPanel*>(element.get())->update();

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

std::unique_ptr<UIElement> UILoader::createImage (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UIImage> (manager);
	auto image = dynamic_cast<UIImage*>(element.get());

	auto src = wstr2str(readCaption(elementNode, "src"));
	std::cout << "Element src: " << src << std::endl;

	if (src == "")
		return element;

	std::array<int, 4> cropRect = readRect (elementNode, "crop", false);

	image->loadImage ("../res/"+src);
	image->cropImage (cropRect);

	return element;

}

/**
 */

std::unique_ptr<UIElement> UILoader::createStackLayout (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UIStackLayout> (manager);
	auto layout = dynamic_cast<UIStackLayout*>(element.get());

	auto orient = elementNode->params.find ("orientation");
	auto end    = elementNode->params.end();

	if (orient != end) {
		if (orient->second[0].str == "horizontal") layout->orientation = Orientation::Horizontal; else
		if (orient->second[0].str == "vertical"  ) layout->orientation = Orientation::Vertical;
	}

	return element;

}

/**
 */

std::unique_ptr<UIElement> UILoader::createButton (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UIButton> (manager, false);
	auto button = dynamic_cast<UIButton*>(element.get());

	button->caption   = readCaption (elementNode, "caption");
	button->textAlign = readAlign   (elementNode, "textalign");
	readIcon (button, elementNode);

	return element;

}

/**
 */

std::unique_ptr<UIElement> UILoader::createLabel (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UILabel> (manager);
	auto label = dynamic_cast<UILabel*>(element.get());

	label->caption   = readCaption (elementNode, "caption");
	label->textAlign = readAlign   (elementNode, "textalign");

	return element;

}

/**
 */

std::unique_ptr<UIElement> UILoader::createGrouped (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UIGrouped> (manager);
	return element;

}

/**
 */

std::unique_ptr<UIElement> UILoader::createToolbar (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UIToolbar> (manager);
	return element;

}

/**
 */

std::unique_ptr<UIElement> UILoader::createToolbarTab (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UIToolbarTab> (manager);
	auto toolbarTab = dynamic_cast<UIToolbarTab*>(element.get());

	toolbarTab->caption = readCaption(elementNode, "caption");
	readIcon (toolbarTab, elementNode);

	return element;

}

/**
 */

std::unique_ptr<UIElement> UILoader::createToolbarItem (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UIToolbarItem> (manager);
	auto toolbarItem = dynamic_cast<UIToolbarItem*>(element.get());

	toolbarItem->caption = readCaption(elementNode, "caption");

	auto iconOffset  = elementNode->params.find ("iconoffset");
	auto end         = elementNode->params.end();

	if (iconOffset != end) {
		toolbarItem->iconOffset[0] = iconOffset->second[0].intval;
		toolbarItem->iconOffset[1] = iconOffset->second[1].intval;
	}

	return element;

}

/**
 */

std::unique_ptr<UIElement> UILoader::createColorPanel (DataMap::DataNode *elementNode) {

	Texture *gridTexture = R::Textures::gridLight.get();

	auto grid = elementNode->params.find ("grid");
	auto end  = elementNode->params.end();

	if (grid != end) {
		if (grid->second[0].str == "light") gridTexture = R::Textures::gridLight.get(); else
		if (grid->second[0].str == "dark" ) gridTexture = R::Textures::gridDark .get(); else
		if (grid->second[0].str == "none" ) gridTexture = nullptr;
	}

	std::unique_ptr<UIElement> element = std::make_unique<UIColorPanel> (manager, gridTexture);
	auto colorPanel = dynamic_cast<UIColorPanel*>(element.get());

	colorPanel->color = readColor (elementNode, "color");
	return element;

}

/**
 */

std::unique_ptr<UIElement> UILoader::createDialog (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UIDialog> (manager);
	auto dialog = dynamic_cast<UIDialog*>(element.get());
	//dialog->caption = readCaption(elementNode, "caption");

	return element;

}
