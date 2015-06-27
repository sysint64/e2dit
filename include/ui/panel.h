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

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "utility/renderer.h"
#include "utility/ui.h"
#include "utility/math.h"
#include "utility/input.h"

#include "ui/element.h"
#include "ui/manager.h"

#include "renderer/shader.h"
#include "renderer/data_render.h"
#include "renderer/base_object.h"

class UIPanel : public UIElement {
protected:

	std::unique_ptr<BaseObject> scrollBg [6];
	std::unique_ptr<BaseObject> scrollBtn[6];

	std::unique_ptr<BaseObject> split       = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
	std::unique_ptr<BaseObject> expandArrow = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
	std::unique_ptr<BaseObject> quadElement = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());

	float backgroundLight [4];
	float backgroundDark  [4];
	float backgroundAction[4];

	/* Scroll */

	int hsOffset = 0; int vsOffset = 0; // Horizontal && Vertical Offset for UI Elements in Px
	int hbOffset = 0; int vbOffset = 0; // Horizontal && Vertical Offset for Scroll Buttons in Px
	int lhbOffset, lvbOffset;           // Last Horizontal && Vertical Buttons Offset in Px

	int scrollOffset = 0;

	/* Split */

	int  splitx, splity;
	int  splitw, splith;
	int  curSize;
	int  panelSize;
	bool clicked;
	bool scrollClicked;
	bool splitClick = false;
	int  headerSize;

	/* */

	int scrollHClick = false;
	int scrollVClick = false;

	void updateAlign();
	void renderScroll();

public:

	int hbSize; // Horisontal button size
	int vbSize; // Vertical button size

	int hbMin, hbMax;
	int vbMin, vbMax;

	bool showScrollX = true;
	bool showScrollY = true;

	enum class Background {Transparent, Light, Dark, Action};
	Background background = Background::Light;
	bool allowResize = true;
	bool allowHide   = false;
	bool open        = true;

	std::wstring caption = L"";
	std::vector<UIElement*> joinElements;

	/* Scroll */

	void addScrollXByPx (int pxVal);  void addScrollXByPct (int pctVal);
	void addScrollYByPx (int pxVal);  void addScrollYByPct (int pctVal);
	void setScrollYByPx (int pxVal);  void setScrollYByPct (int pctVal);
	void setScrollXByPx (int pxVal);  void setScrollXByPct (int pctVal);
	/*   ^^^^^^^^^^^^^^                    ^^^^^^^^^^^^^^^
	     By Pixels                         By Percent                */

	inline int getScrollYByPx() { return vsOffset; }
	inline int getScrollXByPx() { return hsOffset; }

	/* Main */

	virtual void precompute() override;
	virtual void render()     override;

	/* Events */

	virtual void keyPressed  (int key)                  override;
	virtual void dblClick    (int x, int y, int button) override;
	virtual void mouseDown   (int x, int y, int button) override;
	virtual void mouseMove   (int x, int y, int button) override;
	virtual void mouseUp     (int x, int y, int button) override;

	/* Constructor */

	UIPanel (UIManager *manager) : UIElement (manager) {

		for (int i = 0; i < 6; i++) {

			scrollBg [i] = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
			scrollBtn[i] = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());

			if (i >= 3) {

				scrollBg [i]->setRotation (math::pi/2.f);
				scrollBtn[i]->setRotation (math::pi/2.f);

			}

		}

		this->manager = manager;
		precompute();

	}

};
