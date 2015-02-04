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

/** @file shader.h
    @brief Load & compile GLSL Shader
*/

#ifndef H_E2DIT_RENDERER_SHADER
#define H_E2DIT_RENDERER_SHADER

#include <string>
#include <map>
#include "string.h"
#include "GL/glew.h"
#include "../system/application.h"

class Shader {
private:
	typedef std::map<std::string, GLuint> Locations;
	Application *app;

	GLint shaderStatus  (GLuint &shader , GLenum param) const;
	GLint programStatus (GLuint &program, GLenum param) const;

public:

	std::string fileName;
	Locations locations;

	GLuint program;

	void bind() const;
	void unbind() const;

	Shader (const char *fileName);
};

#endif
