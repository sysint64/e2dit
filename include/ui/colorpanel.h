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

#include "ui/element.h"
#include "ui/manager.h"

#include "renderer/base_object.h"
#include "renderer/texture.h"
#include "renderer/color.h"

namespace ui {
	class UIColorPanel : public UIElement {
	private:
		std::unique_ptr<gapi::BaseObject> quadElement = std::make_unique<gapi::BaseObject> (manager->uiDataRender, app->screenCamera.get());
		gapi::Texture *gridTexture = nullptr;

	public:
		gapi::Color color;

		virtual void render();
		virtual void mouseUp (int x, int y, int button) override;

		UIColorPanel (UIManager *manager, gapi::Texture *gridTexture = nullptr) : UIElement(manager) {

			this->manager     = manager;
			this->gridTexture = gridTexture;

			offsetsX[0] = 0.f; offsetsY[0] = 0.f;
			fWidths [0] = 1.f; fHeights[0] = 1.f;

		}

		static std::unique_ptr<UIColorPanel> createInstance(UIManager *manager,
		                                                    int w, int h, int x, int y,
		                                                    float R, float G, float B, float A)
		{
			std::unique_ptr<UIColorPanel> instance = std::make_unique<UIColorPanel> (manager);

			instance->width  = w;
			instance->height = h;
			instance->left   = x;
			instance->top    = y;
			instance->color.RGB.set(R, G, B);
			instance->color.alpha = A;

			return instance;
		}

		inline  void update() {

			if (!gridTexture)
				return;

			float tfw = static_cast<float>(gridTexture->width );
			float tfh = static_cast<float>(gridTexture->height);

			fWidths [0] = static_cast<float>(width ) / tfw;
			fHeights[0] = static_cast<float>(height) / tfh;

		}

	};
};
