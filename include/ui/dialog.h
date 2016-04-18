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
#include "ui/panel.h"

#include "renderer/base_object.h"

class UIDialog : public UIPanel {
protected:
	std::unique_ptr<BaseObject> drawElements[9];
	BaseObject *drawElementsPtr[9];
	int indices[9];

	int lastLeft = 0;
	int lastTop  = 0;

	bool  headerClick = false;
	int   captionArea[4];
	float opacity[4];

public:

	std::wstring caption = L"Hello World!";

	virtual void render()     override;
	virtual void precompute() override;

	virtual void mouseDown (int x, int y, int button) override;
	virtual void mouseUp   (int x, int y, int button) override;

	void show();
	void hide();

	UIDialog (UIManager *manager) : UIPanel (manager) {

		this->style   = "dialog";
		this->manager = manager;

		//visible = false;

		for (int i = 0; i < 9; ++i) {

			drawElements   [i] = std::make_unique<BaseObject>(manager->uiDataRender, app->screenCamera.get());
			drawElementsPtr[i] = drawElements[i].get();

			indices[i] = i;

		}

		precompute();

	}

};
