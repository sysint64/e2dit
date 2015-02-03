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

/** @file data_render.cpp
    @brief Render Data : Vertices, Texcoord ...
*/

#include "data_render.h"

/* Constructor & Destructor */

DataRender::DataRender() {

	app = Application::getInstance();
	renderMode = GL_TRIANGLES;

}

DataRender::~DataRender() {

	glDeleteBuffers (1, &vId);
	glDeleteBuffers (1, &tId);
	glDeleteBuffers (1, &iId);

}

/* Initialize VBO */

void DataRender::createVBO (const bool dynamic) {

	glGenBuffers (1, &vId);
	glBindBuffer (GL_ARRAY_BUFFER, vId);

	const int vbufSize = sizeof(GLfloat)*2*vertexBuffer.size();

	if (!dynamic) {

		glBufferData (GL_ARRAY_BUFFER, vbufSize, &vertexBuffer[0], GL_STATIC_DRAW);

	} else {

		glBufferData    (GL_ARRAY_BUFFER, vbufSize, 0, GL_STREAM_DRAW);
		glBufferSubData (GL_ARRAY_BUFFER, 0, vbufSize, &vertexBuffer[0]);

	}

	/* TexCoord Buffer */

	glGenBuffers (1, &tId);
	glBindBuffer (GL_ARRAY_BUFFER, tId);
	glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat)*2*vbufSize, &texCoordsBuffer[0], GL_STATIC_DRAW);

	/* Indices Buffer */

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

/* Initialize VAO */

void DataRender::createVAO_21() {

	glGenVertexArrays (1, &VAO);
	glBindVertexArray (VAO);

	/* Vertices */

	glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2*vertexBuffer.size(), 0);
	glEnableClientState   (GL_VERTEX_ARRAY);
	glBindBuffer          (GL_ARRAY_BUFFER, vId);
	glVertexPointer       (2, GL_FLOAT, 0, NULL);

	/* Tex Coords */

	glEnableClientState   (GL_TEXTURE_COORD_ARRAY);
	glBindBuffer          (GL_ARRAY_BUFFER, tId);
	glTexCoordPointer     (2, GL_FLOAT, 0, NULL);

}

void DataRender::createVAO_33() {

	glGenVertexArrays (1, &VAO);
	glBindVertexArray (VAO);

	int BindingIndex = 0;
	int attrLoc      = 0;

	/* Vertices */

	attrLoc = 0; // in_Position

	glBindBuffer              (GL_ARRAY_BUFFER, vId);
	glEnableVertexAttribArray (attrLoc);
	glVertexAttribPointer     (attrLoc, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	/* Tex Coords */

	attrLoc = 1; // in_TexCoord

	glBindBuffer              (GL_ARRAY_BUFFER, tId);
	glEnableVertexAttribArray (attrLoc);
	glVertexAttribPointer     (attrLoc, 2, GL_FLOAT, GL_FALSE, 0, NULL);

}

void DataRender::createVAO_43() {

}

/* Render */

void DataRender::bindVBO() const {

	/* Using VAO */

	if (app->VAOEXT) {

		glBindVertexArray (VAO);
		glBindBuffer      (GL_ELEMENT_ARRAY_BUFFER, iId);

		return;

	}

	/* Without VAO */

	/* TexCoords */

	glEnableClientState   (GL_TEXTURE_COORD_ARRAY);
	glBindBuffer          (GL_ARRAY_BUFFER, tId);
	glTexCoordPointer     (2, GL_FLOAT, 0, NULL);

	/* Vertices */

	glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2*vertexBuffer.size(), 0);
	glEnableClientState   (GL_VERTEX_ARRAY);
	glBindBuffer          (GL_ARRAY_BUFFER, vId);
	glVertexPointer       (2, GL_FLOAT, 0, NULL);

	/* Indices */

	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, iId);

}

void DataRender::renderVBO() {

	/* Check to create VBO */

	if (!created) {

		createVBO();
		return;

	}

	if (renderMode == GL_NONE)
		return;

	/* Render */

	bindVBO();
	render();

}

void DataRender::render() const {

	glDrawElements (renderMode, indices.size(), GL_UNSIGNED_INT, NULL);

}
