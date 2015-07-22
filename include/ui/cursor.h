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

		None			= -1,
		Wait			= XC_watch,
		Hand			= XC_hand1,
		Normal			= XC_left_ptr,
		IBeam			= XC_xterm,
		VDoubleArrow	= XC_sb_v_double_arrow,
		HDoubleArrow	= XC_sb_h_double_arrow,
		CrossHair		= XC_crosshair,
		Question		= XC_question_arrow,
		Drag			= XC_fleur

	};

#endif

#ifdef _osx_

	//#include <Cocoa/Cocoa.h>

	enum class CursorIco {

		None			= -1,
		Wait			= 0, // remove
		Hand			= 1,
		Normal			= 2,
		IBeam			= 3,
		VDoubleArrow	= 4,
		HDoubleArrow	= 5,
		CrossHair		= 6,
		Question		= 7, // remove
		Drag			= 8

	};

#endif

class UICursor {
private:
	sf::WindowHandle windowHandle;
	CursorIco	 cursorIco = CursorIco::Normal;

	#ifdef _linux_
		Display		*display;
		XID			 cursor;
	#endif

public:

	UICursor (sf::WindowHandle windowHandle) {

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

	void set  (CursorIco cur);

	inline CursorIco get() const {

		return cursorIco;

	}

};

#endif
