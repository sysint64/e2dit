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

#include "utility/data_map.h"

void DataMap::parse() {

	lexNextToken();
	
	while (1) {

		switch (curToken) {

			case tok_eof	 : bytecode.push_back (op_eof); return;
			case ';'		 : lexNextToken(); break;
			case tok_include : parseInclude(); break;
			case tok_id		 : parseElement(); break;
			default			 :
				
				return;
				lexNextToken();
		}

	}

}


void DataMap::parseElement () {
}

void DataMap::parseElements() {
}

void DataMap::parseFuncs (std::string name) {
}

void DataMap::parseFunc  (std::string name) {
}

void DataMap::parseVal() {
}

void DataMap::parseInclude() {
}

std::vector<DataMap::DataVal> DataMap::parseParams (std::string Name) {
}

DataMap::DataVal DataMap::parseParam  () {
}
