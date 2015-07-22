
#import <SFML/Window.hpp>
#import "ui/cursor.h"
#import "utility/application.h"
#import "utility/input.h"
#import <Cocoa/Cocoa.h>

void UICursor::set (CursorIco cur) {

	if (cur == cursorIco) return;
	cursorIco = cur;

	id handle = (id)windowHandle;
	Application *app = Application::getInstance();

	/* Get the content view of the handle */

	NSView* view = nil;

	if ([handle isKindOfClass:[NSView class]]) {

		view = handle;

	} else {

		NSWindow* nswindow = handle;
		view = nswindow.contentView;

	}

	NSCursor* defaultCursor = [NSCursor currentCursor];
	NSCursor* cursor;//        = [NSCursor pointingHandCursor];

	/* Get Cursor Ico */

	switch (cur) {

		case CursorIco::Wait         : break;
		case CursorIco::Hand         : cursor = [NSCursor pointingHandCursor];    break;
		case CursorIco::Normal       : cursor = [NSCursor arrowCursor];           break;
		case CursorIco::IBeam        : cursor = [NSCursor IBeamCursor];           break;
		case CursorIco::VDoubleArrow : cursor = [NSCursor resizeUpDownCursor];    break;
		case CursorIco::HDoubleArrow : cursor = [NSCursor resizeLeftRightCursor]; break;
		case CursorIco::CrossHair    : cursor = [NSCursor crosshairCursor];       break;
		case CursorIco::Drag         :

			if (app->mouseButton == mouseLeft) {

				cursor = [NSCursor closedHandCursor];

			} else {

				cursor = [NSCursor openHandCursor];

			}

			break;

		default: return;

	}

	/* Set Cursor */

	NSRect rect = view.frame;

	[view addCursorRect:rect cursor:cursor];
	[cursor setOnMouseEntered:YES];

	[view addCursorRect:rect cursor:defaultCursor];
	[defaultCursor setOnMouseExited:YES];

	NSPoint mousePositionInWindow = [[view window] mouseLocationOutsideOfEventStream];
	NSPoint mousePositionInView   = [view convertPoint:mousePositionInWindow
									 fromView:nil]; // i.e. from the window.

	if ([view mouse:mousePositionInView inRect:view.frame]) {
		[cursor set];
	}

}

/*
void ChangeCursor(sf::Window& window)
{
	id handle = (id)window.getSystemHandle();

	// Get the content view of the handle
	NSView* view = nil;
	if ([handle isKindOfClass:[NSView class]]) {
		view = handle;
	} else {
		NSWindow* nswindow = handle;
		view = nswindow.contentView;
	}

	NSCursor* defaultCursor = [NSCursor currentCursor];

	NSCursor* cursor = [NSCursor pointingHandCursor];
	NSRect rect = view.frame;

	[view addCursorRect:rect cursor:cursor];
	[cursor setOnMouseEntered:YES];

	[view addCursorRect:rect cursor:defaultCursor];
	[defaultCursor setOnMouseExited:YES];

	NSPoint mousePositionInWindow = [[view window] mouseLocationOutsideOfEventStream];
	NSPoint mousePositionInView   = [view convertPoint:mousePositionInWindow
									 fromView:nil]; // i.e. from the window.

	if ([view mouse:mousePositionInView inRect:view.frame]) {
		[cursor set];
	}
}
*/
