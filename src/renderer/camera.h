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

#ifndef E2DIT_RENDERER_CAMERA_H
#define E2DIT_RENDERER_CAMERA_H

#include "../math/glm.h"
#include <string>

class Camera {
public:

	/* Parameters */

	float zoom;
	bool needUpdate;
	glm::vec2 position;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 MVPMatrix;

	glm::ivec2 size;
	std::wstring name;

	int viewWidth;
	int viewHeight;

	/* Methods */

	Camera (int width, int height);

	/** Update camera position and set needUpdate = true for
		anew update matrices */

	void setPosition (glm::vec2 vec);

	/** Update camera zoom and set needUpdate = true for
		anew update matrices */

	void setZoom (float zoom);

	/** Update view, projection and mvp matrices and set
		needUpdate = false */

	void updateMatrices();

	/** if needUpdate then update matrices else return */

	void update();
};

#endif
