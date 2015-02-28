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

#include "utility/logger.h"

#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <sstream>

/**
 * Create Log file
 * @param fileName
 */

void Logger::create (const char *fileName) {

	FILE *output;

	if ((output = fopen (fileName, "w")) != NULL)
		fclose(output);

	this->fileName = fileName;

}

/**
 * Write log debug or error
 *
 * @param format
 * @param ...
 */

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

/**
 * Write log debug or error with
 * runtime error
 *
 * @param format
 * @param ...
 */

void Logger::ewrite (const char *format, ...) {

	va_list  ap;
	FILE    *output;
	char     buffer[2048];

	if ((output = fopen (fileName.c_str(), "a+")) == NULL)
		return;

	va_start (ap, format);
	vfprintf (output, format, ap);
	va_end   (ap);

	va_start (ap, format);
	vsprintf (buffer, format, ap);
	va_end   (ap);

	fclose   (output);
	throw std::runtime_error (Formatter() << buffer);

}

/**
 * Write log debug or error with
 * runtime error if except
 *
 * @param stream
 * @param except
 */

void Logger::write (std::stringstream stream, const bool except) {

	FILE *output;

	if ((output = fopen (fileName.c_str(), "a+")) == NULL)
		return;

	fputs  (stream.str().c_str(), output);
	fclose (output);

	if (except)
		throw std::runtime_error (Formatter() << stream.str().c_str());

}
