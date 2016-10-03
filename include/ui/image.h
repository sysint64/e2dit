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

#include "ui/element.h"
#include "ui/manager.h"

#include "renderer/base_object.h"
#include "renderer/texture.h"

#include <string>

namespace ui {
	class UIImage : public UIElement {
	private:
		std::unique_ptr<gapi::BaseObject> imageElement = std::make_unique<gapi::BaseObject> (manager->uiDataRender, app->screenCamera.get());

	public:
		std::unique_ptr<gapi::Texture> texture;

		void loadImage (const std::string &fileName);
		void cropImage (int ox, int oy, int cw, int ch);
		void cropImage (const std::array<int, 4> &cropRect);

		UIImage (UIManager *manager) : UIElement(manager) {

			this->manager = manager;

			offsetsX[0] = 0.f; offsetsY[0] = 0.f;
			fWidths [0] = 1.f; fHeights[0] = 1.f;

		}

		virtual void render();

	};
};
