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
 * Author: Kabylin Andrey <andrey@kabylin.ru
 */

#include "ui/listmenu.h"

void UIListMenu::precompute() {

	/* Top */

	precomputeElement (0, "listmenutop", "left");
	precomputeElement (1, "listmenutop", "center");
	precomputeElement (2, "listmenutop", "right");

	/* Middle */

	precomputeElement (3, "listmenumiddle", "left");
	precomputeElement (4, "listmenumiddle", "center");
	precomputeElement (5, "listmenumiddle", "right");

	/* Bottom */

	precomputeElement (6, "listmenubottom", "left");
	precomputeElement (7, "listmenubottom", "center");
	precomputeElement (8, "listmenubottom", "right");

	/* Mark */

	precomputeElement (9 , "listitemleave", "mark");
	precomputeElement (10, "listitementer", "mark");

	/* Split */

	precomputeElement (11, "general", "menusplit");

}
