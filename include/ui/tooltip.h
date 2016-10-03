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

#pragma once

#include "ui/widget.h"
#include "ui/manager.h"

namespace ui {
	class Tooltip : public Widget {
	protected:

		std::unique_ptr<gapi::BaseObject> leftElement   = std::make_unique<gapi::BaseObject> (manager->uiDataRender, app->screenCamera.get());
		std::unique_ptr<gapi::BaseObject> rightElement  = std::make_unique<gapi::BaseObject> (manager->uiDataRender, app->screenCamera.get());
		std::unique_ptr<gapi::BaseObject> middleElement = std::make_unique<gapi::BaseObject> (manager->uiDataRender, app->screenCamera.get());

		float alpha = 0.f;
		std::string leftParam  = "left";
		std::string rightParam = "right";

		int leftArea  [4]; int rightArea  [4];
		int topArea   [4]; int bottomtArea[4];
		int leftOffset[2]; int rightOffset[2];
		float time = 0;
		bool hidden = true;

	public:

		Widget *target;

		virtual void precompute() override;
		virtual void render()     override;

		inline void setHidden (const bool value) {

			if (hidden == value)
				return;

			time = 0.f;
			hidden = value;

		}

		Tooltip (Manager *manager) : Widget (manager) {

			align = Align::Left;
			this->manager = manager;
			style = "tooltip";
			precompute();

		}

	};
};
