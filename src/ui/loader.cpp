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


void UILoader::placeElements (DataMap::DataNode *rootNode, UIElement *uiParent) {

	for (const auto &it : rootNode->childs) {

		auto el = createElement (it.get());

		if (el == nullptr)
			continue;

		auto elPtr = el.get();

		auto grouped = dynamic_cast<UIGrouped*> (uiParent);

		if (grouped)
			grouped->autoCheck();

		uiParent->addElement (std::move(el));
		placeElements (it.get(), elPtr);

	}

}


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


Align UILoader::readAlign (DataMap::DataNode *elementNode, const std::string &paramName) {

	auto end   = elementNode->params.end();
	auto align = elementNode->params.find (paramName);

	if (align != end) {

		const auto param = align->second[0].str;

		if (param == "none"        ) return Align::None;         else
		if (param == "left"        ) return Align::Left;         else
		if (param == "center"      ) return Align::Center;       else
		if (param == "right"       ) return Align::Right;        else
		if (param == "client"      ) return Align::Client;       else
		if (param == "bottom"      ) return Align::Bottom;       else
		if (param == "all"         ) return Align::All;          else
		if (param == "top"         ) return Align::Top;          else
		if (param == "middle"      ) return Align::Middle;       else
		if (param == "parentwidth" ) return Align::ParentWidth;  else
		if (param == "parentheight") return Align::ParentHeight;

	}

	return Align::None;

}


std::wstring UILoader::readUString (DataMap::DataNode *elementNode, const std::string &paramName, const std::wstring &defaultVal) {
	auto ustring = elementNode->params.find (paramName);
	auto end     = elementNode->params.end();

	if (ustring != end)
		return stringsRes->parseResource (ustring->second[0].wstr);

	return defaultVal;
}


std::string UILoader::readString (DataMap::DataNode *elementNode, const std::string &paramName, const std::string &defaultVal) {
	auto val = elementNode->params.find (paramName);
	auto end = elementNode->params.end();

	if (val != end)
		return wstr2str(stringsRes->parseResource (val->second[0].wstr));

	return defaultVal;
}


int UILoader::readInt (DataMap::DataNode *elementNode, const std::string &paramName, const int defaultVal) {
	auto val = elementNode->params.find (paramName);
	auto end = elementNode->params.end();

	if (val != end)
		return val->second[0].intval;

	return defaultVal;
}


float UILoader::readFloat (DataMap::DataNode *elementNode, const std::string &paramName, const float defaultVal) {
	auto val = elementNode->params.find (paramName);
	auto end = elementNode->params.end();

	if (val != end)
		return val->second[0].num;

	return defaultVal;
}


bool UILoader::readBool (DataMap::DataNode *elementNode, const std::string &paramName, const bool defaultVal) {
	auto val = elementNode->params.find (paramName);
	auto end = elementNode->params.end();

	if (val != end)
		return val->second[0].boolean;

	return defaultVal;
}


void UILoader::readIcon (UIButton *element, DataMap::DataNode *elementNode) {

	auto showIcon    = elementNode->params.find ("showicon");
	auto showIcon2   = elementNode->params.find ("showicon2");
	auto iconOffset  = elementNode->params.find ("icon");
	auto icon2Offset = elementNode->params.find ("icon2");
	auto end         = elementNode->params.end();

	if (iconOffset != end) {
		element->showIcon      = true;
		element->iconOffset[0] = iconOffset->second[0].intval-1;
		element->iconOffset[1] = iconOffset->second[1].intval-1;
	}

	if (icon2Offset != end) {
		element->showIcon2      = true;
		element->icon2Offset[0] = icon2Offset->second[0].intval-1;
		element->icon2Offset[1] = icon2Offset->second[1].intval-1;
	}

	if (showIcon  != end) element->showIcon  = showIcon ->second[0].boolean;
	if (showIcon2 != end) element->showIcon2 = showIcon2->second[0].boolean;

}


