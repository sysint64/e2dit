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

#include "utility/strings_res.h"
#include <vector>
#include <map>

/**
 * \brief Replace all string variables in string to
 * values of variable
 *
 * @param text: Text contains string variable
 * @return: Compiled string with string variables
 */

std::wstring StringRes::parseResource (const std::wstring &text) {

	std::wstring result = L"";

	for (int i = 0; i < text.size(); ++i) {

		if (text[i] == L'@') {
			std::tuple<int, std::wstring> item = parseElement (text, i+1);
			i += std::get<0>(item);
			result += std::get<1>(item);
		} else {
			result += text[i];
		}

	}

	return result;

}

/**
 * \brief Parse text and get strings variables i.e. "@main.test" need
 * replace with varibale value; \
 * Text always split to 2 parts - path | variable, e.g. @main.item1.go
 * splits as path = "main.item1"; variable = "go". Element parsing while char is identifier char
 *
 * @param text: Text contains string variable
 * @param pos: Char position to start parsing
 * @return: size of parsered element and value of variable
 *          e.g. for "@main.item.go" has tuple (13, value of variable)
 */

#include <iostream>
std::tuple<int, std::wstring> StringRes::parseElement (const std::wstring &text, int pos) {

	std::string part = "";
	std::vector<std::string> parts;
	int i = pos;
	bool broke = false;

	for (i; i < text.size(); ++i) {

		if (!isIdentifier (text[i])) {
			if (part.size() > 0)
				parts.push_back (part);

			broke = true;
			break;
		}

		if (text[i] == L'.') {
			parts.push_back(part);
			part = "";
		} else {
			part += text[i];
		}

	}

	if (!broke) {

		if (!isIdentifier (text[i])) {
			if (part.size() > 0)
				parts.push_back (part);
		}

	}

	if (parts.size() < 2)
		return std::make_tuple (i-pos, L"");

	// Extract value from DataMap

	std::string varName  = parts.back();
	std::string pathName = "";

	for (auto it = parts.begin(); it != parts.end()-1; ++it)
		pathName += *it+".";

	pathName.pop_back(); // remove last '.'
	return std::make_tuple (i-pos, data->element[pathName].params[varName][0].wstr);

}
