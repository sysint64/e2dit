/*
 * E2DIT - is a 2D Map Editor to create a levels for 2d games
 * Copyright (C) 2015 Kabylin Andrey <andrey@kabylin.ru>

 * This file is part of E2DIT-UTILITY.

 * E2DIT-UTILITY is free library: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * E2DIT-UTILITY is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with E2DIT-UTILITY.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Kabylin Andrey <andrey@kabylin.ru>
 */

#ifndef E2DIT_UTILITY_MATH_H
#define E2DIT_UTILITY_MATH_H

#include <math.h>
#include <glm/glm.hpp>

/**
 * Min, Max
 */

template <typename T>
inline T min (T a, T b) {
	return a < b ? a : b;
}

template <typename T>
inline T max (T a, T b) {
	return a > b ? a : b;
}

template <typename T>
inline T min (T a, T b, T c) {
	T m = b;

	if (a < m) m = a;
	if (c < m) m = c;

	return m;
}

template <typename T>
inline T max (T a, T b, T c) {
	T m = b;

	if (a > m) m = a;
	if (c > m) m = c;

	return m;
}

inline glm::vec2 min (glm::vec2 a, glm::vec2 b) {
	return glm::vec2(min (a.x, b.x), min (a.y, b.y));
}

inline glm::vec2 max (glm::vec2 a, glm::vec2 b) {
	return glm::vec2(max (a.x, b.x), max (a.y, b.y));
}

/** Compare two float values with laps = eps
 *
 *  @param a LHS value
 *  @param b RHS value
 *  @param eps laps
 *  @return true if a == b with laps = eps
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

#define countof(a) ((sizeof(a) / sizeof(*(a))) / static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

#endif
