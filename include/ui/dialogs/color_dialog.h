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
#include "ui/edit.h"
#include "ui/grouped.h"
#include "ui/colorpanel.h"
#include "ui/manager.h"
#include "ui/loader.h"
#include "ui/dialog.h"

#include "renderer/shader.h"
#include "renderer/data_render.h"
#include "renderer/base_object.h"
#include "renderer/color.h"

#include "resources.h"

namespace ui {
	class ColorDialog {
	private:
		Application *app = Application::getInstance();

		std::unique_ptr<BaseObject> quadElement;
		std::unique_ptr<BaseObject> lineElement;
		std::unique_ptr<UILoader>   loader;

		UIManager *manager;

		enum {
			HSB_H = 0, HSB_S = 1, HSB_B = 2,
			RGB_R = 3, RGB_G = 4, RGB_B = 5
		} typedef Palette;

		Palette colorPalette = HSB_H;

		Shader *colorPickerShader;
		Shader *colorLineShader;

		// UI Elements in dialog

		UIDialog  *dialog;

		UIElement *colorPicker;
		UIElement *colorLine;
		UIElement *cursorPicker;
		UIElement *cursorLine;

		UIGrouped *groupedHSB;
		UIGrouped *groupedRGB;

		UIButton  *buttonHSB_RGB[6];
		UIEdit    *fieldHSB_RGB[6];
		UIEdit    *fieldAlpha;
		UIEdit    *fieldHEX;

		UIColorPanel *newColor;
		UIColorPanel *oldColor;

		// States

		bool colorPickerClick = false;
		bool colorPickerEnter = false;
		bool colorLineClick   = false;
		bool colorLineEnter   = false;

		bool insidePicker = false;

		int lastCursorPickerLeft;
		int lastCursorPickerTop;

		// Cursor scrolls

		glm::vec3 scroll;
		glm::vec3 norm = glm::vec3(100.f, 100.f, 360.f);

		void handleCursors();
		void cursor2ColorXY();
		void cursor2ColorZ();
		void updateUI();
		void fieldsToCursor();
		void bindEvents();

		void setCursorZ  (const float z);
		void setCursorXY (const float x, const float y);

	public:
		gapi::Color color;

		ColorDialog (UIManager *manager, StringRes *stringsRes, const std::string &layout) {

			this->manager = manager;
			loader = std::make_unique<UILoader> (manager, stringsRes, layout);

			quadElement = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
			lineElement = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());

			colorPickerShader = R::Shaders::colorPickerShader.get();
			colorLineShader   = R::Shaders::colorLineShader  .get();

			onCreate();

		}

		inline void transformElement (int x, int y, int w, int h, BaseObject *el) const {

			el->setPosition (glm::vec2(x, app->windowHeight-h-y));
			el->setScale    (glm::vec2(w, h));
			el->updateModelMatrix();

		}

		void onCreate();
		void render();

		void mouseDown (int x, int y, int button);
		void mouseUp   (int x, int y, int button);
	};
}
