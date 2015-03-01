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
 * Get char from stream and count line/pos
 * @return char from stram
 */

char DataMap::lexChar() {

	int buf;

	/* if End of file return EOF symbol */

	if (feof(inFile)) { return EOF; }
	
	/* Read new Char */

	fread (&buf, sizeof(char), 1, inFile);
	offsetStack[offsetStack.size()-1]++;

	/* Char position */

	posno++;

	/* Skip \r */

	if (buf == '\r') {

		/* Read new Char */

		fread (&buf, sizeof(char), 1, inFile);
		offsetStack[offsetStack.size()-1]++;
		//tabSize = 0;
		//puts("R");

		/* Skip \n */
		
		if (buf == '\n') {

			/* new line & new pos */

			lineno++;
			posno = 1;
			//tabSize = 0;
			//puts("N");

		}

	} else if (buf == '\n') {

		/* if \n then
		   new line & new pos */

		lineno++;
		posno = 1;
		//tabSize = 0;
		//puts("N");

	}

	/* Return char from stream */
	
	return buf;
}

/**
 * Open new parser file
 */

void DataMap::lexOpenFile (const char *fileName) {
	
	fclose (inFile);
	inFile = fopen (fileName, "r");
	offsetStack.push_back(0);

}

/**
 * Reopen parsered file with offset
 */

void DataMap::lexReopenFile (const char *fileName) {

	fclose (inFile);
	inFile = fopen (fileName, "r");
	offsetStack.pop_back();
	fseek (inFile, offsetStack.back(), SEEK_CUR);

}

/**
 * Get next token
 */

char DataMap::lexNextToken (const bool skipTab) {

	if (nts < tokenStack.size()) {

		nts++;
		lexPopToken();
		
		return tokenStack[nts].token;

	} else {

		int tok = lexToken (skipTab);
		curToken = tok;
		lexPushToken(); // push to stack
		nts = tokenStack.size();

		return tok;

	}

}

/**
 * Get prev token
 */

char DataMap::lexPrevToken (const bool skipTab) {


	nts--;
	lexPopToken();

	if (skipTab)
	while (tokenStack[nts-1].token == tok_tab) {
		nts--;
		lexPopToken();
	}
	
	return tokenStack[nts-1].token;

}

/**
 * Push new Token to Stack
 */

void DataMap::lexPushToken() {

	tok Tok;
	Tok.token = curToken;
	Tok.idstr = idStr;
	Tok.num   = numVal;
	
	tokenStack.push_back(Tok);

}

/**
 * Pop Token from Stack
 */

void DataMap::lexPopToken() {

	curToken = tokenStack[nts-1].token;
	idStr    = tokenStack[nts-1].idstr;
	numVal   = tokenStack[nts-1].num;

}

/**
 * @return next token from stream
 */

char DataMap::lexToken (const bool skipTab) {

	/* Skip spaces */

	while (isspace (lastChar) && (lastChar != '\t' || skipTab))
		lastChar = lexChar();

	/* Tab Token */

	if (!skipTab && lastChar == '\t') {

		lastChar = lexChar();
		return tok_tab;

	}

	/* if end of file, return tok_eof */

	if (feof(inFile))
		return tok_eof;

	/* */

	/* String or char */

	if (lastChar == '\"') return lexTokString();

	/* Identifier: [a-zA-Z_][a-zA-Z0-9_]* */

	if (isalpha (lastChar) || lastChar == '_') return lexTokId();

	/* Number Float or Integer: [0-9] (.[0-9]+)? (E[+-]? [0-9]+)? */

	if (isdigit(lastChar) || lastChar == '-') return lexTokNumber();

	/* Comment or '/' */

	if (lastChar == '/') return lexComment();

	// return as ASCII

	int thisChar = lastChar;
	lastChar     = lexChar();
	
	return thisChar;

}

/**
 * @return String token
 */

