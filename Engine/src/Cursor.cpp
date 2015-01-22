#ifndef CPP_CURSOR
#define CPP_CURSOR

#ifdef _linux_
	//
	void InitCursor() {
		display = XOpenDisplay(NULL);
	}
	//
	void FreeCursor() {
		XFreeCursor(display, cursor);
	    delete display;
	    display = NULL;
	}
	//
	void SetSysCursor (const int cur_code, const bool forceChange = false) {
		//
		if (LockCursor) return;
		LockCursor = forceChange;
		//
		if (cursor_icon == cur_code)  return;
		cursor_icon = cur_code;

		cursor = XCreateFontCursor(display, cur_code);
		XDefineCursor(display, aWindowHandle, cursor);
	    XFlush(display);
	}
#endif

#ifdef _win_
	//
	void InitCursor() {}
	void FreeCursor() {}
	//
	void SetSysCursor (const int cur_code, const bool forceChange = false) {
		//
		//if (LockCursor) return;
		LockCursor = forceChange;
		//
		//if (cursor_icon == cur_code)  return;
		cursor_icon = cur_code;

		switch (cur_code) {
			case curWait			: cursor = LoadCursor (NULL, IDC_WAIT);    break;
			case curHand			: cursor = LoadCursor (NULL, IDC_HAND);    break;
			case curNormal			: cursor = LoadCursor (NULL, IDC_ARROW);   break;
			case curIBeam			: cursor = LoadCursor (NULL, IDC_IBEAM);   break;
			case curVDoubleArrow	: cursor = LoadCursor (NULL, IDC_SIZENS);  break;
			case curHDoubleArrow	: cursor = LoadCursor (NULL, IDC_SIZEWE);  break;
			case curCrossHair		: cursor = LoadCursor (NULL, IDC_CROSS);   break;
			case curQuestion		: cursor = LoadCursor (NULL, IDC_HELP);    break;
			case curFleur			: cursor = LoadCursor (NULL, IDC_SIZEALL); break;
			case curDefault			: cursor = LoadCursor (NULL, IDC_ARROW);   break;
			default					: cursor = LoadCursor (NULL, IDC_ARROW);   break;
		}

		SetClassLongPtr(aWindowHandle, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(cursor));
		//SetCursor(cursor);
		//cursor = LoadCursor (NULL, IDC_HAND);
		//SetSystemCursor (cursor, OCR_NORMAL);
	}
#endif

#endif
