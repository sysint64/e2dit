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

#ifndef H_E2DIT_RENDERER_SPRITE
#define H_E2DIT_RENDERER_SPRITE

#include "renderer/data_render.h"
#include <glm/glm.hpp>

class SpriteData : public DataRender {
public:	

	SpriteData (bool dynamic = false, bool center = true, bool strip = false) {

		this->dynamic = dynamic;

		// QUAD

		if (center) {
			
			addVertex (glm::vec2 (-0.5f, -0.5f), glm::vec2 (0.0f, 1.0f));
			addVertex (glm::vec2 ( 0.5f, -0.5f), glm::vec2 (1.0f, 1.0f));
			addVertex (glm::vec2 ( 0.5f,  0.5f), glm::vec2 (1.0f, 0.0f));
			addVertex (glm::vec2 (-0.5f,  0.5f), glm::vec2 (0.0f, 0.0f));

		} else {

			addVertex (glm::vec2 (0.0f, 0.0f), glm::vec2 (0.0f, 1.0f));
			addVertex (glm::vec2 (1.0f, 0.0f), glm::vec2 (1.0f, 1.0f));
			addVertex (glm::vec2 (1.0f, 1.0f), glm::vec2 (1.0f, 0.0f));
			addVertex (glm::vec2 (0.0f, 1.0f), glm::vec2 (0.0f, 0.0f));

		}

		/* Add Indices */

		if (strip) {

			renderMode = GL_TRIANGLE_STRIP;

			addIndex(0); addIndex(3);
			addIndex(1); addIndex(2);

		} else {

			renderMode = GL_TRIANGLES;

			addIndex(0); addIndex(3); addIndex(1);
			addIndex(2); addIndex(3); addIndex(1);

		}

	}

};

#endif
