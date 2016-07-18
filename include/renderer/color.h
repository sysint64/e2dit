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

#pragma once

namespace gapi {

	class Color {
	public:
		struct { float R, G, B; } RGB;
		struct { float H, S, B; } HSB;
		struct { float X, Y, Z; } XYZ;
		struct { float L, a, b; } Lab;

		float alpha = 1.f;
		int hex;

		void RGB2HSB();
		void HSB2RGB();
	};

}
