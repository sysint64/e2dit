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

/** @file data_render.h
    @brief Render Data : Vertices, Texcoord ...
*/

#ifndef E2DIT_RENDERER_DATA_RENDER_H
#define E2DIT_RENDERER_DATA_RENDER_H

#include "GL/glew.h"
#include <vector>
#include <glm/glm.hpp>
#include "../system/application.h"

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
