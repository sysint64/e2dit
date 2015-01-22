#ifndef H_CURSOR
#define H_CURSOR

#ifdef _linux_
	#include <X11/cursorfont.h>
	#include <X11/Xlib.h>

	Display* display;
	XID cursor;
#endif

#ifdef _win_
	HCURSOR cursor;
#endif

int cursor_icon = -1;
bool LockCursor = false;

#ifdef _linux_
	#define curWait				XC_watch
	#define curHand				XC_hand1
	#define curNormal			XC_left_ptr
	#define curIBeam			XC_xterm
	#define curVDoubleArrow		XC_sb_v_double_arrow
	#define curHDoubleArrow		XC_sb_h_double_arrow
	#define curCrossHair		XC_crosshair
	#define curQuestion			XC_question_arrow
	#define curFleur			XC_fleur
#endif

#ifdef _win_
	#define curWait				0
	#define curHand				1
	#define curNormal			2
	#define curIBeam			3
	#define curVDoubleArrow		4
	#define curHDoubleArrow		5
	#define curCrossHair		6
	#define curQuestion			7
	#define curFleur			8
	#define curDefault			9
#endif

#include "src/Cursor.cpp" 
#endif
