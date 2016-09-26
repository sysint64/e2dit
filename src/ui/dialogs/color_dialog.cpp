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

void ui::ColorDialog::render() {

	if (!dialog->visible)
		return;

	int x, y, w, h;

	auto HSB = &color.HSB;
	auto RGB = &color.RGB;

	// Picker render

	x = colorPicker->absLeft;  y = colorPicker->absTop;
	w = colorPicker->width;    h = colorPicker->height;

	colorPickerShader->bind();

	transformElement (x, y, w, h, quadElement.get());

	glUniform1i        (colorPickerShader->locations["palette"], colorPalette);
	glUniform3f        (colorPickerShader->locations["HSB"]    , HSB->H, HSB->S, HSB->B);
	glUniform3f        (colorPickerShader->locations["RGB"]    , RGB->R, RGB->G, RGB->B);
	glUniformMatrix4fv (colorPickerShader->locations["MVP"]    , 1,
	                    GL_FALSE, &(quadElement->MVPMatrix[0][0]));

	quadElement->render();

	// Line render

	x = colorLine->absLeft;  y = colorLine->absTop;
	w = colorLine->width;    h = colorLine->height;

	colorLineShader->bind();

	transformElement (x, y, w, h, lineElement.get());

	glUniform1i        (colorLineShader->locations["palette"], colorPalette);
	glUniform3f        (colorLineShader->locations["HSB"]    , HSB->H, HSB->S, HSB->B);
	glUniform3f        (colorLineShader->locations["RGB"]    , RGB->R, RGB->G, RGB->B);
	glUniformMatrix4fv (colorLineShader->locations["MVP"]    , 1, GL_FALSE,
	                    &(lineElement->MVPMatrix[0][0]));

	lineElement->render();

	manager->atlasShader->bind();
	handleCursors();

}

void ui::ColorDialog::handleCursors() {

	auto pickerBound = colorPicker->getScreenBound();

	colorPickerEnter = pointInElement (app->mouseX, app->mouseY, colorPicker);
	colorLineEnter   = pointInElement (app->mouseX, app->mouseY, colorLine);

	if (!colorPickerClick) {
		glEnable  (GL_SCISSOR_TEST);
		glScissor (pickerBound[0], pickerBound[1], pickerBound[2], pickerBound[3]);

		cursorPicker->left = lastCursorPickerLeft;
		cursorPicker->top  = lastCursorPickerTop;
		cursorPicker->render();

		glDisable (GL_SCISSOR_TEST);
	}

	if ((colorPickerEnter || colorPickerClick) && !colorLineClick) {

		app->cursorVisible = false;
		insidePicker = true;

		int halfWidth  = (cursorPicker->width  >> 1);
		int halfHeight = (cursorPicker->height >> 1);

		cursorPicker->left = app->mouseX - halfWidth  - dialog->left;
		cursorPicker->top  = app->mouseY - halfHeight - dialog->top;

		math::clamp (&cursorPicker->left,  colorPicker->left  - halfWidth,
		              colorPicker ->left + colorPicker->width - halfWidth);

		math::clamp (&cursorPicker->top,  colorPicker->top    - halfHeight,
		              colorPicker ->top + colorPicker->height - halfHeight);

		if (colorPickerClick) {
			lastCursorPickerLeft = cursorPicker->left;
			lastCursorPickerTop  = cursorPicker->top;

			cursorPicker->updateAbsPos();

			scroll.x = static_cast<float>(cursorPicker->left - colorPicker->left + halfWidth) /
			           static_cast<float>(colorPicker ->width);

			scroll.y = static_cast<float>(cursorPicker->top  - colorPicker->top  + halfHeight) /
			           static_cast<float>(colorPicker ->height);

			scroll.y = 1.f - scroll.y;
			cursor2ColorXY();
			sf::Mouse::setPosition(sf::Vector2i(cursorPicker->absLeft + halfWidth,
			                                    cursorPicker->absTop  + halfHeight),
			                       *(manager->window));
		}

		cursorPicker->render();

	} else if (insidePicker) {

		insidePicker = false;
		app->cursorVisible = true;

	}

	// Color Line

	cursorLine->render();

	if (colorLineClick) {
		int halfHeight = (cursorLine->height >> 1);
		cursorLine->top = app->mouseY - halfHeight - dialog->top;
		math::clamp (&cursorLine->top,  colorLine->top    - halfHeight,
		              colorLine ->top + colorLine->height - halfHeight);

		scroll.z = static_cast<float>(cursorLine->top - colorLine->top  + halfHeight) /
		           static_cast<float>(colorLine ->height);

		scroll.z = 1.f - scroll.z;
		cursor2ColorZ();
	}

	cursorLine->render();
}

void ui::ColorDialog::mouseDown (int x, int y, int button) {
	colorPickerClick = colorPickerEnter;
	colorLineClick   = colorLineEnter;
}

