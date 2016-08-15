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

#include "utility/config.h"
#include "utility/application.h"
#include "utility/string.h"
#include "utility/renderer.h"

#include "renderer/base_object.h"
#include "renderer/shader.h"

#include "ui/cursor.h"

#include "boost/assign/std/vector.hpp"
#include <functional>
#include <map>

#undef None

using namespace boost::assign;
enum class Align {None, Left, Center, Right, Client, Bottom, Top, Middle, All};

class UIManager;
class UIPanel;
class UIButton;
class UITreeListNode;
class UITreeList;
class UIStackLayout;

/**
 * \brief Base user interface class
 * Such class have base parameters for rendering, events...
 */

class UIElement {
protected:
	Application *app = Application::getInstance();

	int   iWidths [32];    int   iHeights[32];
	float fWidths [32];    float fHeights[32];
	float offsetsX[32];    float offsetsY[32];
	int  iOffsetsX[32];    int  iOffsetsY[32];

	float textColors [32];
	float textOffsets[32];

	int scrollElementWidth  = 0;
	int scrollElementHeight = 0;

	int depth = 0;

	/* Navigation (for focus) */

	UIElement *next = nullptr;
	UIElement *prev = nullptr;

	UIElement *lastEl  = nullptr;
	UIElement *firstEl = nullptr;

	int wrapperWidth, wrapperHeight;
	int wrapperWidthClamped, wrapperHeightClamped;

	int overWidth  = -1;
	int overHeight = -1;

	bool wasClick = false; // ???
	bool keyClick = false; // ???

	bool focused  = false;
	bool inDialog = false;

	int  layer    = 0; ///< Work layer, if current layer not eq of this layer, then element will disabled

	virtual void updateAlign();
	virtual void updateVerticalAlign();

	/* Precompute */

	void  precomputeElement     (const int n, const std::string &element, const std::string &params);
	void  precomputeText        (const int n, const std::string &element);
	void  precomputeFloatArray  (const std::string &element, const std::string &params, float *arr, const int size, float normalize = 1.f);
	void  precomputeIntArray    (const std::string &element, const std::string &params, int   *arr, const int size);
	void  precomputeColor3f     (const std::string &element, const std::string &params, float *arr);
	void  precomputeColor4f     (const std::string &element, const std::string &params, float *arr);
	float precomputeFloat       (const std::string &element, const std::string &params);
	int   precomputeInt         (const std::string &element, const std::string &params);

	void  precomputeException   (const std::string &element, const std::string &params, const int size) const;

	/* Render */

	void renderElement         (int idx, int x, int y, int w, int h, BaseObject *el) const;
	void transformElement      (int x, int y, int w, int h, BaseObject *el) const;
	void renderMaskElement     (int maskidx, int idx, int x, int y, int w, int h, BaseObject *el) const;
	void renderColorElement    (int x, int y, int w, int h, BaseObject *el, float *color) const;

	void renderPartsElementH   (int il, int ic, int ir,
	                            BaseObject *el, BaseObject *ec, BaseObject *er,
	                            int x, int y, int w, bool ignoreDrawAlign = false) const;

	void renderPartsElementH   (int il, int ic, int ir,
	                            BaseObject *el, BaseObject *ec, BaseObject *er,
	                            int x, int y, int w, int h, bool ignoreDrawAlign = false) const;

	void renderPartsElementV   (int it, int im, int ib,
	                            BaseObject *et, BaseObject *em, BaseObject *eb,
	                            int x, int y, int h, bool ignoreDrawAlign = false) const;

	void renderPartsElementV90 (int it, int im, int ib,
	                            BaseObject *et, BaseObject *em, BaseObject *eb,
	                            int x, int y, int h, bool ignoreDrawAlign = false) const;

	inline void renderPartsElementBlock (int itl, int itc, int itr,
	                                     int iml, int imc, int imr,
	                                     int ibl, int ibc, int ibr,
	                                     BaseObject *etl, BaseObject *etc, BaseObject *etr,
	                                     BaseObject *eml, BaseObject *emc, BaseObject *emr,
	                                     BaseObject *ebl, BaseObject *ebc, BaseObject *ebr,
	                                     int x, int y, int w, int h) const
	{

		const int mh = h-iHeights[itc]-iHeights[ibc];

		renderPartsElementH (itl, itc, itr, etl, etc, etr, x, y, w);
		renderPartsElementH (iml, imc, imr, eml, emc, emr, x, y+iHeights[itc], w, mh);
		renderPartsElementH (ibl, ibc, ibr, ebl, ebc, ebr, x, y+iHeights[itc]+mh, w);

	}

	inline void renderPartsElementBlock (int pts[9], BaseObject *els[9],
	                                     int x, int y, int w, int h) const
	{
		renderPartsElementBlock (pts[0], pts[1], pts[2],
		                         pts[3], pts[4], pts[5],
		                         pts[6], pts[7], pts[8],
		                         els[0], els[1], els[2],
		                         els[3], els[4], els[5],
		                         els[6], els[7], els[8],
		                         x, y, w, h);
	}

	inline int offsetSizeX() {
		return marginLeft+marginRight+parent->paddingLeft+parent->paddingRight;
	}

	inline int offsetSizeY() {
		return marginTop+marginBottom+parent->paddingTop+parent->paddingBottom;
	}

public:

	friend UIManager;
	friend UIPanel;
	friend UITreeListNode;
	friend UITreeList;
	friend UIStackLayout;

