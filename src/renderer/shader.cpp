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

#include "renderer/shader.h"

/**
 * Get Shader Status, if fail, then write Error to log
 *
 * @param shader
 * @param param
 * @return GL_TRUE if all is OK
 */

GLint Shader::shaderStatus (GLuint &shader, GLenum param) const {

	GLint  status, length;
	GLchar buffer[1024];

	glGetShaderiv (shader, param, &status);

	/* Check status */

	if (status != GL_TRUE) {

		/* If fail, then write error to log */

		glGetShaderInfoLog (shader, 1024, &length, buffer);
		app->log.write ("Shader error: %s\n", (const char*) buffer);

	}

	/* return Shader Status */

	return status;

}

/**
 * Get Program Status, if fail, then write Error to log
 *
  * @param program
  * @param param
  * @return GL_TRUE if all is OK
 */

GLint Shader::programStatus (GLuint &program, GLenum param) const {

	GLint status, length;
	GLchar buffer[1024];

	glGetProgramiv (program, param, &status);

	/* Check status */

	if (status != GL_TRUE) {

        /* If fail, then write error to log */

		glGetProgramInfoLog (program, 1024, &length, buffer);
		app->log.write ("Shader program error: %s\n", (const char*) buffer);

	}

	/* return Program Status */

	return status;

}

/**
 * Bin Shader
 */

void Shader::bind() const {

	glUseProgram (program);

}

/**
 * Unbind Shader
 */

void Shader::unbind() const {

	glUseProgram (0);

}

/**
 * Load And Create Shader
 * @param fileName
 */

void Shader::load (const char *fileName) {

	/* Get Application Instance */

	app = Application::getInstance();

	/* Open Shader File */

	this->fileName = std::string (fileName);
	FILE *in = fopen (fileName, "r");

	/* Text Buffers */

	char fp[20048], vp[20048]; // Text Buffers
	int  fi  = 0;  int vi = 0; // Counters

	/* Current Text Buffer & Counter */

    char *cp = fp;  // Current Buffer
	int  *ci = &fi; // Current Counter

	/* Current Char */

	char ch;

	/* Traverse all file */

	while (!feof (in)) {

		/* Get Char */

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

		/* Get new char */

		ch = fgetc (in);

		/* While not end line or file */

		while (ch != EOF && ch != '\n' && ch != '\r') {

			/* Add new char on macros name & get next char */

			id[ii] = ch; ii++;
			ch = fgetc (in);

		}

		/* end mactos */

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

	/* End File, close */

	fp[fi] = '\0'; vp[vi] = '\0';
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

	glAttachShader (program, fs); /**/ programStatus (program, GL_LINK_STATUS);
	glAttachShader (program, vs); /**/ programStatus (program, GL_LINK_STATUS);

	/* Validate program */

	glValidateProgram (program);
	programStatus     (program, GL_VALIDATE_STATUS);

}

/**
 * Constructor, load shader
 * @param fileName
 */

Shader::Shader (const char *fileName) {

	load (fileName);

}

/**
 * Constructor, load shader & initialize Shader Locations
 *
 * @param fileName
 * @param vloc: Shader Location Names
 */

Shader::Shader (const char *fileName, std::vector<std::string> vlocs) {

	load (fileName);

	for (auto loc : vlocs) {

		locations[loc] = glGetUniformLocation (program, loc.c_str());

	}

}
