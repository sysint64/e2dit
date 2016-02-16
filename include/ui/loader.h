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

#pragma once

#include "ui/manager.h"
#include "utility/data_map.h"
#include "utility/strings_res.h"

class UILoader {
private:
	Application *app = Application::getInstance();

	void placeElements (DataMap::DataNode *rootNode, UIElement *uiParent);

	std::unique_ptr<UIElement> createElement (DataMap::DataNode *elementNode);

	std::unique_ptr<UIElement> createPanel   (DataMap::DataNode *elementNode);
	std::unique_ptr<UIElement> createButton  (DataMap::DataNode *elementNode);

public:
	std::unique_ptr<DataMap> data = nullptr;
	StringRes *stringsRes;
	UIManager *manager = nullptr;

	UILoader (UIManager *manager) : manager (manager) {
		data = std::make_unique<DataMap> ();
	}

	UILoader (UIManager *manager, StringRes *stringsRes, const std::string &fileName,
		DataMap::ReadType rt = DataMap::ReadType::Text) : manager(manager), stringsRes(stringsRes)
	{

		data = std::make_unique<DataMap> ();
		data->hierarchy = true;

		switch (rt) {

			case DataMap::ReadType::Bin  : loadFromBin  (fileName); break;
			case DataMap::ReadType::Text : loadFromText (fileName); break;
			default : return;

		}

	}

	inline void loadFromText (const std::string &fileName) {
		data->loadFromText (fileName.c_str());
		placeElements (data->root.get(), manager->root.get());
	}

	inline void loadFromBin (const std::string &fileName) {
		data->loadFromBin (fileName.c_str());
		placeElements (data->root.get(), manager->root.get());
	}

};
