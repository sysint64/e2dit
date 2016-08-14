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

#include "ui/dialogs/color_dialog.h"

void ui::ColorDialog::updateUI() {

	auto HSB = &color.HSB;
	auto RGB = &color.RGB;

	// color.HSB2RGB();
	newColor->color = color;

	fieldHSB_RGB[HSB_H]->text = std::to_wstring (static_cast<int>(std::round(HSB->H)));
	fieldHSB_RGB[HSB_S]->text = std::to_wstring (static_cast<int>(std::round(HSB->S)));
	fieldHSB_RGB[HSB_B]->text = std::to_wstring (static_cast<int>(std::round(HSB->B)));

	fieldHSB_RGB[RGB_R]->text = std::to_wstring (static_cast<int>(std::round(RGB->R*255.f)));
	fieldHSB_RGB[RGB_G]->text = std::to_wstring (static_cast<int>(std::round(RGB->G*255.f)));
	fieldHSB_RGB[RGB_B]->text = std::to_wstring (static_cast<int>(std::round(RGB->B*255.f)));

}

void ui::ColorDialog::bindEvents() {

	// Text Fields
	// HSB

	fieldHSB_RGB[HSB_H]->onChange = [this](UIElement* el) {
		color.HSB.H = std::stoi(fieldHSB_RGB[HSB_H]->text);
		color.HSB2RGB();

		updateUI(); fieldsToCursor();
	};

	fieldHSB_RGB[HSB_S]->onChange = [this](UIElement* el) {
		color.HSB.S = std::stoi(fieldHSB_RGB[HSB_S]->text);
		color.HSB2RGB();

		updateUI(); fieldsToCursor();
	};

	fieldHSB_RGB[HSB_B]->onChange = [this](UIElement* el) {
		color.HSB.B = std::stoi(fieldHSB_RGB[HSB_B]->text);
		color.HSB2RGB();

		updateUI(); fieldsToCursor();
	};

	// RGB

	fieldHSB_RGB[RGB_R]->onChange = [this](UIElement* el) {
		color.RGB.R = std::stoi(fieldHSB_RGB[RGB_R]->text) / 255.f;
		color.RGB2HSB();

		updateUI(); fieldsToCursor();
	};

	fieldHSB_RGB[RGB_G]->onChange = [this](UIElement* el) {
		color.RGB.G = std::stoi(fieldHSB_RGB[RGB_G]->text) / 255.f;
		color.RGB2HSB();

		updateUI(); fieldsToCursor();
	};

	fieldHSB_RGB[RGB_B]->onChange = [this](UIElement* el) {
		color.RGB.B = std::stoi(fieldHSB_RGB[RGB_B]->text) / 255.f;
		color.RGB2HSB();

		updateUI(); fieldsToCursor();
	};
}
