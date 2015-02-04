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

/** @file shader.cpp
    @brief Load & compile GLSL Shader
*/

#include "shader.h"

GLint Shader::shaderStatus (GLuint &shader, GLenum param) const {

	GLint  status, length;
	GLchar buffer[1024];

	glGetShaderiv (shader, param, &status);

	if (status != GL_TRUE) {

		glGetShaderInfoLog (shader, 1024, &length, buffer);
		app->log.write ("Shader error: %s\n", (const char*) buffer);

	}

	return status;

}

GLint Shader::programStatus (GLuint &program, GLenum param) const {

	GLint status, length;
	GLchar buffer[1024];

	glGetProgramiv (program, param, &status);

	if (status != GL_TRUE) {

		glGetProgramInfoLog (program, 1024, &length, buffer);
		app->log.write ("Shader program error: %s\n", (const char*) buffer);

	}

	return status;

}

void Shader::bind() const {

	glUseProgram (program);

}

void Shader::unbind() const {

	glUseProgram (0);

}

Shader::Shader (const char *fileName) {

	app = Application::getInstance();

	this->fileName = std::string (fileName);
	FILE *in = fopen (fileName, "r");

	char fp[20048], vp[20048];
	int  fi  = 0; int vi = 0;
	int  *ci = &fi;
	char *cp = fp;

	char ch;

	while (!feof (in)) {

		ch = fgetc (in);

		if (ch != '#') {

			cp[*ci] = ch; (*ci)++;
			continue;

		}

		/* Read Macros */

		char id[64];
		int  ii = 0;

		ch = fgetc (in);

		while (ch != EOF && ch != '\n' && ch != '\r') {

			id[ii] = ch; ii++;
			ch = fgetc (in);

		}

		id[ii] = '\0';

		/* Check shader type & select needed buffer */

		if (!strcmp (id ,"vertex shader\0")) {

			cp = vp;
			ci = &vi;

		} else if (!strcmp (id ,"fragment shader\0")) {

			cp = fp;
			ci = &fi;

		} else {

			cp[*ci] = '#'; (*ci)++;

			for (int i = 0; i < ii; i++) {
				cp[*ci] = id[i];
				(*ci)++;
			}

		}

	}

	fp[fi] = '\0'; vp[vi] = '\0';
	fclose (in);

	/* Create Shader */

	const char *ff = fp;
	const char *vv = vp;

	program = glCreateProgram();

	/* Vertex Shader */

	GLuint vs = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource  (vs, 1, &vv, nullptr);
	glCompileShader (vs);
	shaderStatus    (vs, GL_COMPILE_STATUS);

	/* Fragment Shader */

	GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
	glShaderSource  (fs, 1, &ff, nullptr);
	glCompileShader (fs);
	shaderStatus    (fs, GL_COMPILE_STATUS);

	/* Link shaders */

	glAttachShader  (program, fs); programStatus (program, GL_LINK_STATUS);
	glAttachShader  (program, vs); programStatus (program, GL_LINK_STATUS);

	/* Validate program */

	glValidateProgram (program);
	programStatus     (program, GL_VALIDATE_STATUS);

}