char DataMap::lexTokString() {

	std::string strVal;
	idStr = "";
	int state = 0;
	
	/* State machine */

	while (state != 2) {

		switch (state) {
			case 0 :

				do {
					lastChar = lexChar();

					if (lastChar != '\"') {
						idStr += lastChar;
					}
				} while (lastChar != '\"' && lastChar != EOF);
	
				if (lastChar == EOF) {
					
					/* TODO Error */

				} else lastChar = lexChar();

				if (lastChar == '#')  { state = 1; break; }
				if (lastChar == '\"') { idStr += '\"'; state = 0; break; }

				state = 2;
				break;

			case 1 :

				std::string numStr = "";
				lastChar = lexChar();

				while (isdigit(lastChar)) {
					numStr += lastChar;//
					lastChar = lexChar();
				} 

				//strtod(numStr.c_str(), 0);
				char buff[3];

				idStr += buff;

				if (lastChar == '\"') { state = 0; break; }
				if (lastChar == '#')  { state = 1; break; }

				state = 2;
				break;
		}
	}

	return tok_string;

}

/**
 * @return identifier token
 */

char DataMap::lexTokId () {

	idStr = lastChar;
	
	while (true) {

		lastChar = lexChar();
		
		if (isdigit(lastChar) || isalpha(lastChar) || lastChar == '_') {
			idStr += lastChar;
			continue;
		}
		
		break;

	}
	
	std::transform (idStr.begin(), idStr.end(), idStr.begin(), ::tolower);
	
	// Key Words

	if (idStr == "end")			return tok_end;
	if (idStr == "include")		return tok_include;
	if (idStr == "false")		{ numVal = 0  ; return tok_number; }
	if (idStr == "true")		{ numVal = 1.f; return tok_number; }
	
	// Other Id

	return tok_id;

}

/**
 * [0-9] (.[0-9]+)? (E[+-]? [0-9]+)?
 * @return number token
 */

char DataMap::lexTokNumber () {

	int stat = 2;
		
		bool neg = false;
		std::string numStr;
		std::string scaleStr;
		int power = 0;
		
		/* State machine */

		while (stat < 8) {

			switch (stat) {
				case 2 :
					numStr += lastChar;
					lastChar = lexChar();
						
					if (isdigit(lastChar)) {
						stat = 2;
					} else if (lastChar == '.') {
						stat = 3;
					} else if (lastChar == 'E' || lastChar == 'e') {
						stat = 5;
					} else stat = 9; /* Final State */
					
					break;
					
				/* Comma */

				case 3 :
					numStr += lastChar;
					lastChar = lexChar();
					
					if (isdigit(lastChar)) {
						stat = 4;
					} else stat = 9; /* TODO ERROR! */
					
					break;
					
				case 4 :
					numStr += lastChar;
					lastChar = lexChar();
						
					if (isdigit(lastChar)) {
						stat = 4;
					} else if (lastChar == 'E' || lastChar == 'e') {
						stat = 5;
					} else stat = 10; /* Final State */
					
					break;
					
				/* Power */

				case 5 :
					lastChar = lexChar();
					
					if (lastChar == '+' || lastChar == '-') {
						stat = 6;
					} else if (isdigit(lastChar)) {
						stat = 7;
					} else ; /* TODO ERROR! */
					
					break;
					
				case 6 :
					scaleStr += lastChar;
					lastChar = lexChar();
					
					if (isdigit(lastChar)) {
						stat = 7;
					} else ; /* TODO ERROR! */
						
					break;
					
				case 7 :
					scaleStr += lastChar;
					lastChar = lexChar();
					
					if (isdigit(lastChar)) {
						stat = 7;
					} else stat = 8; /* Final State */
					
					break;
					
				default : ;
			}

		} 

		/* Convert string to Number */
		
		numVal = strtod (numStr.c_str(), 0);
		
		if (scaleStr != "") power = strtod(scaleStr.c_str(), 0);
		
		numVal = numVal*pow(10, power);
		if (neg) numVal = -numVal;
		
		return tok_number;

}

/**
 * Skip comment
 */

char DataMap::lexComment () {

	int thisChar = lastChar;
	lastChar     = lexChar();

	// Single line

	if (lastChar == '/') {
		
		/* Skip comment */

		while (lastChar != EOF && lastChar != '\n' && lastChar != '\r')
			lastChar = lexChar();
		
		/* Return Next Token */

		if (lastChar != EOF)
			return lexToken();
	}
	
	// Multi line

	if (lastChar == '*') {

		/* skip comment */

		while (lastChar != EOF) {

			lastChar = lexChar();
			
			if (lastChar == '*') {

				lastChar = lexChar();
				if (lastChar == '/') { lastChar = lexChar(); break; }

			}

		}

		/* Return next token */
		
		if (lastChar != EOF)
			return lexToken();
	}
	
	return thisChar;

}
