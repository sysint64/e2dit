/*
 * E2DIT - is a 2D Map Editor to create a levels for 2d games
 * Copyright (C) 2015 Kabylin Andrey <andrey@kabylin.ru>

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
 */

#include <utility/filesystem.h>
#include "renderer/shader.h"

GLint gapi::Shader::shaderStatus (GLuint &shader, GLenum param) const {

	GLint  status, length;
	GLchar buffer[1024];

	glGetShaderiv (shader, param, &status);

	if (status != GL_TRUE) {

		glGetShaderInfoLog (shader, 1024, &length, buffer);
		app->log.write (("Shader error ("+fileName+"): %s\n").c_str(), (const char*) buffer);

	}

	return status;

}

GLint gapi::Shader::programStatus (GLuint &program, GLenum param) const {

	GLint status, length;
	GLchar buffer[1024];

	glGetProgramiv (program, param, &status);

	if (status != GL_TRUE) {

		glGetProgramInfoLog (program, 1024, &length, buffer);
		app->log.write (("Shader program error ("+fileName+"): %s\n").c_str(), (const char*) buffer);

	}

	return status;

}

void gapi::Shader::bind() const {

	glUseProgram (program);

}

void gapi::Shader::unbind() const {

	glUseProgram (0);

}

void gapi::Shader::load (const std::string &fileName) {

	app = Application::getInstance();

	/* Open Shader File */

	this->fileName = fileName;

	if (!fs::exists (fileName)) {

		Application::getInstance()->log.ewrite ("Load shader error, file doesn't exist : %s", fileName.c_str());
		return;

	}

	FILE *in = fopen (fileName.c_str(), "r");

	/* Text Buffers */

	char fp[20048], vp[20048]; // Text Buffers
	int  fi  = 0;  int vi = 0; // Counters

	/* Current Text Buffer & Counter */

	char *cp = fp;  // Current Buffer
	int  *ci = &fi; // Current Counter

	char ch; /* Current Char */

	while (!feof (in)) {
		ch = fgetc (in);

		/* if is not macros */

		if (ch != '#') {

			/* just add new char on current buffer */

			cp[*ci] = ch; (*ci)++;
			continue;

		}

		/* Else Read Macros */

		char id[256]; // Macros Buffer
		int  ii = 0;

		ch = fgetc (in);

		while (ch != EOF && ch != '\n' && ch != '\r') {

			id[ii] = ch; ii++;
			ch = fgetc (in);

		}

		/* end macros */

		id[ii] = '\0';

		/* Check shader type & select need buffer */

		if (!strcmp (id ,"vertex shader\0")) {

			/* It is Vertex shader, set Vertex buffer
			   and counter */

			cp = vp;
			ci = &vi;

		} else if (!strcmp (id ,"fragment shader\0")) {

			/* It is Fragment shader, set Fragment buffer
			   and counter */

			cp = fp;
			ci = &fi;

		} else {

			/* It is other macros */

			cp[*ci] = '#'; (*ci)++;

			/* Copy Macros name to current buffer */

			for (int i = 0; i < ii; i++) {

				cp[*ci] = id[i];
				(*ci)++;

			}

		}

	}

	fp[fi-1] = '\0'; vp[vi-1] = '\0';
	fclose (in);

	/* Create Shader */

	const char *ff = fp;
	const char *vv = vp;

	/* Create Vertex Shader */

	GLuint vs = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource  (vs, 1, &vv, nullptr);
	glCompileShader (vs);
	shaderStatus    (vs, GL_COMPILE_STATUS);

	/* Create Fragment Shader */

	GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
	glShaderSource  (fs, 1, &ff, nullptr);
	glCompileShader (fs);
	shaderStatus    (fs, GL_COMPILE_STATUS);

	/* Create Program  */

	program = glCreateProgram();

	/* Link shaders */

	glAttachShader (program, vs); /**/ programStatus (program, GL_LINK_STATUS);
	glAttachShader (program, fs); /**/ programStatus (program, GL_LINK_STATUS);

	/* Validate program */

	glValidateProgram (program);
	programStatus     (program, GL_VALIDATE_STATUS);

	glLinkProgram     (program);

}

gapi::Shader::Shader (const std::string &fileName) {

	load (fileName);

}

gapi::Shader::Shader (const std::string &fileName, std::vector<std::string> vlocs) {

	load (fileName);

	for (auto loc : vlocs) {

		locations[loc] = glGetUniformLocation (program, loc.c_str());

	}

}
