/*
 * E2DIT - is a 2D Map Editor to create a levels for 2d games
 * Copyright (C) 2015 Kabylin Andrey <andrey@kabylin.ru>

 * This file is part of E2DIT-UI.

 * E2DIT-UI is free library: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.

 * E2DIT-UI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with E2DIT-UI. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Kabylin Andrey <andrey@kabylin.ru>
 */

#pragma once

namespace gapi {

	class Color {
	private:
		float _data[4];

	public:
		struct { float R, G, B; } RGB;
		struct { float H, S, B; } HSB;
		struct { float X, Y, Z; } XYZ;
		struct { float L, a, b; } Lab;

		float alpha = 1.f;
		int hex;

		void RGB2HSB();
		void HSB2RGB();
		inline void normRGB(int norm) {
			RGB.R *= norm;
			RGB.G *= norm;
			RGB.B *= norm;
		}

		static inline Color createFromRGBA (float R, float G, float B, float alpha) {
			Color color;
			color.RGB.R = R; color.RGB.G = G; color.RGB.B = B;
			color.alpha = alpha;
			return color;
		}

		static inline Color createFromRGB (float R, float G, float B) {
			return createFromRGBA (R, G, B, 1.f);
		}

		static inline Color createFromHSBA (float H, float S, float B, float alpha) {
			Color color;
			color.HSB.H = H; color.HSB.S = S; color.HSB.B = B;
			color.alpha = alpha;
			return color;
		}

		static inline Color createFromHSB (float H, float S, float B) {
			return createFromHSBA (H, S, B, 1.f);
		}

		inline float *data() {
			_data[0] = RGB.R;
			_data[1] = RGB.G;
			_data[2] = RGB.B;
			_data[3] = alpha;

			return _data;
		};
	};

}