	std::vector<void*> metaData;  ///< Extended data

	/* Mouse State */

	bool enter     = false;
	bool leave     = true;
	bool click     = false;
	bool over      = false; ///< true if mouse over element even if another element overlaps it
	bool alignSize = false; ///< used in updateAlign method. If true then element change size depenging on the alignment

	std::wstring hint = L"";
	std::string  name = "";
	int          tag  = 0;

	/* Scroll */

	int scrollX = 0; ///< Horizontal scroll inside element
	int scrollY = 0; ///< Vertical scroll inside element

	bool autoSize   = false; ///< Size will calculate automatically if true
	bool autoWidth  = false;
	bool autoHeight = false;
	bool allowAlign = true;

	/* */

	bool isRoot = false; ///< Root of system

	int id; ///< Unique identifier

	int width   = 0; int height = 0;
	int absLeft = 0; int absTop = 0;
	int left    = 0; int top    = 0;

	// Margin
	int marginLeft  = 0; int marginRight   = 0;
	int marginTop   = 0; int marginBottom  = 0;

	// Padding
	int paddingLeft = 0; int paddingRight  = 0;
	int paddingTop  = 0; int paddingBottom = 0;

	bool visible = true;
	bool enabled = true;
	bool checked = false;

	bool withoutSkin = false; ///< No draw skin element if true
	bool allowScroll = false;

	UIElement *parent  = nullptr; ///< Parent element
	UIManager *manager = nullptr; ///< Elements manager

	CursorIco cursor    = CursorIco::Normal; ///< System cursor icon
	Align drawAlign     = Align::All;        ///< Skin draw align: Left [-]; Center -; Right ->; All <->
	Align align         = Align::None;
	Align verticalAlign = Align::None;

	std::string style = "";

	/* Functors: Callback Events */

	std::function<void(UIElement*)>                onClick      = nullptr;
	std::function<void(UIElement*)>                onProgress   = nullptr;
	std::function<void(UIElement*)>                onUnfocused  = nullptr;
	std::function<void(UIElement*, int)>           onKeyPressed = nullptr;
	std::function<void(UIElement*, int, int, int)> onMouseDown  = nullptr;
	std::function<void(UIElement*, int, int, int)> onDblClick   = nullptr;

	/* Childrens */

	std::map<int, std::unique_ptr<UIElement>> elements;

	inline bool isEnter()   { return enter;   }
	inline bool isLeave()   { return leave;   }
	inline bool isClick()   { return click;   }
	inline bool isOver ()   { return over;    }
	inline bool isFocused() { return focused; }

	/* Methods */

	UIElement (UIManager *manager) {

		this->manager = manager;
		precompute();

	}

	inline int maxScrollX() { return wrapperWidthClamped-width+scrollElementWidth;    }
	inline int maxScrollY() { return wrapperHeightClamped-height+scrollElementHeight; }

	inline bool canScrollX() {
		return allowScroll && scrollElementWidth  != 0 && scrollX > 0 && scrollX < maxScrollX();
	}

	inline bool canScrollY() {
		return allowScroll && scrollElementHeight != 0 && scrollY > 0 && scrollY < maxScrollY();
	}

	virtual ~UIElement() {}

	virtual void focus();
	virtual void unfocus();

	virtual void precompute() {}
	virtual void render ();

	void updateAbsPos();
	void checkFocus();

	inline std::array<int, 4> getBound() {
		return { absLeft, absTop, width, height };
	}

	inline std::array<int, 4> getScreenBound() {
		return { absLeft, app->windowHeight-height-absTop, width, height };
	}

	/* Manage Elements */

	virtual void addElement    (std::unique_ptr<UIElement> el);
	virtual void deleteElement (std::unique_ptr<UIElement> el);
	virtual void deleteElement (const int id);

	UIElement *findElement (const std::string &name);
	UIElement *getElement  (const int id);
	std::unique_ptr<UIElement> takeElement (const int id);

	/* Manager Elements Events */

	virtual void poll();
	virtual void setCursor();

	/* Events */

	virtual void dblClick    (int x, int y, int button);
	virtual void mouseDown   (int x, int y, int button);
	virtual void mouseUp     (int x, int y, int button);
	virtual void mouseMove   (int x, int y, int button);
	virtual void mouseWheel  (int dx, int dy);

	virtual void resized     (int width, int height);
	virtual void keyPressed  (int key);
	virtual void keyReleased (int key);
	virtual void textEntered (int key);
	virtual void progress    ();

};

class UICheckedElements : public UIElement {
protected:
	UIElement *lastSelected = nullptr;

public:

	friend UIButton;
	bool multiSelect = false;

	inline void checkElement (UIElement* el) {

		if (el->parent != this)
			return;

		if (!multiSelect && lastSelected != nullptr)
			lastSelected->checked = false;

		el->checked  = true;
		lastSelected = el;

	}

	inline void uncheck() {

		if (!multiSelect && lastSelected != nullptr)
			lastSelected->checked = false;

	}

	inline void uncheckAll() {

		for (const auto &kvp : elements)
			kvp.second->checked = false;

	}

	inline void autoCheck() {

		for (const auto &kvp : elements) {

			if (kvp.second->checked) {
				lastSelected = kvp.second.get();
				break;
			}

		}

	}

	UICheckedElements (UIManager *manager) : UIElement (manager) {

		this->manager = manager;
		precompute();

	}

};
