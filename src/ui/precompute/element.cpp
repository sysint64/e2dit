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

#include "ui/element.h"
#include "ui/manager.h"

#include "utility/exceptions.h"

void UIElement::precomputeException (const std::string &element, const std::string &params, const int size) const {

	if (manager->theme->element.count(element) == 0)
		throw std::runtime_error (Formatter() << "Can't find element \"" << element << "\"");

	if (manager->theme->element[element].params.count(params) == 0)
		throw std::runtime_error (Formatter() << "Can't find param \"" << params << "\" in \"" << element << "\"");

	if (manager->theme->element[element].params[params].size() < size)
		throw std::runtime_error (Formatter() << element << "[" << params << "] size less than " << size << " ("
		                                      << manager->theme->element[element].params[params].size() << ")");

}

/**
 *
 */

void UIElement::precomputeElement (const int n, const std::string &element, const std::string &params) {

	precomputeException (element, params, 4);

	offsetsX[n] = manager->theme->element[element].params[params][0].num / manager->theme->skin->width;
	offsetsY[n] = manager->theme->element[element].params[params][1].num / manager->theme->skin->height;
	fWidths [n] = manager->theme->element[element].params[params][2].num / manager->theme->skin->width;
	fHeights[n] = manager->theme->element[element].params[params][3].num / manager->theme->skin->height;

	iOffsetsX[n] = floor (manager->theme->element[element].params[params][0].num);
	iOffsetsY[n] = floor (manager->theme->element[element].params[params][1].num);
	iWidths  [n] = floor (manager->theme->element[element].params[params][2].num);
	iHeights [n] = floor (manager->theme->element[element].params[params][3].num);

}

/**
 *
 */

void UIElement::precomputeText (const int n, const std::string &element) {

	int tcs = n*3; /* Text Color  Index Start */
	int tos = n*2; /* Text Offset Index Start */

	/* Text Color */

	precomputeException (element, "textcolor", 3);

	textColors [tcs+0] = manager->theme->element[element].params["textcolor"][0].num / 255.f;
	textColors [tcs+1] = manager->theme->element[element].params["textcolor"][1].num / 255.f;
	textColors [tcs+2] = manager->theme->element[element].params["textcolor"][2].num / 255.f;

	/* Text Offset */

	precomputeException (element, "textoffset", 2);

	textOffsets[tos+0] = floor (manager->theme->element[element].params["textoffset"][0].num);
	textOffsets[tos+1] = floor (manager->theme->element[element].params["textoffset"][1].num);

}

/**
 *
 */

void UIElement::precomputeColor3f (const std::string &element, const std::string &params, float *arr) {

	precomputeException (element, params, 3);

	arr[0] = manager->theme->element[element].params[params][0].num / 255.f;
	arr[1] = manager->theme->element[element].params[params][1].num / 255.f;
	arr[2] = manager->theme->element[element].params[params][2].num / 255.f;

}

/**
 *
 */

void UIElement::precomputeColor4f (const std::string &element, const std::string &params, float *arr) {

	precomputeException (element, params, 4);

	arr[0] = manager->theme->element[element].params[params][0].num / 255.f;
	arr[1] = manager->theme->element[element].params[params][1].num / 255.f;
	arr[2] = manager->theme->element[element].params[params][2].num / 255.f;
	arr[3] = manager->theme->element[element].params[params][3].num / 100.f;

}

/**
 *
 */

void UIElement::precomputeFloatArray (const std::string &element, const std::string &params, float *arr, const int size, float normalize) {

	precomputeException (element, params, size);

	for (int i = 0; i < size; i++)
		arr[i] = manager->theme->element[element].params[params][i].num / normalize;

}

/**
 *
 */

void UIElement::precomputeIntArray (const std::string &element, const std::string &params, int *arr, const int size) {

	precomputeException (element, params, size);

	for (int i = 0; i < size; i++)
		arr[i] = manager->theme->element[element].params[params][i].num;

}

int UIElement::precomputeInt (const std::string &element, const std::string &param) {

	return manager->theme->element[element].params[param][0].intval;

}

float UIElement::precomputeFloat (const std::string &element, const std::string &param) {

	return manager->theme->element[element].params[param][0].num;

}
