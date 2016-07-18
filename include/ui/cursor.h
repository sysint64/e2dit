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

#ifndef E2DIT_UI_CURSOR_H
#define E2DIT_UI_CURSOR_H

#include <SFML/Window.hpp>

#ifdef _linux_
	#include <X11/cursorfont.h>
	#include <X11/Xlib.h>

	#undef None

	enum class CursorIco {

		None              = -1,
		Hand              = XC_hand1,
		Normal            = XC_left_ptr,
		IBeam             = XC_xterm,
		VDoubleArrow      = XC_sb_v_double_arrow,
		HDoubleArrow      = XC_sb_h_double_arrow,
		CrossHair         = XC_crosshair,
		Drag              = XC_fleur,
		TopSide           = XC_top_side,
		BottomSide        = XC_bottom_side,
		LeftSide          = XC_left_side,
		RightSide         = XC_right_side,
		TopLeftCorner     = XC_top_left_corner,
		TopRightCorner    = XC_top_right_corner,
		BottomLeftCorner  = XC_bottom_left_corner,
		BottomRightCorner = XC_bottom_right_corner

	};

#endif

#if defined(_osx_) || defined(_win_)

	enum class CursorIco {

		None              = -1,
		Hand              = 0,
		Normal            = 1,
		IBeam             = 2,
		VDoubleArrow      = 3,
		HDoubleArrow      = 4,
		CrossHair         = 5,
		Drag              = 6,
		TopSide           = 7,
		BottomSide        = 8,
		LeftSide          = 9,
		RightSide         = 10,
		TopLeftCorner     = 11,
		TopRightCorner    = 12,
		BottomLeftCorner  = 13,
		BottomRightCorner = 14

	};

#endif

#ifdef _win_
	#include <windows.h>
#endif

class UICursor {
private:
	sf::WindowHandle windowHandle;
	CursorIco	 cursorIco = CursorIco::Normal;

	#ifdef _linux_
		Display		*display;
		XID			 cursor;
	#endif

	#ifdef _win_
		HCURSOR cursor;
	#endif

public:

	UICursor (const sf::WindowHandle &windowHandle) {

		#ifdef _linux_
			display = XOpenDisplay (nullptr);
		#endif

		this->windowHandle = windowHandle;

	}

	~UICursor () {

		#ifdef _linux_
			XFreeCursor (display, cursor);
		#endif

	}

	void set (const CursorIco cur);

	inline CursorIco get() const {

		return cursorIco;

	}

};

#endif
