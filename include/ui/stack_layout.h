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

#pragma once

#include "ui/widget.h"
#include "ui/manager.h"

namespace ui {

	enum class Orientation {Horizontal, Vertical};
	class StackLayout : public Widget {
	public:
		Orientation orientation = Orientation::Vertical;

		virtual void render() override;
		virtual void addElement    (std::unique_ptr<Widget> el) override;
		//virtual void deleteElement (std::unique_ptr<Widget> el) override;
		//virtual void deleteElement (const int id) override;

		StackLayout (Manager *manager) : Widget (manager) {
			this->manager = manager;
		}

	};

};
