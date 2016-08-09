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
#include "utility/ui.h"

/**
 * \brief
 */

void ui::ColorDialog::onCreate() {

	dialog = dynamic_cast<UIDialog*>(manager->findElement("colorDialog"));

	colorPicker  = manager->findElement ("colorPicker");
	colorLine    = manager->findElement ("colorLine");
	cursorPicker = manager->findElement ("cursorPicker");
	cursorLine   = manager->findElement ("cursorLine");

	cursorPicker->visible = false;
	cursorLine  ->visible = false;

	lastCursorPickerLeft = cursorPicker->left;
	lastCursorPickerTop  = cursorPicker->top;

}

/**
 * \brief
 */

void ui::ColorDialog::render() {

	int x, y, w, h;

	// Picker render

	x = colorPicker->absLeft; y = colorPicker->absTop;
	w = colorPicker->width;   h = colorPicker->height;

	colorPickerShader->bind();

	transformElement (x, y, w, h, quadElement.get());
	glUniformMatrix4fv (colorPickerShader->locations["MVP"], 1,
	                    GL_FALSE, &(quadElement->MVPMatrix[0][0]));

	quadElement->render();

	// Line render

	x = colorLine->absLeft; y = colorLine->absTop;
	w = colorLine->width;   h = colorLine->height;

	colorLineShader->bind();

	transformElement (x, y, w, h, lineElement.get());
	glUniformMatrix4fv (colorLineShader->locations["MVP"], 1, GL_FALSE,
	                    &(lineElement->MVPMatrix[0][0]));

	lineElement->render();

	manager->atlasShader->bind();
	poll();

}

/**
 * \brief
 */

void ui::ColorDialog::poll() {

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
