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

class UILoader {
private:
	void placeElements();

public:
	std::unique_ptr<DataMap> data = nullptr;
	UIManager *manager = nullptr;

	UILoader (UIManager *manager) : manager (manager) {
		data = std::make_unique<DataMap> ();
	}

	UILoader (UIManager *manager, const std::string &fileName, ReadType rt = ReadType::Text) : manager (manager) {

		data = std::make_unique<DataMap> ();
		data->hierarchy = true;

		switch (rt) {

			case ReadType::Bin  : loadFromBin  (fileName); break;
			case ReadType::Text : loadFromText (fileName); break;
			default             : return;

		}

	}

	inline void loadFromText (const std::string &fileName) {
		data->loadFromText (fileName.c_str());
		placeElements();
	}

	inline void loadFromBin (const std::string &fileName) {
		data->loadFromBin (fileName.c_str());
		placeElements();
	}

};
