/*
 * E2DIT - is a 2D Map Editor to create a levels for 2d games
 * Copyright (C) 2015 Kabylin Andrey <andrey@kabylin.ru>

 * This file is part of E2DIT.

 * E2DIT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * E2DIT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with E2DIT.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Kabylin Andrey <andrey@kabylin.ru>
 */

/** @file utils.h
    @brief Some added function for work with math
 */

#ifndef E2DIT_MATH_UTILS_H
#define E2DIT_MATH_UTILS_H

#include <math.h>

/** Compare two float values with laps = eps
    @param a LHS value
    @param b RHS value
    @param eps laps
    @return true if a == b with laps = eps
 */

template <typename T>
inline bool feq (const T a, const T b, const float eps = std::numeric_limits<T>::epsilon()) {

	return  a >= b-eps && a <= b+eps;

}

/** Rotate point a2 relative a1 around on angle
    @param a1 pivot
    @param a2 rotated point
    @param angle
    @return new rotated vector
 */

inline glm::vec2 rot2D (glm::vec2 &a1, glm::vec2 &a2, float angle) {

	float x = a1.x + (a2.x-a1.x) * cos (angle) + (a2.y-a1.y) * sin (angle);
	float y = a1.y - (a2.x-a1.x) * sin (angle) + (a2.y-a1.y) * cos (angle);

	return glm::vec2 (x, y);
}

/** Degrees To Radians
    @deg Degrees
    @return Radians
 */

inline float deg2rad (float deg) {

	return M_PI*deg / 180.0f;

}

#endif
