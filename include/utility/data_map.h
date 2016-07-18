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

#ifndef E2DIT_UTILITY_DATA_MAP_H
#define E2DIT_UTILITY_DATA_MAP_H

#include <vector>
#include <string>
#include <map>
#include <memory>
#include "stdio.h"
#include "utility/application.h"
#include "utility/exceptions.h"
#include "utility/filesystem.h"

class DataMap {
public:

	enum class ReadType { Text, Bin };

	/* Data Value */

	struct DataVal {

		float        num;
		std::string  str;
		std::wstring wstr;
		bool         isStr;
		bool         boolean;
		int          intval;

	} typedef DataVal;

	/* Data Functions */

	struct DataParams {

		std::map <std::string, std::vector<DataVal> > params;
		std::string type = "undef";
		bool def;

	} typedef DataParams;

	struct DataNode {

		std::map <std::string, std::vector<DataVal> > params;
		std::string type = "undef";
		std::string name = "";
		DataNode *parent = nullptr;
		std::vector <std::unique_ptr<DataNode>> childs;

	} typedef DataNode;

private:

	/* In File */

	FILE        *inFile;

	/* Parser */

	char         curToken;
	std::string  idStr;
	float        numVal;
	int          lastChar = ' ';
	int          posno, lineno; // Line & Pos
	bool         end = false;
	bool         merge = false;

	/* Data */

	std::vector<int> bytecode;
	std::vector<int> offsetStack; // Files offset in bytes

	/**/
	Application *app = Application::getInstance();

	/* Token */

	struct tok {

		int token;
		std::string idstr;
		float num;

	} typedef tok;

	std::vector<tok> tokenStack;
	int              nts = 0;
	int              lastTabs = 0;
	int              tabSize  = 0;
	int              lastHierarchyTabs = 0;
	/**/

	//std::vector<std::string> names;
	//std::vector<std::string> parents;

	std::string names  [32];
	std::string parents[32];

	//
	DataNode *parentNode = nullptr;
	DataNode *lastNode   = nullptr;

	/* Lexer */

	char lexChar();
	char lexStrLen();
	char lexToken      (const bool skipTab = true);
	char lexNextToken  (const bool skipTab = true);
	char lexPrevToken  (const bool skipTab = true);
	void lexOpenFile   (const char *fileName);
	void lexReopenFile (const char *fileName);

	void lexPushToken();
	void lexPopToken();

	/* */

	char lexTokString();
	char lexTokNumber();
	char lexTokId();
	char lexComment();

	/* Parser */

	void parse();

	void parseElement ();
	void parseElements();

	void parseFuncs (std::string name);
	void parseFunc  (std::string name);

	void parseVal();
	void parseInclude();

	std::vector<DataVal> parseParams (std::string Name);
	DataVal              parseParam  ();

public:

	/* Tokens */

	enum Token {

		tok_eof = -1, tok_tab = -2,

		tok_id = -3, tok_number = -4, tok_string = -5,
		tok_begin = -6, tok_end = -7, tok_include = -8

	};

	/* Operands */

	enum Operand {
		op_eof  = 0x00, op_elem = 0x01, op_def = 0x02,
		op_str  = 0x03, op_num  = 0x04, op_end = 0x05,
		op_type = 0x06,
	};

	/* Data */

	std::map<std::string, DataParams> element;
	bool haltIfErr = false;
	std::string fileName;

	// Hierarchy Representation

	bool hierarchy = false;
	std::unique_ptr<DataNode> root;

	/* Methods */

	DataMap ();
	DataMap (const char *fileName, ReadType rt = ReadType::Text);

	virtual void loadFromText (const char *fileName);
	virtual void loadFromBin  (const char *fileName);

	void saveToText (const char *fileName);
	void saveToBin  (const char *fileName);

	void update();

	//

	//bool readInt      (const std::string &key, DataNode *elementNode, int &res, int def);
	//bool readIntArr   (const std::string &key, DataNode *elementNode, const int size, int   *res, int   def);
	//bool readFloatArr (const std::string &key, DataNode *elementNode, const int size, float *res, float def);
	//bool readString   (const std::string &key, DataNode *elementNode, std::string &res);

};

#endif