gapi::Color UILoader::readColor (DataMap::DataNode *elementNode, const std::string &paramName) {

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

	return gapi::Color::createFromRGBA (c[0], c[1], c[2], c[3]);

}


std::unique_ptr<UIElement> UILoader::createElement (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = nullptr;

	if (elementNode->name == "panel" )       element = createPanel        (elementNode); else
	if (elementNode->name == "stacklayout")  element = createStackLayout  (elementNode); else
	if (elementNode->name == "button")       element = createButton       (elementNode); else
	if (elementNode->name == "edit")         element = createEdit         (elementNode); else
	if (elementNode->name == "toolbar")      element = createToolbar      (elementNode); else
	if (elementNode->name == "toolbartab")   element = createToolbarTab   (elementNode); else
	if (elementNode->name == "toolbaritem")  element = createToolbarItem  (elementNode); else
	if (elementNode->name == "colorpanel")   element = createColorPanel   (elementNode); else
	if (elementNode->name == "image")        element = createImage        (elementNode); else
	if (elementNode->name == "label")        element = createLabel        (elementNode); else
	if (elementNode->name == "grouped")      element = createGrouped      (elementNode); else
	if (elementNode->name == "dialog")       element = createDialog       (elementNode); else
	if (elementNode->name == "listmenu")     element = createListMenu     (elementNode); else
	if (elementNode->name == "dropdown")     element = createDropMenu     (elementNode); else
	if (elementNode->name == "menuitem")     element = createMenuItem     (elementNode); else
	if (elementNode->name == "menuhr")       element = createMenuHr       (elementNode); else
	if (elementNode->name == "tallmenuitem") element = createTallMenuItem (elementNode); else
	if (elementNode->name == "treelist")     element = createTreeList     (elementNode); else
	if (elementNode->name == "treelistnode") element = createTreeListNode (elementNode);

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
	auto visible  = elementNode->params.find ("visible");
	auto end      = elementNode->params.end();

	if (size != end) {
		element->width  = size->second[0].intval;
		element->height = size->second[1].intval;
	} else {
		element->autoSize = true;
	}

	if (location != end) {
		element->left = location->second[0].intval;
		element->top  = location->second[1].intval;
	}

	if (name    != end) element->name    = name   ->second[0].str;
	if (left    != end) element->left    = left   ->second[0].intval;
	if (top     != end) element->top     = top    ->second[0].intval;
	if (width   != end) element->width   = width  ->second[0].intval;
	if (height  != end) element->height  = height ->second[0].intval;
	if (visible != end) element->visible = visible->second[0].boolean;

	element->align         = readAlign (elementNode, "align");
	element->verticalAlign = readAlign (elementNode, "verticalalign");
	element->alignSize     = readBool  (elementNode, "alignsize");
	element->withoutSkin   = readBool  (elementNode, "withoutskin");
	element->autoSize      = readBool  (elementNode, "autosize");
	element->autoWidth     = readBool  (elementNode, "autowidth");
	element->autoHeight    = readBool  (elementNode, "autoheight");

	std::array<int, 4> marginRect  = readRect (elementNode, "margin");
	std::array<int, 4> paddingRect = readRect (elementNode, "padding");

	element->marginTop     = marginRect [0]; element->marginRight  = marginRect [1];
	element->marginBottom  = marginRect [2]; element->marginLeft   = marginRect [3];

	element->paddingTop    = paddingRect[0]; element->paddingRight = paddingRect[1];
	element->paddingBottom = paddingRect[2]; element->paddingLeft  = paddingRect[3];

	if (elementNode->name == "colorpanel")
		dynamic_cast<UIColorPanel*>(element.get())->update();

	return element;

}


