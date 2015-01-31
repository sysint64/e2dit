/* E2DIT - 2D Map Editor for game
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public 
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
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

