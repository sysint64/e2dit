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

#include "utility/renderer.h"

#include "ui/element.h"
#include "ui/manager.h"
#include "ui/loader.h"
#include "ui/dialog.h"

#include "renderer/shader.h"
#include "renderer/data_render.h"
#include "renderer/base_object.h"
#include "renderer/color.h"

#include "resources.h"

// Color Palette Code
// HSV 0: H; 1: S; 2: B;
// RGB 3: R; 4: G; 5: B;
// Lab 6: L; 7: a; 8: b;

namespace ui {
	class ColorDialog : UIElement {
	private:
		Application *app = Application::getInstance();

		std::unique_ptr<BaseObject> quadElement;
		std::unique_ptr<BaseObject> lineElement;
		std::unique_ptr<UILoader>   loader;

		char ColorPallete;
		Shader *colorPickerShader;
		Shader *colorLineShader;

		// UI Elements in dialog

		UIDialog  *dialog;
		UIElement *colorPicker;
		UIElement *colorLine;

	public:
		gapi::Color color;

		ColorDialog (UIManager *manager, StringRes *stringsRes, const std::string &layout) : UIElement (manager) {

			loader = std::make_unique<UILoader> (manager, stringsRes, layout);

			quadElement = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
			lineElement = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());

			colorPickerShader = R::Shaders::colorPickerShader.get();
			colorLineShader   = R::Shaders::colorLineShader  .get();

			onCreate();

		}

		void onCreate();
		virtual void render() override;

	};
}
