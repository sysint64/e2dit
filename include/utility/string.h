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

#ifndef E2DIT_UTILITY_STRING_H
#define E2DIT_UTILITY_STRING_H

#include <string>

/**
 * Decode UTF Char to sequence of bytes
 *
 * @param uc: UTF char
 * @return decoded string
 */

inline std::string decodeUTF8 (wchar_t uc) {

	std::string ret;
	unsigned char b1, b2, b3, b4;//, b5, b6;
	
	if (uc == 0x0040) return "";
	if ((uc >= 0xD800 & uc <= 0xDFFF) == 1) return "";
	
	//
	if ((uc > 0x1 & uc <= 0x007F) == 1) {
		ret += uc;
		//
	} else if ((uc > 0x007F & uc <= 0x07FF) == 1) {
		b1 = 0xC0 | (uc & 0x7C0) >> 6;
		b2 = 0x80 | (uc & 0x3F);
		//
		ret  = b1; ret += b2;
		//
	} else if ((uc > 0x07FF & uc <= 0xFFFF) == 1) {
		b1 = 0xE0 | (uc & 0xF000) >> 12;
		b2 = 0x80 | (uc & 0xFC0) >> 6;
		b3 = 0x80 | (uc & 0x3F);
		//
		ret  = b1; ret += b2; ret += b3;
		//
	} else if ((uc > 0xFFFF & uc <= 0x10FFFF) == 1) {
		b1 = 0xF0 | (uc & 0x1C00000) >> 18;
		b2 = 0x80 | (uc & 0x3F000) >> 12;
		b3 = 0x80 | (uc & 0xFC0) >> 6;
		b4 = 0x80 | (uc & 0x3F);
		//
		ret  = b1; ret += b2; ret += b3;
		ret += b4;
		//
	}/* else if ((uc > 0x1FFFFF & uc <= 0x3FFFFFF) == 1) {
		b1 = 0xF8 | (uc & 0x3000000) >> 24;
		b2 = 0x80 | (uc & 0xFC0000) >> 18;
		b3 = 0x80 | (uc & 0xF000) >> 12;
		b4 = 0x80 | (uc & 0xFC0) >> 6;
		b5 = 0x80 | (uc & 0x3F);
		//
		ret  = b1; ret += b2; ret += b3;
		ret += b4; ret += b5;
		//
	} else if ((uc > 0x3FFFFFF & uc <= 0x7FFFFFFF) == 1) {
		b1 = 0xFC | (uc & 0x40000000) >> 30;
		b2 = 0x80 | (uc & 0x3F000000) >> 24;
		b3 = 0x80 | (uc & 0xFC0000) >> 18;
		b4 = 0x80 | (uc & 0xF000) >> 12;
		b5 = 0x80 | (uc & 0xFC0) >> 6;
		b5 = 0x80 | (uc & 0x3F);
		//
		ret  = b1; ret += b2; ret += b3;
		ret += b4; ret += b5; ret += b6;
	}*/
	
	return ret;
}

/**
 * Convert std::wstring to std::string
 * 
 * @param ws: std::wstring
 * @retinr s: std::string
 */

inline std::string wstr2str (std::wstring ws) {

	std::string s;
	
	for (auto ch : ws)
		s += decodeUTF8 (ch);
	
	return s;

}

/**
 * Convert std::wstring to std::string
 * 
 * @param   s: std::string
 * @retinr ws: std::wstring
 */

inline std::wstring str2str (const std::string &s) {
	
	std::wstring ws (s.size(), L' ');
	ws.resize (mbstowcs (&ws[0], s.c_str(), s.size()));

	return ws;

}

#endif