std::unique_ptr<UIElement> UILoader::createPanel (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UIPanel> (manager);
	auto panel = dynamic_cast<UIPanel*>(element.get());

	auto background  = elementNode->params.find ("background");
	auto end         = elementNode->params.end();

	panel->allowResize = readBool    (elementNode, "allowresize");
	panel->allowHide   = readBool    (elementNode, "allowhide");
	panel->allowDrag   = readBool    (elementNode, "allowdrag");
	panel->blackSplit  = readBool    (elementNode, "blacksplit");
	panel->showSplit   = readBool    (elementNode, "showsplit", true);
	panel->allowScroll = readBool    (elementNode, "allowscroll", true);
	panel->caption     = readUString (elementNode, "caption");

	if (background != end) {

		if (background->second[0].str == "transparent")
			panel->background = UIPanel::Background::Transparent;

		if (background->second[0].str == "light")
			panel->background = UIPanel::Background::Light;

		if (background->second[0].str == "dark")
			panel->background = UIPanel::Background::Dark;

		if (background->second[0].str == "action")
			panel->background = UIPanel::Background::Action;

	}

	return element;

}


std::unique_ptr<UIElement> UILoader::createImage (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UIImage> (manager);
	auto image = dynamic_cast<UIImage*>(element.get());
	auto src = readString(elementNode, "src");

	if (src == "")
		return element;

	std::array<int, 4> cropRect = readRect (elementNode, "crop", false);

	puts((app->resPath+"/"+src).c_str());
	image->loadImage (app->resPath+"/"+src);
	image->cropImage (cropRect);

	return element;

}


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


std::unique_ptr<UIElement> UILoader::createButton (DataMap::DataNode *elementNode) {

	bool allowCheck = readBool (elementNode, "allowcheck");

	std::unique_ptr<UIElement> element = std::make_unique<UIButton> (manager, allowCheck);
	auto button = dynamic_cast<UIButton*>(element.get());

	button->caption   = readUString (elementNode, "caption");
	button->textAlign = readAlign   (elementNode, "textalign");
	button->checked   = readBool    (elementNode, "checked");

	if (button->textAlign == Align::None)
		button->textAlign = Align::Center;

	readIcon (button, elementNode);
	return element;

}


std::unique_ptr<UIElement> UILoader::createLabel (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UILabel> (manager);
	auto label = dynamic_cast<UILabel*>(element.get());

	label->caption   = readUString (elementNode, "caption");
	label->textAlign = readAlign   (elementNode, "textalign");

	return element;

}


std::unique_ptr<UIElement> UILoader::createEdit (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UIEdit> (manager);
	auto edit = dynamic_cast<UIEdit*>(element.get());

	edit->text   = readUString (elementNode, "text");
	edit->before = readUString (elementNode, "before");
	edit->after  = readUString (elementNode, "after");

	return element;

}


std::unique_ptr<UIElement> UILoader::createGrouped (DataMap::DataNode *elementNode) {

	bool multiSelect = readBool (elementNode, "multiselect");
	int  spacing     = readInt  (elementNode, "spacing");

	std::unique_ptr<UIElement> element = std::make_unique<UIGrouped> (manager, multiSelect, spacing);
	return element;

}


std::unique_ptr<UIElement> UILoader::createToolbar (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UIToolbar> (manager);
	UIToolbar *toolbar = dynamic_cast<UIToolbar*>(element.get());
	toolbar->tabsOffset = readInt(elementNode, "tabsoffset", 20);
	this->toolbar = toolbar;
	return element;

}


std::unique_ptr<UIElement> UILoader::createToolbarTab (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UIToolbarTab> (manager);
	auto toolbarTab = dynamic_cast<UIToolbarTab*>(element.get());

	toolbarTab->caption = readUString (elementNode, "caption");
	toolbarTab->parent = toolbar;
	readIcon (toolbarTab, elementNode);

	bool checked = readBool(elementNode, "checked");

	if (checked)
		toolbar->checkElement(toolbarTab);

	return element;

}


