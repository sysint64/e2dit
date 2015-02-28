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

#include "renderer/camera.h"

/**
 * Constructor
 *
 * @param width
 * @param height
 */

Camera::Camera (int width, int height) : viewWidth (width), viewHeight (height) {

	/* Set Base params */

	setPosition (glm::vec2(0, 0));
	zoom = 1.f;

}

/**
 * Update Camera Position
 * @param vec New Position
 */

void Camera::setPosition (glm::vec2 vec) {

	position   = vec;
	needUpdate = true;

}

/**
 * Update Camera Zoom
 * @param zoom New Zoom
 */

void Camera::setZoom (float zoom) {

	this->zoom = zoom;
	needUpdate = true;

}

/**
 * Update Camera Matrices
 */

void Camera::updateMatrices() {

	/* Create base Matrices */

	viewMatrix       = glm::lookAt (glm::vec3 (position, 1), glm::vec3 (position, 0), glm::vec3(0, 1, 0));
	projectionMatrix = glm::ortho  (0.0f, (float) viewWidth, 0.0f, (float) viewHeight/*, -1.0f, 10.0f*/);

	/* If Zoom greater then normal */

	if (zoom > 1.f) {

		/* Added scale matrix ti MVP */

		glm::mat4 sm = glm::scale  (glm::mat4 (1.0f), glm::vec3 (zoom, zoom, 1.0f));
		MVPMatrix    = projectionMatrix*sm*viewMatrix; // Result Matrix

	} else {

		/* Else only projectionMatrix & viewMatrix */

		MVPMatrix    = projectionMatrix*viewMatrix;    // Result Matrix

	}

}

/**
 * Update Camera
 */

void Camera::update() {

	/* If matrices was changes, then update it */

	if (needUpdate)
		updateMatrices();

}
