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

#include "renderer/color.h"
#include "utility/math.h"

void Color::RGB2HSB() {
	float rgb_max = math::max (RGB.R, RGB.G, RGB.B);
	float rgb_min = math::min (RGB.R, RGB.G, RGB.B);

	if (math::feq<float>(rgb_max, rgb_min))
		HSB.H = 0.f;

	else if (math::feq<float>(rgb_max, RGB.R) && RGB.G >= RGB.B)
		HSB.H = 60.f * (RGB.G - RGB.B) / (rgb_max - rgb_min);

	else if (math::feq<float>(rgb_max, RGB.R) && RGB.G <  RGB.B)
		HSB.H = 60.f * (RGB.G-  RGB.B) / (rgb_max - rgb_min) + 360.f;

	else if (math::feq<float>(rgb_max, RGB.G))
		HSB.H = 60.f * (RGB.B - RGB.R) / (rgb_max - rgb_min) + 120.f;

	else if (math::feq<float>(rgb_max, RGB.B))
		HSB.H = 60.f * (RGB.R - RGB.G) / (rgb_max - rgb_min) + 240.f;

	HSB.B = rgb_max*100.f;
}

void Color::HSB2RGB() {
	float Hi = HSB.H/60.f;
	float Bmin = ((100.f-HSB.S)*HSB.B)/100.f;
	float a = (HSB.B-Bmin)*((int(HSB.H) % 60)/60.f);
	float Binc = Bmin+a;
	float Bdec = HSB.B-a;

	float B = HSB.B / 100.f;
	Binc /= 100.f; Bmin /= 100.f; Bdec /= 100.f;

	switch (int(Hi)) {
		case 0 : HSB = { B   , Binc, Bmin }; break;
		case 1 : HSB = { Bdec, B   , Bmin }; break;
		case 2 : HSB = { Bmin, B   , Binc }; break;
		case 3 : HSB = { Bmin, Bdec, B    }; break;
		case 4 : HSB = { Binc, Bmin, B    }; break;
		case 5 : HSB = { B   , Bmin, Bdec }; break;
		default: HSB = { 0.f , 0.f , 0.f  };
	}
}
