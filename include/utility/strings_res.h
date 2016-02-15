/*
 * E2DIT - is a 2D Map Editor to create a levels for 2d games
 * Copyright (C) 2015 Kabylin Andrey <andrey@kabylin.ru>

 * This file is part of E2DIT-UTILITY.

 * E2DIT-UTILITY is free library: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * E2DIT-UTILITY is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with E2DIT-UTILITY.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Kabylin Andrey <andrey@kabylin.ru>
 */

#pragma once

#include <tuple>
#include <string>
#include "utility/data_map.h"

/*
 * \brief class for work with string resources i.e. for localizing
 * applications into other languages
 */

class StringRes {
private:
	std::unique_ptr<DataMap> data = std::make_unique<DataMap>();
	std::tuple<int, std::string> parseElement (const std::string &text, int pos);

	inline bool isIdentifier (const char ch) {

		return isalpha(ch) || isdigit(ch) || ch == '_' || ch == '.';

	}

public:
	std::string parseResource (const std::string &text);
	void addResource (const std::string &fileName,
		DataMap::ReadType rt = DataMap::ReadType::Text)
	{

		switch (rt) {

			case DataMap::ReadType::Bin  : data->loadFromBin  (fileName.c_str()); break;
			case DataMap::ReadType::Text : data->loadFromText (fileName.c_str()); break;
			default : return;

		}

	}
};
