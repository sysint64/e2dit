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

#include <ui/all.h>
#include "ui/dialogs/color_dialog.h"

void ui::ColorDialog::onCreate() {

	dialog = dynamic_cast<UIDialog*>(manager->findElement("colorDialog"));

	colorPicker  = manager->findElement ("colorPicker");
	colorLine    = manager->findElement ("colorLine");
	cursorPicker = manager->findElement ("cursorPicker");
	cursorLine   = manager->findElement ("cursorLine");

	// Text fields

	fieldHSB_RGB[HSB_H]  =       (UIEdit*) manager->findElement ("eHSB_H");
	fieldHSB_RGB[HSB_S]  =       (UIEdit*) manager->findElement ("eHSB_S");
	fieldHSB_RGB[HSB_B]  =       (UIEdit*) manager->findElement ("eHSB_B");

	fieldHSB_RGB[RGB_R]  =       (UIEdit*) manager->findElement ("eRGB_R");
	fieldHSB_RGB[RGB_G]  =       (UIEdit*) manager->findElement ("eRGB_G");
	fieldHSB_RGB[RGB_B]  =       (UIEdit*) manager->findElement ("eRGB_B");

	// Buttons

	buttonHSB_RGB[HSB_H] =     (UIButton*) manager->findElement ("bHSB_H");
	buttonHSB_RGB[HSB_S] =     (UIButton*) manager->findElement ("bHSB_S");
	buttonHSB_RGB[HSB_B] =     (UIButton*) manager->findElement ("bHSB_B");

	buttonHSB_RGB[RGB_R] =     (UIButton*) manager->findElement ("bRGB_R");
	buttonHSB_RGB[RGB_G] =     (UIButton*) manager->findElement ("bRGB_G");
	buttonHSB_RGB[RGB_B] =     (UIButton*) manager->findElement ("bRGB_B");

	fieldAlpha           =       (UIEdit*) manager->findElement ("alpha");
	fieldHEX             =       (UIEdit*) manager->findElement ("HEX");

	// Grouped

	groupedHSB           =    (UIGrouped*) manager->findElement ("gHSB");
	groupedRGB           =    (UIGrouped*) manager->findElement ("gRGB");

	// Color

	newColor             = (UIColorPanel*) manager->findElement ("newColor");
	oldColor             = (UIColorPanel*) manager->findElement ("oldColor");

	// Settings

	cursorPicker->visible = false;
	cursorLine  ->visible = false;

	lastCursorPickerLeft = cursorPicker->left;
	lastCursorPickerTop  = cursorPicker->top;

	bindEvents();

}

void ui::ColorDialog::updateUI() {

	auto HSB = &color.HSB;
	auto RGB = &color.RGB;

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

		updateUI();
		fieldsToCursor();
	};

	fieldHSB_RGB[HSB_S]->onChange = [this](UIElement* el) {
		color.HSB.S = std::stoi(fieldHSB_RGB[HSB_S]->text);
		color.HSB2RGB();

		updateUI();
		fieldsToCursor();
	};

	fieldHSB_RGB[HSB_B]->onChange = [this](UIElement* el) {
		color.HSB.B = std::stoi(fieldHSB_RGB[HSB_B]->text);
		color.HSB2RGB();

		updateUI();
		fieldsToCursor();
	};

	// RGB

	fieldHSB_RGB[RGB_R]->onChange = [this](UIElement* el) {
		color.RGB.R = std::stoi(fieldHSB_RGB[RGB_R]->text) / 255.f;
		color.RGB2HSB();

		updateUI();
		fieldsToCursor();
	};

	fieldHSB_RGB[RGB_G]->onChange = [this](UIElement* el) {
		color.RGB.G = std::stoi(fieldHSB_RGB[RGB_G]->text) / 255.f;
		color.RGB2HSB();

		updateUI();
		fieldsToCursor();
	};

	fieldHSB_RGB[RGB_B]->onChange = [this](UIElement* el) {
		color.RGB.B = std::stoi(fieldHSB_RGB[RGB_B]->text) / 255.f;
		color.RGB2HSB();

		updateUI();
		fieldsToCursor();
	};

	// HEX

	fieldHEX->onChange = [this](UIElement* el) {
		if (HEX2RGB (fieldHEX->text)) {
			updateUI();
			fieldsToCursor();
		} else {
			//fieldHEX->text = lastHEX;
		}
	};

	// Alpha

	fieldAlpha->onChange = [this](UIElement* el) {
		color.alpha = std::stoi(fieldAlpha->text) / 100.f;
		newColor->color = color;
	};

	// Buttons
	// HSB

	buttonHSB_RGB[HSB_H]->onClick = [this](UIElement* el) {
		colorPalette = HSB_H;
		groupedRGB->uncheck();
		norm = glm::vec3(100.f, 100.f, 360.f);
		fieldsToCursor();
	};

	buttonHSB_RGB[HSB_S]->onClick = [this](UIElement* el) {
		colorPalette = HSB_S;
		groupedRGB->uncheck();
		norm = glm::vec3(360.f, 100.f, 100.f);
		fieldsToCursor();
	};

	buttonHSB_RGB[HSB_B]->onClick = [this](UIElement* el) {
		colorPalette = HSB_B;
		groupedRGB->uncheck();
		norm = glm::vec3(360.f, 100.f, 100.f);
		fieldsToCursor();
	};

	// RGB

	buttonHSB_RGB[RGB_R]->onClick = [this](UIElement* el) {
		colorPalette = RGB_R;
		groupedHSB->uncheck();
		norm = glm::vec3(1.f, 1.f, 1.f);
		fieldsToCursor();
	};

	buttonHSB_RGB[RGB_G]->onClick = [this](UIElement* el) {
		colorPalette = RGB_G;
		groupedHSB->uncheck();
		norm = glm::vec3(1.f, 1.f, 1.f);
		fieldsToCursor();
	};

	buttonHSB_RGB[RGB_B]->onClick = [this](UIElement* el) {
		colorPalette = RGB_B;
		groupedHSB->uncheck();
		norm = glm::vec3(1.f, 1.f, 1.f);
		fieldsToCursor();
	};
}
