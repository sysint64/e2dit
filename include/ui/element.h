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

#ifndef E2DIT_UI_ELEMENT_H
#define E2DIT_UI_ELEMENT_H

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
enum class Align {None, Left, Center, Right, Client, Bottom, Top, All};

class UIManager;
class UIElement {
protected:
	Application *app = Application::getInstance();

	int   iWidths [32];    int   iHeights[32];
	float fWidths [32];    float fHeights[32];
	float offsetsX[32];    float offsetsY[32];

	float textColors [32];
	float textOffsets[32];

	int paddingTop  = 0;
	int paddingLeft = 0;

	/* Precompute */

	void precomputeElement     (const int n, const std::string &element, const std::string &params);
	void precomputeText        (const int n, const std::string &element);
	void precomputeFloatArray  (const std::string &element, const std::string &params, float *arr, const int size, float normalize = 1.f);
	void precomputeIntArray    (const std::string &element, const std::string &params, int   *arr, const int size);
	void precomputeColor3f     (const std::string &element, const std::string &params, float *arr);
	void precomputeColor4f     (const std::string &element, const std::string &params, float *arr);

	void precomputeException   (const std::string &element, const std::string &params, const int size) const;

	/* Render */

	void renderElement         (int idx, int x, int y, int w, int h, BaseObject *el) const;
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

public:

	std::vector<void*> metaData;

	/* Scroll */

	int scrollElementWidth  = 0;
	int scrollElementHeight = 0;

	int scrollX = 0;
	int scrollY = 0;

	bool autoSize = false;

	/* Icon */

	int iconOffset     [2] = {-1, -1}; bool showIcon      = false;
	int icon2Offset    [2] = {-1, -1}; bool showIcon2     = false;
	int clickIconOffset[2] = {-1, -1}; bool showClickIcon = false;

	/* Navigation (for focus) */

	UIElement *next = nullptr;
	UIElement *prev = nullptr;

	UIElement *lastEl  = nullptr;
	UIElement *firstEl = nullptr;

	/* */

	bool isRoot = false;

	int id;
	int tmp = 0;

	int width  = 0;
	int height = 0;

	int absLeft = 0;
	int absTop  = 0;

	int left = 0;
	int top  = 0;

	int wrapperWidth, wrapperHeight;
	int wrapperWidthClamped, wrapperHeightClamped;

	bool visible = true;
	bool enabled = true;
	bool focused = false;
	bool checked = false;

	bool withoutSkin = false;
	bool wasClick    = false;

	bool keyClick    = false;
	bool allowScroll = false;

	/* Mouse State */

	bool enter  = false;
	bool leave  = true;
	bool click  = false;
	bool over   = false; // true if mouse over element even if another element overlaps it

	bool inDialog = false;

	UIElement *parent  = nullptr;
	UIManager *manager = nullptr;

	CursorIco cursor = CursorIco::Normal;
	Align drawAlign  = Align::All;
	Align align      = Align::None;

	/* Functors: Callback Events */

	std::function<void(UIElement*)>                onClick      = nullptr;
	std::function<void(UIElement*)>                onProgress   = nullptr;
	std::function<void(UIElement*)>                onUnfocused  = nullptr;
	std::function<void(UIElement*, int)>           onKeyPressed = nullptr;
	std::function<void(UIElement*, int, int, int)> onMouseDown  = nullptr;
	std::function<void(UIElement*, int, int, int)> onDblClick   = nullptr;

	/* Childrens */

	std::map<int, std::unique_ptr<UIElement>> elements;

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

	void focus();
	void unfocus();

	virtual void precompute() {}
	virtual void render ();

	void updateAbsPos();
	void checkFocus();

	/* Manage Elements */

	void addElement    (std::unique_ptr<UIElement> el);
	void deleteElement (std::unique_ptr<UIElement> el);
	void deleteElement (const int id);

	UIElement *getElement (const int id);
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

#endif
