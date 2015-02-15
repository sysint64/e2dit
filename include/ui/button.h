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

#ifndef E2DIT_UI_BUTTON_H
#define E2DIT_UI_BUTTON_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "utility/config.h"

#include "ui/element.h"
#include "ui/manager.h"

#include "renderer/shader.h"
#include "renderer/data_render.h"
#include "renderer/base_object.h"

class UIButton : public UIElement {
protected:

    /* Precomputed tables */

    int   iWidths [12];    int   iHeights[12];
	float fWidths [12];    float fHeights[12];
	float offsetsX[12];    float offsetsY[12];

    float textColors [12];
	float textOffsets[16];

    /* Param Names for load from Layout file */

    std::string leavParam  = "buttonleav";
	std::string enterParam = "buttonenter";
	std::string clickParam = "buttonclick";

    /* Render Objects */

    BaseObject	*leftElement;
	BaseObject	*rightElement;
	BaseObject	*middleElement;
	BaseObject	*iconElement;
	BaseObject	*iconElement2;

    /* Draw Params */

    bool NoDrawText;
	int  IcoOffset;

    /* Draw Text */

    void DrawText (int Align, std::string Text, int Size, int Offset = 0);

public:

    std::wstring caption;

    /* */

	~UIButton();

    virtual void precompute();
	virtual void draw (int x, int y);

};

#endif
