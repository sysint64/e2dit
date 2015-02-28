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

#include "renderer/data_render.h"

/**
 * Destructor
 */

DataRender::~DataRender() {

	/* Delete all buffers */

	glDeleteBuffers (1, &vId);
	glDeleteBuffers (1, &tId);
	glDeleteBuffers (1, &iId);

}

/**
 * Create VBO
 * @param dynamic if true then data may change
 */

void DataRender::createVBO() {

	/* Create Vertex Buffer */

	glGenBuffers (1, &vId);
	glBindBuffer (GL_ARRAY_BUFFER, vId);

	/* Get Vertex buffer Size */

	const int vbufSize = sizeof(GLfloat)*2*vertexBuffer.size();

	/* set Mode for Buffer */

	if (!dynamic) {

		/* Static Object */

		glBufferData (GL_ARRAY_BUFFER, vbufSize, &vertexBuffer[0], GL_STATIC_DRAW);

	} else {

		/* Dynamic Object */

		glBufferData    (GL_ARRAY_BUFFER, vbufSize, 0, GL_STREAM_DRAW);
		glBufferSubData (GL_ARRAY_BUFFER, 0, vbufSize, &vertexBuffer[0]);

	}

	/* Create TexCoord Buffer */

	glGenBuffers (1, &tId);
	glBindBuffer (GL_ARRAY_BUFFER, tId);
	glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat)*2*vbufSize, &texCoordsBuffer[0], GL_STATIC_DRAW);

	/* Create Indices Buffer */

	glGenBuffers (1, &iId);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, iId);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);

	/* If VAO is available, create new VAO buffer */

	if (app->VAOEXT) {

		if (app->OGLMajor >= 3) createVAO_33();
		else createVAO_21();

	}

	/* VBO was created */

	created = true;

}

/**
 * Create VAO for OpenGL 2.1
 */

void DataRender::createVAO_21() {

	/* Create VAO buffer */

	glGenVertexArrays (1, &VAO);
	glBindVertexArray (VAO);

	/* Setup Vertices */

	glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2*vertexBuffer.size(), 0);
	glEnableClientState   (GL_VERTEX_ARRAY);
	glBindBuffer          (GL_ARRAY_BUFFER, vId);
	glVertexPointer       (2, GL_FLOAT, 0, nullptr);

	/* Setup Tex Coords */

	glEnableClientState   (GL_TEXTURE_COORD_ARRAY);
	glBindBuffer          (GL_ARRAY_BUFFER, tId);
	glTexCoordPointer     (2, GL_FLOAT, 0, nullptr);

}

/**
 * Create VAO for OpenGL 3.3
 */

void DataRender::createVAO_33() {

	/* Create VAO buffer */

	glGenVertexArrays (1, &VAO);
	glBindVertexArray (VAO);

	/* Location of Attribute */

	int attrLoc = 0;

	/* Setup Vertices */

	attrLoc = 0; // in_Position

	glBindBuffer              (GL_ARRAY_BUFFER, vId);
	glEnableVertexAttribArray (attrLoc);
	glVertexAttribPointer     (attrLoc, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	/* Setup Tex Coords */

	attrLoc = 1; // in_TexCoord

	glBindBuffer              (GL_ARRAY_BUFFER, tId);
	glEnableVertexAttribArray (attrLoc);
	glVertexAttribPointer     (attrLoc, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

}

/**
 * Create VAO for OpenGL 4.3
 */

void DataRender::createVAO_43() {

}

/**
 * Bind VBO Buffers
 */

void DataRender::bind() const {

	/* Using VAO */

	if (app->VAOEXT) {

		/* bind */

		glBindVertexArray (VAO);
		glBindBuffer      (GL_ELEMENT_ARRAY_BUFFER, iId);

		/* Exit from function */

		return;

	}

	/* Without VAO */

	/* Bind TexCoords */

	glEnableClientState   (GL_TEXTURE_COORD_ARRAY);
	glBindBuffer          (GL_ARRAY_BUFFER, tId);
	glTexCoordPointer     (2, GL_FLOAT, 0, nullptr);

	/* Bin Vertices */

	glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2*vertexBuffer.size(), 0);
	glEnableClientState   (GL_VERTEX_ARRAY);
	glBindBuffer          (GL_ARRAY_BUFFER, vId);
	glVertexPointer       (2, GL_FLOAT, 0, nullptr);

	/* Bin Indices */

	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, iId);

}

/**
 * Render VBO Data
 */

void DataRender::render() {

	/* Check to create VBO */

	if (!created) {

		createVBO();
		return;

	}

	/* if Render Mode if GL_NONE */

	if (renderMode == GL_NONE) {

		/* Skip Render */

		return;

	}

	/* Render */

	bind();
	render();

}

/**
 * Render
 */

void DataRender::renderVBO() const {

	glDrawElements (renderMode, indices.size(), GL_UNSIGNED_INT, nullptr);

}
