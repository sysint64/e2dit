/*
 * This file is part of E2DIT-GAPI.

 * E2DIT-GAPI is free library: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.

 * E2DIT-GAPI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with E2DIT-GAPI. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Kabylin Andrey <andrey@kabylin.ru>
 */

#ifndef H_E2DIT_RENDERER_SHADER
#define H_E2DIT_RENDERER_SHADER

#include <string>
#include <map>
#include "string.h"
#include "GL/glew.h"
#include "utility/application.h"

class Shader {
private:
	typedef std::map<std::string, GLuint> Locations;
	Application *app;

	GLint shaderStatus  (GLuint &shader , GLenum param) const;
	GLint programStatus (GLuint &program, GLenum param) const;

	void load (const char *fileName);

public:

	std::string fileName;
	Locations locations;

	GLuint program;

	void bind() const;
	void unbind() const;

	Shader (const char *fileName);
	Shader (const char *fileName, std::vector<std::string> vlocs);
};

#endif
