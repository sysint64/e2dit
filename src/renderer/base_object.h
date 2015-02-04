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

/** @file base_object.h
    @brief Base Rendered Object
*/

#ifndef E2DIT_RENDERER_BASE_OBJECT_H
#define E2DIT_RENDERER_BASE_OBJECT_H

#include "data_render.h"
#include "camera.h"
#include "shader.h"
#include "../math/utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

class BaseObject {
private:
	glm::vec2   pivot;
	bool        customPivot = false;

public:
	DataRender *data;
	Camera     *camera;

	glm::mat4   modelMatrix;
	glm::mat4   MVPMatrix;

	bool        needUpdate  = true;
	bool        visible     = true;

	float       rotation;
	glm::vec2   position;
	glm::vec2   scale;

	virtual void render();

	glm::vec2 worldToScreen () const;
	glm::vec2 worldToScreen (const glm::vec2 &screenPos) const;

	void updateModelMatrix();

	void setRotation (const float ang);
	void setPosition (const glm::vec2 &pos);
	void setScale    (const glm::vec2 &scl);

};

#endif
