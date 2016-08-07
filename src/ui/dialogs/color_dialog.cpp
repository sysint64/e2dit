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

/**
 * \brief
 */

void ui::ColorDialog::onCreate() {

	dialog = dynamic_cast<UIDialog*>(manager->findElement("colorDialog"));
	colorPicker = manager->findElement ("colorPicker");
	colorLine   = manager->findElement ("colorLine");

}

/**
 * \brief
 */

void ui::ColorDialog::render() {

	int x = dialog->left + colorPicker->left;
	int y = dialog->top  + colorPicker->top;

	int w = colorPicker->width;
	int h = colorPicker->height;

	// Picker render
	colorPickerShader->bind();

	glUniformMatrix4fv (colorPickerShader->locations["MVP"], 1,
	                    GL_FALSE, &(quadElement->MVPMatrix[0][0]));

	transformElement (x, y, w, h, quadElement.get());
	quadElement->render();

	// Line render

	x = dialog->left + colorLine->left;
	y = dialog->top  + colorLine->top;

	w = colorLine->width;
	h = colorLine->height;

	colorLineShader->bind();

	glUniformMatrix4fv (colorLineShader->locations["MVP"], 1, GL_FALSE,
	                    &(lineElement->MVPMatrix[0][0]));

	transformElement (x, y, w, h, lineElement.get());
	lineElement->render();

	manager->atlasShader->bind();

}
