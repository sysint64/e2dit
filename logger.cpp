/*
 * E2DIT - is a 2D Map Editor to create a levels for 2d games
 * Copyright (C) 2015 Kabylin Andrey <andrey@kabylin.ru>

 * This file is part of E2DIT.

 * E2DIT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * E2DIT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with E2DIT.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Kabylin Andrey <andrey@kabylin.ru>
 */

/** @file logger.cpp
    @brief make logs
*/

#include "logger.h"

#include <cstdio>
#include <cstring>
#include <cstdarg>

void Logger::create (const char *fileName) {

	FILE *output;

	if ((output = fopen (fileName, "w")) != NULL)
		fclose(output);

	this->fileName = fileName;

}

void Logger::write (const char *format, ...) {

	va_list ap;
	FILE    *output;

	if ((output = fopen (fileName.c_str(), "a+")) == NULL)
		return;

	va_start (ap, format);
	vfprintf (output, format, ap);
	va_end   (ap);

	fclose   (output);

}
