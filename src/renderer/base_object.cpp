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

#include "base_object.h"

void BaseObject::render() {

	if (!visible)
		return;

	if (needUpdate)
		updateModelMatrix();

	data->render();

}

glm::vec2 BaseObject::worldToScreen() const {

	return position-camera->position;

}

glm::vec2 BaseObject::worldToScreen (const glm::vec2 &screenPos) const {

	return screenPos-camera->position;

}

void BaseObject::setRotation (const float ang) {

	if (feq<float>(ang, rotation))
		return;

	rotation   = ang;
	needUpdate = true;

}

void BaseObject::setPosition (const glm::vec2 &pos) {

	if (feq<float>(pos.x, position.x) && feq<float>(pos.y, position.y))
		return;

	position   = pos;
	needUpdate = true;

}

void BaseObject::setScale (const glm::vec2 &scl) {

	if (feq<float>(scl.x, scale.x) || feq<float>(scl.y, scale.y))
		return;

	scale      = scl;
	needUpdate = true;

}

void BaseObject::updateModelMatrix() {

	glm::mat4 mr = glm::rotate    (glm::mat4 (1.0f), rotation, glm::vec3 (0, 0, 1));
	glm::mat4 mt = glm::translate (glm::mat4 (1.0f), glm::vec3 (position, 0.0f));
	glm::mat4 ms = glm::scale     (glm::mat4 (1.0f), glm::vec3 (scale, 1.0f));

	modelMatrix  = mt*mr*ms;
	MVPMatrix    = modelMatrix;
	MVPMatrix    = camera->MVPMatrix*modelMatrix;
	needUpdate   = false;

}
