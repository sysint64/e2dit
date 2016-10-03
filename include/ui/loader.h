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
#include "ui/toolbar.h"
#include "utility/data_map.h"
#include "utility/strings_res.h"
#include "utility/filesystem.h"
#include "renderer/color.h"

namespace ui {

	class Loader {
	private:
		Application *app      = Application::getInstance();
		TreeList  *treeList = nullptr;
		ui::Toolbar   *toolbar  = nullptr;

		void placeElements (DataMap::DataNode *rootNode, Widget *uiParent);

		std::unique_ptr<Widget> createElement      (DataMap::DataNode *elementNode);

		std::unique_ptr<Widget> createPanel        (DataMap::DataNode *elementNode);
		std::unique_ptr<Widget> createButton       (DataMap::DataNode *elementNode);
		std::unique_ptr<Widget> createEdit         (DataMap::DataNode *elementNode);
		std::unique_ptr<Widget> createStackLayout  (DataMap::DataNode *elementNode);
		std::unique_ptr<Widget> createToolbar      (DataMap::DataNode *elementNode);
		std::unique_ptr<Widget> createToolbarTab   (DataMap::DataNode *elementNode);
		std::unique_ptr<Widget> createToolbarItem  (DataMap::DataNode *elementNode);
		std::unique_ptr<Widget> createColorPanel   (DataMap::DataNode *elementNode);
		std::unique_ptr<Widget> createDialog       (DataMap::DataNode *elementNode);
		std::unique_ptr<Widget> createImage        (DataMap::DataNode *elementNode);
		std::unique_ptr<Widget> createLabel        (DataMap::DataNode *elementNode);
		std::unique_ptr<Widget> createGrouped      (DataMap::DataNode *elementNode);
		std::unique_ptr<Widget> createDropMenu     (DataMap::DataNode *elementNode);
		std::unique_ptr<Widget> createListMenu     (DataMap::DataNode *elementNode);
		std::unique_ptr<Widget> createMenuItem     (DataMap::DataNode *elementNode);
		std::unique_ptr<Widget> createMenuHr       (DataMap::DataNode *elementNode);
		std::unique_ptr<Widget> createTallMenuItem (DataMap::DataNode *elementNode);
		std::unique_ptr<Widget> createTreeList     (DataMap::DataNode *elementNode);
		std::unique_ptr<Widget> createTreeListNode (DataMap::DataNode *elementNode);

		int                readInt     (DataMap::DataNode *elementNode, const std::string &paramName, const int defaultVal = 0);
		float              readFloat   (DataMap::DataNode *elementNode, const std::string &paramName, const float defaultVal = 0.f);
		bool               readBool    (DataMap::DataNode *elementNode, const std::string &paramName, const bool defaultVal = false);
		std::string        readString  (DataMap::DataNode *elementNode, const std::string &paramName, const std::string &defaultVal = "");
		std::wstring       readUString (DataMap::DataNode *elementNode, const std::string &paramName, const std::wstring &defaultVal = L"");

		std::array<int, 4> readRect    (DataMap::DataNode *elementNode, const std::string &paramName, const bool autoFill = true);
		gapi::Color        readColor   (DataMap::DataNode *elementNode, const std::string &paramName);
		Align              readAlign   (DataMap::DataNode *elementNode, const std::string &paramName);
		void               readIcon    (Button *element, DataMap::DataNode *elementNode);

	public:
		std::unique_ptr<DataMap> data = nullptr;
		StringRes *stringsRes;
		Manager *manager = nullptr;
		Widget *root = nullptr;

		Loader (Manager *manager) : manager (manager) {
			data = std::make_unique<DataMap> ();
		}

		Loader (Manager *manager, StringRes *stringsRes, const std::string &fileName,
			Widget *root = nullptr, DataMap::ReadType rt = DataMap::ReadType::Text)
				: manager(manager), stringsRes(stringsRes), root(root)
		{

			data = std::make_unique<DataMap>();
			data->hierarchy = true;

			if (root == nullptr)
				this->root = manager->root.get();

			switch (rt) {

				case DataMap::ReadType::Bin  : loadFromBin  (app->resPath+"/ui/layouts/"+fileName); break;
				case DataMap::ReadType::Text : loadFromText (app->resPath+"/ui/layouts/"+fileName); break;
				default : return;

			}

		}

		inline void loadFromText (const std::string &fileName) {

			data->loadFromText (fileName.c_str());
			placeElements (data->root.get(), root);

		}

		inline void loadFromBin (const std::string &fileName) {

			data->loadFromBin (fileName.c_str());
			placeElements (data->root.get(), root);

		}

	};

};
