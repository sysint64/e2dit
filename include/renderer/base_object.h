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

#ifndef E2DIT_RENDERER_BASE_OBJECT_H
#define E2DIT_RENDERER_BASE_OBJECT_H

#include "data_render.h"
#include "camera.h"
#include "shader.h"
#include "utility/math.h"
#include "utility/config.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

class BaseObject {
private:
	glm::vec2   pivot;
	//bool        customPivot = false;

public:
	DataRender *data;
	Camera     *camera;

	glm::mat4   modelMatrix;
	glm::mat4   MVPMatrix;

	bool        needUpdate  = true;
	bool        visible     = true;

	float       rotation = 0;
	glm::vec2   position;
	glm::vec2   scale;

	virtual void render();
	virtual ~BaseObject() {}

	BaseObject (DataRender *data, Camera *camera)
		: data (data), camera (camera) {};

	glm::vec2 worldToScreen () const;
	glm::vec2 worldToScreen (const glm::vec2 screenPos) const;

	void updateModelMatrix();

	void setRotation (const float ang);
	void setPosition (const glm::vec2 pos);
	void setScale    (const glm::vec2 scl);

};

#endif