void ui::ColorDialog::mouseUp (int x, int y, int button) {
	colorPickerClick = false;
	colorLineClick   = false;
}

// Convert cursor position to color

void ui::ColorDialog::cursor2ColorXY() {

	auto HSB = &color.HSB;
	auto RGB = &color.RGB;

	switch (colorPalette) {
		case HSB_H: HSB->S = scroll.x * norm.x;  HSB->B = scroll.y * norm.y; break;
		case HSB_S: HSB->H = scroll.x * norm.x;  HSB->B = scroll.y * norm.y; break;
		case HSB_B: HSB->H = scroll.x * norm.x;  HSB->S = scroll.y * norm.y; break;

		case RGB_R: RGB->G = scroll.y * norm.y;  RGB->B = scroll.x * norm.x; break;
		case RGB_G: RGB->R = scroll.y * norm.y;  RGB->B = scroll.x * norm.x; break;
		case RGB_B: RGB->R = scroll.x * norm.x;  RGB->G = scroll.y * norm.y; break;
	}

	if (colorPalette >= RGB_R) color.RGB2HSB();
	else                       color.HSB2RGB();

	RGB2HEX();
	updateUI();

}

void ui::ColorDialog::cursor2ColorZ() {

	auto HSB = &color.HSB;
	auto RGB = &color.RGB;

	switch (colorPalette) {
		case HSB_H: HSB->H = scroll.z * norm.z; break;
		case HSB_S: HSB->S = scroll.z * norm.z; break;
		case HSB_B: HSB->B = scroll.z * norm.z; break;

		case RGB_R: RGB->R = scroll.z * norm.z; break;
		case RGB_G: RGB->G = scroll.z * norm.z; break;
		case RGB_B: RGB->B = scroll.z * norm.z; break;
	}

	if (colorPalette >= RGB_R) color.RGB2HSB();
	else                       color.HSB2RGB();

	RGB2HEX();
	updateUI();

}

void ui::ColorDialog::setCursorZ (const float z) {

	scroll.z = 1.f - z / norm.z;
	cursorLine->top = colorLine->top + static_cast<int> (scroll.z*colorLine->height);
	cursorLine->top -= cursorLine->height >> 1;

}

void ui::ColorDialog::setCursorXY (const float x, const float y) {

	scroll.x = x / norm.x;
	scroll.y = 1.f - y / norm.y;

	lastCursorPickerLeft = colorPicker->left + static_cast<int> (scroll.x*colorPicker->width );
	lastCursorPickerTop  = colorPicker->top  + static_cast<int> (scroll.y*colorPicker->height);

	lastCursorPickerLeft -= cursorPicker->width  >> 1;
	lastCursorPickerTop  -= cursorPicker->height >> 1;

}

bool ui::ColorDialog::HEX2RGB (const std::wstring &hex) {

	float RGB[3] {0, 0, 0};
	const std::wstring alphabet = L"0123456789abcdefABCDEF";

	if (hex.length() != 6)
		return false;

	for (int i = 0; i < 6; i += 2) {

		auto wc1 = hex[i];
		auto wc2 = hex[i+1];

		if (alphabet.find(wc1) == std::wstring::npos || alphabet.find(wc2) == std::wstring::npos)
			return false;

		std::wstring byte;
		byte += wc2; byte += wc1;
		RGB[i/2] = static_cast<float>(math::hexstr2int(byte)) / 255.f;

	}

	color.RGB.set(RGB);
	color.RGB2HSB();

	return true;

}

void ui::ColorDialog::RGB2HEX() {
	std::wostringstream stringStream;

	auto addColorComponent = [&stringStream](float val) {
		stringStream << std::hex << std::setw(2) << std::setfill(L'0')
		             << static_cast<int>(round(val*255.f));
	};

	addColorComponent (color.RGB.R);
	addColorComponent (color.RGB.G);
	addColorComponent (color.RGB.B);

	fieldHEX->text = stringStream.str();
}

void ui::ColorDialog::fieldsToCursor() {

	const auto HSB = &color.HSB;
	const auto RGB = &color.RGB;

	switch (colorPalette) {
		case HSB_H: setCursorZ (HSB->H);  setCursorXY (HSB->S, HSB->B); break;
		case HSB_S: setCursorZ (HSB->S);  setCursorXY (HSB->H, HSB->B); break;
		case HSB_B: setCursorZ (HSB->B);  setCursorXY (HSB->H, HSB->S); break;

		case RGB_R: setCursorZ (RGB->R);  setCursorXY (RGB->B, RGB->G); break;
		case RGB_G: setCursorZ (RGB->G);  setCursorXY (RGB->B, RGB->R); break;
		case RGB_B: setCursorZ (RGB->B);  setCursorXY (RGB->R, RGB->G); break;
	}

}
