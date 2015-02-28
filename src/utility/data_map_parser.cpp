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

/**
 *
 */

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

/**
 *
 */

void DataMap::parseElement() {

	std::string Name = idStr;
	std::string Type = "";
	//
	bytecode.push_back (op_elem);
	bytecode.push_back (Name.size());
	//
	for (int i = 0; i < Name.size(); i++)
		bytecode.push_back(Name[i]);
	//
	//
	lexNextToken();

	if (curToken == '(') {
		lexNextToken();
		Type = idStr;

		bytecode.push_back (op_type);
		bytecode.push_back (Type.size());
		//
		for (int i = 0; i < Type.size(); i++)
			bytecode.push_back(Type[i]);

		lexNextToken();
		//
		if (curToken != ')') {
			
			app->log.ewrite ("%s:%d:%d: Parse error", fileName.c_str(), lineno, posno);
			//Terminate(EXIT_FAILURE);
			//
			lexNextToken();
		}

		lexNextToken();
	}

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

	while ((curToken != tok_end & curToken != tok_eof) == 1) {

		parseFunc(name);
		//if (curToken == tok_end) break;
		if (end) { end = false; break; }

	}

}

/**
 *
 */

void DataMap::parseFunc (std::string name) {

	std::string funcName = idStr;

	bytecode.push_back (op_def);
	bytecode.push_back (funcName.size());

	for (int i = 0; i < funcName.size(); i++)
		bytecode.push_back (funcName[i]);

	lexNextToken();

	if (curToken != ':') {

		/*app->log.ewrite ("%s:%d:%d: Parse error except ':'", fileName.c_str(), lineno, posno);
		//Terminate(EXIT_FAILURE);
		//
		lexNextToken();*/
		end = true;

		//if (curToken != tok_id)
		//	app->log.ewrite ("%s:%d:%d: Parse error except element name", fileName.c_str(), lineno, posno);

		return;

	}
	
	lexNextToken();
	std::vector<DataVal> params = parseParams (funcName);

	/*if (curToken != ';') {
		
		app->log.write ("%s:%d:%d: Parse error", fileName.c_str(), lineno, posno);
		//Terminate(EXIT_FAILURE);
		
		lexNextToken();

	}*/

	bytecode.push_back (op_end);
	//lexNextToken();
	
	// Fill

	//if (FillType == Map) {
		element[name].params[funcName] = params;
		element[name].def = true;
	//}  else {
		//
	/*	for (int i = 0; i < DataStack.size(); i++) {
			//
			if (DataStack[i].Name == Name) {
				DataStack[i].Func[FuncName] = Params;
				return;
			}
		}
		//
	}*/

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
		//Terminate(EXIT_FAILURE);
	}

	std::string FN = idStr;
	//FN = fs::extractFileDir(fileName)+FN;
	fs::path p (fileName);
	this->fileName = p.parent_path().string()+FN;

	lexNextToken();

	if (curToken != ';') {

		app->log.ewrite ("%s:%d:%d: Parse error except ';'", fileName.c_str(), lineno, posno);
		//Terminate(EXIT_FAILURE);
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
	//
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
		
		lexNextToken();
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
		//
		numByte.num = numVal;
		//
		bytecode.push_back (op_num);
		bytecode.push_back (numByte.bytes[0]);
		bytecode.push_back (numByte.bytes[1]);
		bytecode.push_back (numByte.bytes[2]);
		bytecode.push_back (numByte.bytes[3]);
	}
	
	lexNextToken();
	return val;

}
