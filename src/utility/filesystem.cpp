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

#include "utility/filesystem.h"

#include <unistd.h>
#include <algorithm>
#include <cstring>

#ifdef _linux_
	#include <mntent.h>
#endif

#ifdef _win_
	#include "windows.h"
#endif

#define PATH_MAX 512
namespace fs {

	std::string upFolder (const std::string &fn) {

		std::string str;
		bool upped = 0;
		int start = fn.size()-1;

		if (fn[start] == '/' || fn[start] == '\\')
			start--;

		for (int i = start; i >= 1; --i) {

			if (upped < 1 && (fn[i] == '/' || fn[i] == '\\'))
				upped++;

			if (upped >= 1)
				str += fn[i-1];

		}

		std::reverse (str.begin(), str.end());

		if (upped) return str;
		else return fn;

	}

	bool exists (const std::string &fn) {

		if (access(fn.c_str(), F_OK) != 0)
			return false;

		return true;

	}

	std::string extractFileDir (const std::string &fn) {

		if (fn == "/" || fn == "\\") return fn;
		std::string str;
		int pos = 0;

		for (int i = fn.size()-1; i >= 0; i--) {
			if (fn[i] == '/' || fn[i] == '\\') {
				pos = i;
				break;
			}
		}

		for (int i = pos; i >= 0; i--) {
			str += fn[i];
		}

		std::reverse (str.begin(), str.end());
		return str;
	}

#ifdef _linux_

	std::string getExeDir() {

		char buffer[PATH_MAX];
		size_t len = sizeof(buffer);
		char *path_end;

		if (readlink ("/proc/self/exe", buffer, len) <= 0)
			return ".";

		path_end = std::strrchr (buffer, '/');

		if (path_end == nullptr)
			return ".";

		*path_end = '\0';
		return std::string(buffer);

	}

#endif

#ifdef _win_

	inline std::string getExeDir() {
		char dir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, &dir[0]);
		return std::string(dir);
	}

#endif

}
