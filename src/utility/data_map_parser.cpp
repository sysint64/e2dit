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
#include "utility/string.h"
#include <iostream>

/**
 *
 */

void DataMap::parse() {

	if (hierarchy) {

		root = std::make_unique<DataNode>();
		root->name = "root";
		root->parent = root.get();

		parentNode = root.get();
		lastNode   = root.get();

	}

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

/**
 *
 */

void DataMap::parseElement() {

	std::string Name = idStr;
	std::string type = "";

	bytecode.push_back (op_elem);
	bytecode.push_back (Name.size());

	for (int i = 0; i < Name.size(); i++)
		bytecode.push_back(Name[i]);

	lexNextToken (false);
	puts (Name.c_str());

	if (curToken == '(') {

		lexNextToken();
		type = idStr;

		bytecode.push_back (op_type);
		bytecode.push_back (type.size());

		for (int i = 0; i < type.size(); i++)
			bytecode.push_back(type[i]);

		lexNextToken();

		if (curToken != ')') {

			app->log.ewrite ("%s:%d:%d: Parse error", fileName.c_str(), lineno, posno);

		}

		lexNextToken ();

	}

	/*while (curToken == tok_tab) {

		lastTabs++;
		lexNextToken (false);

	}*/
	//parents.push_back (Name);

	std::vector<DataVal> params;

	if (hierarchy) {

		if (lastTabs > lastHierarchyTabs) {

			parentNode = lastNode;

		} else

		while (lastTabs < lastHierarchyTabs) {

			parentNode = parentNode->parent;
			lastHierarchyTabs--;

		}

		auto node = std::make_unique<DataNode>();
		node->name = Name;
		node->type = type;
		node->parent = parentNode;
		lastNode = node.get();
		parentNode->childs.push_back(std::move(node));

		puts (type.c_str());

	} else {

		if (lastTabs == 0) {

			params.push_back ({0, "Root", L"", true});

			element[Name].params["parent"] = params;
			element[Name].type             = type;

		} else {

			params.push_back ({0, parents[lastTabs-1], L"", true});

			element[Name].params["parent"] = params;
			element[Name].type             = type;

		}

	}

	printf("%d, %d\n", lastTabs, lastHierarchyTabs);
	lastHierarchyTabs = lastTabs;

	if (hierarchy && parentNode != nullptr)
		puts (("parent: "+parentNode->name).c_str());

	if (merge) {

		merge = false;
		Name = parents[lastTabs-1]+Name;
		//std::string main = Name;
		//Name = "";

		//for (int i = 0; i < lastTabs; i++) {

		//Name += parents[i];
			//puts (parents[i].c_str());

		//}

		//Name += main;
	} else {
		if (lastTabs-1 >= 0)
			Name = parents[lastTabs-1]+"."+Name;
	}

	parents[lastTabs] = Name;

	/*for (auto t : parents) {

		std::cout << t << std::endl;

	}*/
	//if ()

	//std::cout << (int) curToken << idStr << std::endl;
	/*if (curToken != ':') {

		app->log.ewrite ("%s:%d:%d: Parse error", fileName.c_str(), lineno, posno);

	}*/

	//
	/*if (FillType == Vector) {
		struct ElData Element;
		Element.Name = Name;
		Element.Type = Type;
		DataStack.push_back(Element);
	}*/

	parseFuncs (Name);

	/*if (curToken != tok_end) {

		app->log.ewrite ("%s:%d:%d: Parse error except 'end'", fileName.c_str(), lineno, posno);
		//Terminate(EXIT_FAILURE);
		//
		lexNextToken();
	}*/


	/*lexNextToken();

	if (curToken != ';') {

		app->log.write ("%s:%d:%d: Parse error", fileName.c_str(), lineno, posno);
		//Terminate(EXIT_FAILURE);
		//
		lexNextToken();
	}*/

	bytecode.push_back (op_end);
	//std::cout << (int) curToken << idStr << std::endl;
	//lexNextToken();

}

/**
 *
 */

void DataMap::parseElements() {

	while (curToken != tok_eof) {
		parseElement();
	}

}

/**
 *
 */

void DataMap::parseFuncs (std::string name) {

	while ((curToken != tok_end && curToken != tok_eof) == 1) {

		parseFunc(name);
		//if (curToken == tok_end) break;
		if (end) { end = false; break; }

	}

}

/**
 *
 */

void DataMap::parseFunc (std::string name) {

	if (lastTabs == 0) {
		tabSize = 0;

		while (curToken == tok_tab) {

			tabSize++;
			lexNextToken (false);

		}

	} else {

		tabSize = lastTabs+1;
		lastTabs = 0;
		//std::cout << tabSize << std::endl;

	}

	//std::cout << tabSize << std::endl;

	merge = false;

	if (curToken == '&') {

		if (tabSize == 0) tabSize = 1;
		merge = true;
		lexNextToken();
		std::cout << tabSize << std::endl;

	}

	std::string funcName = idStr;

	bytecode.push_back (op_def);
	bytecode.push_back (funcName.size());

	for (int i = 0; i < funcName.size(); i++)
		bytecode.push_back (funcName[i]);

	lexNextToken();

	/*while (curToken == tok_tab)
		lexNextToken(false);*/

	if (curToken != ':') {

		//if (tabSize-lastTabs)
		//std::cout << tabSize << std::endl;
		lexPrevToken();
		end = true;
		lastTabs = tabSize;

		return;

	}

	lexNextToken();
	std::vector<DataVal> params = parseParams (funcName);

	if (curToken == ';') lexNextToken();

	bytecode.push_back (op_end);

	// Fill
	//tabSize = 0;
	if (hierarchy) {

		lastNode->params[funcName] = params;

	} else {

		if (tabSize == 0) {

			element[name].params[funcName] = params;
			element[name].def = true;

		} else {

			element[parents[tabSize-1]].params[funcName] = params;
			element[parents[tabSize-1]].def = true;

		}

	}

}

/**
 *
 */

void DataMap::parseVal() {
}

/**
 *
 */

void DataMap::parseInclude() {

	lexNextToken();

	if (curToken != tok_string) {

		app->log.ewrite ("%s:%d:%d: Parse error except string", fileName.c_str(), lineno, posno);

	}

	std::string FN = idStr;
	//FN = fs::extractFileDir(fileName)+FN;
	//fs::path p (fileName);
	//fileName = p.parent_path().string()+FN;
	fileName = fs::extractFileDir(fileName);

	lexNextToken();

	if (curToken != ';') {

		app->log.ewrite ("%s:%d:%d: Parse error except ';'", fileName.c_str(), lineno, posno);
	}

	std::string LastFN = fileName;
	fileName = FN;

	int lastPos  = posno;
	int lastLine = lineno;

	lexOpenFile (fileName.c_str());
	posno = 1; lineno = 1;

	parse();

	fileName = LastFN;

	lexReopenFile (fileName.c_str());
	lexToken();

	parse();

	posno  = lastPos;
	lineno = lastLine;

}

/**
 *
 */

std::vector<DataMap::DataVal> DataMap::parseParams (std::string Name) {

	std::vector<DataVal> params;
	//
	while ((curToken != ';' & curToken != tok_eof) == 1) {
		DataVal param = parseParam();
		params.push_back (param);

		//if (curToken == ';') break;
		/*if (curToken != ',') {

			app->log.write ("%s:%d:%d: Parse error", fileName.c_str(), lineno, posno);
			//Terminate(EXIT_FAILURE);

			lexNextToken();
		}*/
		if (curToken != ',') {
			break;
		}

		lexNextToken (false);
	}

	return params;

}

/**
 *
 */

DataMap::DataVal DataMap::parseParam() {

	DataVal val;
	union {
		float num;
		char  bytes[4];
	} numByte;

	if (curToken == tok_string || curToken == tok_id) {
		val.isStr = true;
		val.str = idStr;
		val.wstr = str2wstr (idStr);
		//
		bytecode.push_back (op_str);
		bytecode.push_back (idStr.size());
		//
		for (int i = 0; i < idStr.size(); i++)
			bytecode.push_back(idStr[i]);
		//
	} else {
		val.isStr = false;
		val.num = numVal;
		val.intval  = floor(numVal);
		val.boolean = floor(numVal) != 0;
		//
		numByte.num = numVal;
		//
		bytecode.push_back (op_num);
		bytecode.push_back (numByte.bytes[0]);
		bytecode.push_back (numByte.bytes[1]);
		bytecode.push_back (numByte.bytes[2]);
		bytecode.push_back (numByte.bytes[3]);
	}

	lexNextToken (false);
	return val;

}
