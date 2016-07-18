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
#include "ui/panel.h"

#include "renderer/base_object.h"

class UIDialog : public UIPanel {
protected:

	const int edgeWidth = 15;
	enum class Edge {
		None,
		TopLeftCorner,
		TopRightCorner,
		BottomLeftCorner,
		BottomRightCorner,
		TopSide,
		BottomSide,
		LeftSide,
		RightSide
	};

	std::unique_ptr<BaseObject> drawElements[13];
	BaseObject *drawElementsPtr[13];
	int indices[13];

	int lastLeft   = 0;
	int lastTop    = 0;
	int lastWidth  = 0;
	int lastHeight = 0;

	int   captionArea [4];
	float buttonsTop  [2];
	float buttonsRight[2];
	float opacity     [4];

	bool  headerClick   = false;
	bool  maximizeEnter = false;
	bool  closeEnter    = false;
	bool  edgeClick     = false;
	bool  btnDown       = false;
	Edge  edgeEnter     = Edge::None;

	void renderButtons (int x, int y);
	void handleResize();
	void handleEdgeEnter();

public:

	std::wstring caption    = L"Hello World!";
	bool maximized          = false;
	bool allowResize        = false;
	bool showMaximizeButton = true;

	int minWidth  = 100;
	int minHeight = 40;
	int maxWidth  = 1000;
	int maxHeight = 1000;

	virtual void render()     override;
	virtual void precompute() override;
	virtual void setCursor()  override;

	virtual void mouseDown (int x, int y, int button) override;
	virtual void mouseUp   (int x, int y, int button) override;

	void show();
	void hide();

	UIDialog (UIManager *manager) : UIPanel (manager) {

		this->style   = "dialog";
		this->manager = manager;

		//visible = false;

		for (int i = 0; i < 13; ++i) {

			drawElements   [i] = std::make_unique<BaseObject>(manager->uiDataRender, app->screenCamera.get());
			drawElementsPtr[i] = drawElements[i].get();

			indices[i] = i;

		}

		precompute();

	}

};
