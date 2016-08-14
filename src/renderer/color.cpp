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

void gapi::Color::RGB2HSB() {
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

	if (math::feq<float>(rgb_max, 0.f)) HSB.S = 0.f;
	else HSB.S = (1.f - rgb_min / rgb_max) * 100.f;

	HSB.B = rgb_max*100.f;
}

void gapi::Color::HSB2RGB() {

	if (HSB.H >= 360.f-std::numeric_limits<float>::epsilon())
		HSB.H = 0;

	float Hi = HSB.H/60.f;
	float Bmin = ((100.f-HSB.S)*HSB.B)/100.f;
	float a = (HSB.B-Bmin)*((static_cast<int>(HSB.H) % 60)/60.f);
	float Binc = Bmin+a;
	float Bdec = HSB.B-a;

	float B = HSB.B / 100.f;
	Binc /= 100.f; Bmin /= 100.f; Bdec /= 100.f;

	switch (int(Hi)) {
		case 0 : RGB = { B   , Binc, Bmin }; break;
		case 1 : RGB = { Bdec, B   , Bmin }; break;
		case 2 : RGB = { Bmin, B   , Binc }; break;
		case 3 : RGB = { Bmin, Bdec, B    }; break;
		case 4 : RGB = { Binc, Bmin, B    }; break;
		case 5 : RGB = { B   , Bmin, Bdec }; break;
		default: RGB = { 0.f , 0.f , 0.f  };
	}
}