std::unique_ptr<UIElement> UILoader::createToolbarItem (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UIToolbarItem> (manager);
	auto toolbarItem = dynamic_cast<UIToolbarItem*>(element.get());

	toolbarItem->caption = readUString (elementNode, "caption");

	auto iconOffset  = elementNode->params.find ("iconoffset");
	auto end         = elementNode->params.end();

	if (iconOffset != end) {
		toolbarItem->iconOffset[0] = iconOffset->second[0].intval;
		toolbarItem->iconOffset[1] = iconOffset->second[1].intval;
	}

	return element;

}


std::unique_ptr<UIElement> UILoader::createColorPanel (DataMap::DataNode *elementNode) {

	gapi::Texture *gridTexture = R::Textures::gridLight.get();

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


std::unique_ptr<UIElement> UILoader::createDialog (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UIDialog> (manager);
	auto dialog = dynamic_cast<UIDialog*>(element.get());
	dialog->caption = readUString (elementNode, "caption");

	return element;

}


std::unique_ptr<UIElement> UILoader::createTreeList (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UITreeList> (manager);
	auto treeList = dynamic_cast<UITreeList*>(element.get());
	this->treeList = treeList;

	return element;

}


std::unique_ptr<UIElement> UILoader::createTreeListNode (DataMap::DataNode *elementNode) {

	if (treeList == nullptr)
		return nullptr;

	std::unique_ptr<UIElement> element = std::make_unique<UITreeListNode> (manager, treeList);
	auto listNode = dynamic_cast<UITreeListNode*>(element.get());

	listNode->caption   = readUString (elementNode, "caption");
	listNode->allowHide = readBool    (elementNode, "allowhide", true);

	readIcon (listNode, elementNode);
	return element;

}


std::unique_ptr<UIElement> UILoader::createListMenu (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UIListMenu> (manager);
	auto listMenu = dynamic_cast<UIListMenu*>(element.get());

	listMenu->transparent = readBool (elementNode, "transparent");
	listMenu->popup       = readBool (elementNode, "popup");
	listMenu->checkList   = readBool (elementNode, "checklist");

	return element;

}


std::unique_ptr<UIElement> UILoader::createDropMenu (DataMap::DataNode *elementNode) {

	bool flat = readBool(elementNode, "flat");
	std::unique_ptr<UIElement> element = std::make_unique<UIDropMenu> (manager, flat);
	auto listMenu = dynamic_cast<UIDropMenu*>(element.get());

	listMenu->caption = readUString (elementNode, "caption");

	return element;

}


std::unique_ptr<UIElement> UILoader::createMenuItem (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UIMenuItem> (manager);
	auto menuItem = dynamic_cast<UIMenuItem*>(element.get());

	menuItem->caption   = readUString (elementNode, "caption");
	menuItem->shortKey  = readUString (elementNode, "shortkey");
	menuItem->textAlign = readAlign   (elementNode, "textalign");

	if (menuItem->textAlign == Align::None)
		menuItem->textAlign = Align::Left;

	readIcon (menuItem, elementNode);
	return element;

}


std::unique_ptr<UIElement> UILoader::createMenuHr (DataMap::DataNode *elementNode) {

	return std::make_unique<UIMenuHr> (manager);

}

std::unique_ptr<UIElement> UILoader::createTallMenuItem (DataMap::DataNode *elementNode) {

	std::unique_ptr<UIElement> element = std::make_unique<UITallMenuItem> (manager);
	auto menuItem = dynamic_cast<UITallMenuItem*>(element.get());

	menuItem->caption   = readUString (elementNode, "caption");
	menuItem->desc      = readUString (elementNode, "desc");
	menuItem->textAlign = readAlign   (elementNode, "textalign");
	menuItem->loadImage (app->resPath+"/"+readString(elementNode, "src"));

	if (menuItem->textAlign == Align::None)
		menuItem->textAlign = Align::Left;

	readIcon (menuItem, elementNode);
	return element;

}
