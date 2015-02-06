/*
 * This file is part of E2DIT-GAPI.

 * E2DIT-GAPI is free software: you can redistribute it and/or modify
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

#ifndef E2DIT_RENDERER_DATA_RENDER_H
#define E2DIT_RENDERER_DATA_RENDER_H

#include "GL/glew.h"
#include <vector>
#include <glm/glm.hpp>
#include "system/application.h"

class DataRender {
private:
	GLuint vId, tId, iId;
	GLuint VAO;
	Application *app;
	bool created = false;

	void createVBO (const bool dynamic = false);
	void renderVBO ();
	void bindVBO   () const;

	void createVAO_21(); // for OpenGL 2.1
	void createVAO_33(); // for OpenGL 3.3
	void createVAO_43(); // for OpenGL 4.3

public:
	std::vector <glm::vec2> vertexBuffer;
	std::vector <glm::vec2> texCoordsBuffer;
	std::vector <GLuint>    indices;

	bool dynamic;
	int  renderMode;

	 DataRender();
	~DataRender();

	void setAttrs();
	virtual void render() const;

	inline void addVertex (glm::vec2 v, glm::vec2 tc) {

		vertexBuffer   .push_back (v);
		texCoordsBuffer.push_back (tc);

	}

	inline void addIndex (GLuint index) {

		indices.push_back (index);

	}
};

#endif
