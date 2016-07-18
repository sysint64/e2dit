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

/**
 * UIEdit - Editable Field
 *
 * @method copyText  : Copy Selected Text to Clipboard
 * @method pasteText : Paste Selected Text from Clipboard
 * @method parseExpr : Parse simple Expressions, example: 2+4 converted to 6
 */

const wchar_t splitChars[] = L" ,.;:?'!|/\\~*+-=(){}<>[]#%&^@$№`\"";
enum class UITextFilter {Text, Int, Float};
class UIEdit : public UIElement {
protected:

	float selectColor[4];
	float selectOffset[2]; // Offset top and bottom
	int   trackWay      = 0;
	int   lastMouseX    = 0;
	bool  wholeTrackWay = 0;
	int   test          = 0;

	/* Params */

	std::string leaveElement = "leave";
	std::string enterElement = "enter";
	std::string clickElement = "click";
	std::string focusElement = "focus";

	/* Render Objects */

	std::unique_ptr<BaseObject> leftElement       = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
	std::unique_ptr<BaseObject> rightElement      = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
	std::unique_ptr<BaseObject> middleElement     = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
	std::unique_ptr<BaseObject> stickElement      = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
	std::unique_ptr<BaseObject> selectElement     = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
	std::unique_ptr<BaseObject> leftArrowElement  = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
	std::unique_ptr<BaseObject> rightArrowElement = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());

	/* Stick */

	int   stickChPos     = 0;
	int   stickPxPos     = 0;
	float stickTime      = 0;
	bool  showStick      = false;
	int   lastStickChPos = 0;
	bool  firstRender    = true;

	/* Text */

	int textPosX   = 0;
	int textPosY   = 0;
	int textOffset = 0;

	int  cWidth; // FIXME: Rename

	/* Work With Text */

	void  copyText();
	void  pasteText();
	float parseExpr();

	void  updateSelect();
	void  setStickPos (int x, int y);

	void  addVal (const int acc);

	/* Draw methods */

	void  renderSkin();

public:

	UITextFilter filter = UITextFilter::Int;

	/* Track Mode */

	glm::vec2 trackRange { -999, 999 };
	int       trackStep = 2;
	bool      trackMode = true;

	/* Display Text */

	std::wstring text   = L"";
	std::wstring before = L"";
	std::wstring after  = L"";

	/* Select */

	int selStart = 0;
	int selEnd   = 0;

	/* Functors: Callback Events */

	std::function<void(UIElement*)>      onChange      = nullptr;
	std::function<void(UIElement*, int)> onTextEntered = nullptr;

	bool trackBar = false;
	bool expr     = false;

	/* */

	virtual void precompute() override;
	virtual void render()     override;
	virtual void setCursor()  override;
	virtual void focus()      override;

	/* Events */

	virtual void keyPressed  (int key) override;
	virtual void textEntered (int key) override;

	virtual void dblClick    (int x, int y, int button) override;
	virtual void mouseDown   (int x, int y, int button) override;
	virtual void mouseMove   (int x, int y, int button) override;
	virtual void mouseUp     (int x, int y, int button) override;

	/* Constructor */

	UIEdit (UIManager *manager) : UIElement (manager) {

		this->manager = manager;
		this->cursor  = CursorIco::IBeam;

		style = "edit";
		precompute();

	}

};
