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

#include "renderer/base_object.h"

/**
 * Render Data & update matrices
 */

void BaseObject::render() {

	/* if is invisible, then no render */

	if (!visible)
		return;

	/* If matrices was change, then update it */

	if (needUpdate)
		updateModelMatrix();

	/* Render Data */

	data->render();

}

/**
 * Convert world Position to Screen
 * @return Screen Position
 */

glm::vec2 BaseObject::worldToScreen() const {

	return position-camera->position;

}

/**
 * Convert Custom Position to Screen
 *
 * @param screenPos
 * @return Screen Position
 */

glm::vec2 BaseObject::worldToScreen (const glm::vec2 &screenPos) const {

	return screenPos-camera->position;

}

/**
 * Update Rotation
 * @param ang new Angle
 */

void BaseObject::setRotation (const float ang) {

	/* Check current Angle */

	if (feq<float>(ang, rotation))
		return;

	/* If angles is difference, then update Angle
	   and need update matrices */

	rotation   = ang;
	needUpdate = true;

}

/**
 * Update Position
 * @param pos new Position
 */

void BaseObject::setPosition (const glm::vec2 &pos) {

	/* Check current Position */

	if (feq<float>(pos.x, position.x) && feq<float>(pos.y, position.y))
		return;

	/* If positions is difference, then update Position
	   and need update matrices */

	position   = pos;
	needUpdate = true;

}

/**
 * Update Position
 * @param scl new Scale
 */

void BaseObject::setScale (const glm::vec2 &scl) {

	/* Check current Scale */

	if (feq<float>(scl.x, scale.x) || feq<float>(scl.y, scale.y))
		return;

	/* If Scales is difference, then update Scale
	   and need update matrices */

	scale      = scl;
	needUpdate = true;

}

/**
 * Update Matrices
 */

void BaseObject::updateModelMatrix() {

	/* Create main matrices */

	glm::mat4 mr = glm::rotate    (glm::mat4 (1.0f), rotation, glm::vec3 (0, 0, 1));
	glm::mat4 mt = glm::translate (glm::mat4 (1.0f), glm::vec3 (position, 0.0f));
	glm::mat4 ms = glm::scale     (glm::mat4 (1.0f), glm::vec3 (scale, 1.0f));

	/* Calculate new matrices */

	modelMatrix  = mt*mr*ms;
	MVPMatrix    = modelMatrix;
	MVPMatrix    = camera->MVPMatrix*modelMatrix;

	/* Matrices is updated */

	needUpdate   = false;

}
