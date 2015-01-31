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

/** @file camera.cpp
    @brief 2D Camera
*/

#include "camera.h"

Camera::Camera (int width, int height) : viewWidth (width), viewHeight (height) {

	setPosition (glm::vec2(0, 0));
	needUpdate = true;
	zoom = 1.f;

}

void Camera::setPosition (glm::vec2 vec) {

	position = vec;
	needUpdate = true;

}

void Camera::setZoom (float zoom) {

	this->zoom = zoom;
	//NeedUpdate = true;

}

void Camera::updateMatrices() {

	viewMatrix       = glm::lookAt (glm::vec3 (position, 1), glm::vec3 (position, 0), glm::vec3(0, 1, 0));
	projectionMatrix = glm::ortho  (0.0f, (float) viewWidth, 0.0f, (float) viewHeight/*, -1.0f, 10.0f*/);

	if (zoom > 1.f) {
		
		glm::mat4 sm = glm::scale  (glm::mat4 (1.0f), glm::vec3 (zoom, zoom, 1.0f));
		MVPMatrix = projectionMatrix*sm*viewMatrix; // Result Matrix

	} else {
		
		MVPMatrix = projectionMatrix*viewMatrix; // Result Matrix

	}

}

void Camera::update() {
	
	if (needUpdate)
		updateMatrices();

}
