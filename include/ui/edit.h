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

const wchar_t SplitChars[] = L" ,.;:?'!|/\\~*+-=(){}<>[]#%&^@$№`\"";
class UIEdit : public UIElement {
protected:
	int   iWidths [17];    int   iHeights[17];
	float fWidths [17];    float fHeights[17];
	float offsetsX[17];    float offsetsY[17];
	
	float textColors [12];
	float textOffsets[16];

	/* Params */



	/* Render Objects */

	std::unique_ptr<BaseObject> leftElement   = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
	std::unique_ptr<BaseObject> rightElement  = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
	std::unique_ptr<BaseObject> middleElement = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
	std::unique_ptr<BaseObject> iconElement   = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());
	std::unique_ptr<BaseObject> iconElement2  = std::make_unique<BaseObject> (manager->uiDataRender, app->screenCamera.get());

	/* Work With Text */

	void  copyText();
	void  pasteText();
	float parseExpr();

public:

	/* Display Text */

	wstring text   = L"";
	wstring before = L"";
	wstring after  = L"";

	/* Functors: Callback Events */

	std::function<void(UIElement*)>         onChange      = nullptr;
	std::function<void(UIElement*, Uint16)> onTextEntered = nullptr;

	bool trackBar = false;

	/* */

	virtual void precompute() override;
	virtual void render()     override;

	/* Constructor */
	
	UIEdit (UIManager *manager) : UIElement (manager) {
		
		this->manager = manager;
		precompute();

	}

};
