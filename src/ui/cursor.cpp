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

#include "ui/cursor.h"

/**
 * Set system set cursor
 * @param cur: Cursor Icon
 */

void UICursor::set (CursorIco cur) {

	if (cur == cursorIco) return;
	cursorIco = cur;

	#ifdef _linux_
		cursor = XCreateFontCursor (display, static_cast<unsigned int> (cursorIco));
		XDefineCursor (display, windowHandle, cursor);
		XFlush (display);
	#endif

	#ifdef _win_
		//cursor = LoadCursor (nullptr, cur);

		switch (cur) {

			case CursorIco::CrossHair    : cursor = LoadCursor (nullptr, IDC_CROSS);   break;
			case CursorIco::Hand         : cursor = LoadCursor (nullptr, IDC_HAND);    break;
			case CursorIco::Normal       : cursor = LoadCursor (nullptr, IDC_ARROW);   break;
			case CursorIco::IBeam        : cursor = LoadCursor (nullptr, IDC_IBEAM);   break;
			case CursorIco::VDoubleArrow : cursor = LoadCursor (nullptr, IDC_SIZENS);  break;
			case CursorIco::HDoubleArrow : cursor = LoadCursor (nullptr, IDC_SIZEWE);  break;
			case CursorIco::Drag         : cursor = LoadCursor (nullptr, IDC_SIZEALL); break;

			default: return;

		}

		SetClassLongPtr (windowHandle, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(cursor));
	#endif

}
