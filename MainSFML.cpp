
#define GLEW_STATIC
#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
//#define GLM_FORCE_AVX for x86_64
#define GLM_FORCE_INLINE
#define GLM_FORCE_PURE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/fast_square_root.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
//

#include "GL/glew.h"
//#include "GL/wglew.h"

#ifdef _win_
	#include "windows.h"
#endif
//
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <math.h>
#include <stdexcept>
#include <clocale>
#include <locale>
#include <iconv.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#ifdef _linux_
	#include <mntent.h>
#endif

#include <unistd.h>
//
using namespace std;
//
void Terminate (const int type);
//
#include "Engine/Logger.h"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
//#include <SFML/Window/Mouse.hpp>

#include <iostream>
#include <pthread.h>

#include <SOIL/SOIL.h>
#include <FTGL/ftgl.h>

using namespace FTGL;

#include "Config.h"
#include "Utils.h"
#include "fs.h"
#include "Containers.h"

//
#include "gapi/gapi.h"
#include "gapi/DebugDraw.h"

#include "Engine/Dicts.h"
#include "Engine/DataMap.h"
#include "Engine/Keyboard.h"
#include "Engine/Cursor.h"

// User Interface (UI)
#include "UI/UI.h"

#include "Engine/HotKeys.h"
#include "Engine/EditableObject.h"
#include "Engine/FormData.h"

#include "Engine/Dialogs/OKDialog.h"
#include "Engine/Dialogs/YesNoDialog.h"
#include "Engine/Dialogs/AddVar.h"
#include "Engine/Dialogs/AddUniform.h"
#include "Engine/Dialogs/SetUniform.h"
#include "Engine/Dialogs/ObjectParams.h"
#include "Engine/Dialogs/AddSpriteForm.h"
#include "Engine/Dialogs/ColorDialog.h"
#include "Engine/Dialogs/Finder.h"
#include "Engine/Dialogs/UserPreferences.h"
#include "Engine/Dialogs/Animations.h"
#include "Engine/Dialogs/SetStrip.h"
#include "Engine/Dialogs/SetPolygon.h"
#include "Engine/Dialogs/RetopologyDialog.h"

#include "Engine/ObjectVarsEvents.h"
#include "Engine/MapEditor2D.h"
#include "Engine/Engine.h"

// Sources
#include "gapi/src/DebugDraw.cpp"

/**/

#include "Engine/src/HotKeys.cpp"
#include "Engine/src/Engine.cpp"

#include "Engine/Dialogs/src/OKDialog.cpp"
#include "Engine/Dialogs/src/YesNoDialog.cpp"
#include "Engine/Dialogs/src/AddVar.cpp"
#include "Engine/Dialogs/src/AddUniform.cpp"
#include "Engine/Dialogs/src/SetUniform.cpp"
#include "Engine/Dialogs/src/ObjectParams.cpp"
#include "Engine/Dialogs/src/AddSpriteForm.cpp"
#include "Engine/Dialogs/src/ColorDialog.cpp"
#include "Engine/Dialogs/src/Finder.cpp"
#include "Engine/Dialogs/src/UserPreferences.cpp"
#include "Engine/Dialogs/src/Animations.cpp"
#include "Engine/Dialogs/src/SetStrip.cpp"
#include "Engine/Dialogs/src/SetPolygon.cpp"
#include "Engine/Dialogs/src/RetopologyDialog.cpp"

#include "Engine/src/ObjectVarsEvents.cpp"
#include "Engine/src/MapEditor2D.cpp"
#include "Engine/src/EditableObject.cpp"
#include "Engine/src/SaveLoadMap.cpp"

//
void Draw() {
	Core->Draw();
}
//
void Step() {
	Core->Step();
}
//
inline void Free() {
	delete Core;
}

//
void Terminate (const int type) {
	//
	Free();
	FreeCursor();
	Config->SaveToFile ("Config.thm");
	//
	exit(type);
}

//* Events *//
//
// Keyboard
/*inline void KeyPressed (GLFWwindow* window, int key, int scancode, int PR, int mods) {
	//
	if (PR == GLFW_PRESS) {
		Core->KeyPressed(key);
		isKey[key] = true;
	}
	
	if (PR == GLFW_RELEASE) isKey[key] = false;
}*/

inline void KeyPressed (int key) {
	//
	Core->KeyPressed(key);
	isKey[key] = true;

	if (key == keyRShift || key == keyLShift) isKey[keyShift] = true;
	if (key == keyRCtrl  || key == keyLCtrl)  isKey[keyCtrl]  = true;
}

inline void KeyReleased (int key) {
	//
	Core->KeyReleased(key);
	isKey[key] = false;

	if (key == keyRShift || key == keyLShift) isKey[keyShift] = false;
	if (key == keyRCtrl  || key == keyLCtrl)  isKey[keyCtrl]  = false;
}
//
inline void TextEntered (unsigned int key) {
	Core->TextEntered(key);
}

// Mouse
void MouseButton(int button) {
	/*//
	if (PR == GLFW_PRESS) {
		Shift = button;
		Core->MouseDown(MouseX, MouseY, Shift);
	}
	//
	if (PR == GLFW_RELEASE) {
		Shift = mouseNone;
		Core->MouseUp(MouseX, MouseY, Shift);
	}*/
}

inline void MouseDown (int x, int y, int button) {
	//
	Shift = button;
	Core->MouseDown(x, y, Shift);
}

inline void MouseUp (int x, int y, int button) {
	Shift = mouseNone;
	Core->MouseUp(x, y, button);
}

inline void DblClick (int x, int y, int button) {
	Shift = mouseNone;
	Core->DblClick(x, y, button);
}

inline void MouseMove (double x, double y) {
	MouseX = x; MouseY = y;
	Core->MouseMove(x, y, Shift);
}

inline void MouseWheel (int delta, int x, int y) {
	Core->MouseWheel (delta, x, y);
}

// Window
inline void Resized (int width, int height) {
	//
	Core->Resized (width, height);
	if (height == 0) height = 1;
	//
	WindowWidth  = width;
	WindowHeight = height;
}

//* Main driver code *//
//

int main() {
	//
	setlocale(LC_CTYPE, "");
	DebugInit (5);
	LoggerCreate ("log.txt");
	ExeDir = fs::getExeDir();

	LOG_DEBUG("%s\n", ExeDir.c_str());

	sf::ContextSettings Settings;
	//
	Settings.depthBits         = 24;
	Settings.stencilBits       = 8;
	Settings.antialiasingLevel = 0;
	Settings.majorVersion      = OGLMajor;
	Settings.minorVersion      = OGLMinor;

	ScreenWidth  = sf::VideoMode::getDesktopMode().width;
	ScreenHeight = sf::VideoMode::getDesktopMode().height;

	WindowWidth  = ScreenWidth -40;
	WindowHeight = ScreenHeight-80;

	sf::Window App (sf::VideoMode (WindowWidth, WindowHeight, 24), "", sf::Style::Default, Settings);

	aWindowHandle = App.getSystemHandle();
	Shift = mouseNone;

	/* Maximize Window */

	//InitCursor();

	//#ifdef _linux_

	XEvent xev;
	Display* dpy  = XOpenDisplay(0);
	Atom wm_state = XInternAtom (dpy, "_NET_WM_STATE", False);
	Atom max_horz = XInternAtom (dpy, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
	Atom max_vert = XInternAtom (dpy, "_NET_WM_STATE_MAXIMIZED_VERT", False);
	Atom wm_add   = XInternAtom (dpy, "_NET_WM_STATE_ADD", False);

	memset(&xev, 0, sizeof(xev));
	xev.type = ClientMessage;
	xev.xclient.window = aWindowHandle;
	xev.xclient.message_type = wm_state;
	xev.xclient.format = 24;
	xev.xclient.data.l[0] = _NET_WM_STATE_ADD;
	xev.xclient.data.l[1] = max_horz;
	xev.xclient.data.l[2] = max_vert;

	XSendEvent (dpy, DefaultRootWindow(dpy), False, SubstructureNotifyMask, &xev);

	//#endif

	#ifdef _win_
	#endif
	
	App.setVerticalSyncEnabled(false);
	App.setFramerateLimit(0);

	MainWindow = &App;
	sf::Clock Clock;

	//set_wm_state(aWindowHandle, _NET_WM_STATE_TOGGLE, _NET_WM_STATE_MAXIMIZED_VERT, true);
	//set_wm_state(aWindowHandle, _NET_WM_STATE_TOGGLE, _NET_WM_STATE_MAXIMIZED_HORZ, true);
	
	//glewInit();
	GLenum err = glewInit();

	if (err != GLEW_OK) {
		LOG_ERROR("Error: %s\n", glewGetErrorString(err));
		Terminate(EXIT_SUCCESS);
	}

	if (!__GLEW_VERSION_2_1) {
		LOG_ERROR("Error: OpenGL Version less than 2.1\n");
		Terminate(EXIT_SUCCESS);
	}

	if (OGLMajor == 3 && !__GLEW_VERSION_3_3) {
		LOG_ERROR("Error: OpenGL Version less than 3.3\n");
		Terminate(EXIT_SUCCESS);
	}

	if (OGLMajor == 4 && !__GLEW_VERSION_4_3) {
		LOG_ERROR("Error: OpenGL Version less than 4.3\n");
		Terminate(EXIT_SUCCESS);
	}
	
	LOG_DEBUG ("OpenGL Version : %d.%d\n", OGLMajor, OGLMinor);

	/* Initizlize Core */
	
	Core = new Engine();
	Core->Init();

	bool running = true;
	bool dpresed = false;
	float dtime = 0;
	float stime = 0;

	glDisable(GL_CULL_FACE);
	//
	glDisable   (GL_MULTISAMPLE);
	glDisable   (GL_DEPTH_TEST);
	glEnable    (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport (0, 0, ScreenWidth, ScreenHeight);
	glClearColor (Core->Theme->Data["general"].Func["clearcolor"][0].Num/255.0f,
				  Core->Theme->Data["general"].Func["clearcolor"][1].Num/255.0f,
				  Core->Theme->Data["general"].Func["clearcolor"][2].Num/255.0f, 0);

	/* Main Loop */

	while (running) {
		//
		SetSysCursor(curWait);
		App.setMouseCursorVisible (!HideCursor);
		sf::Event Event;

		while (App.pollEvent(Event)) {
			switch (Event.type) {
				// Window
				case sf::Event::Closed	: running = false; break;
				case sf::Event::Resized	: Resized (Event.size.width, Event.size.height); break;
				
				// Mouse
				case sf::Event::MouseMoved			: MouseMove  (Event.mouseMove.x     , Event.mouseMove  .y); break;
				case sf::Event::MouseWheelMoved		: MouseWheel (Event.mouseWheel.delta, Event.mouseWheel .x, Event.mouseWheel.y); break;
				case sf::Event::MouseButtonPressed	: MouseDown  (Event.mouseButton.x   , Event.mouseButton.y, Event.mouseButton.button); break;
				case sf::Event::MouseButtonReleased :
					MouseUp   (Event.mouseButton.x, Event.mouseButton.y, Event.mouseButton.button);
					//
					if (dpresed)
						DblClick (Event.mouseButton.x, Event.mouseButton.y, Event.mouseButton.button);
					//
					dpresed = true;
				break;
					
				// Keyboard
				case sf::Event::KeyPressed	: KeyPressed  (Event.key.code); break;
				case sf::Event::TextEntered	: TextEntered (Event.text.unicode); break;
				case sf::Event::KeyReleased : KeyReleased (Event.key.code); break;
			}
		}
		//
		App.setActive();
		//
		CurrTime = Clock.getElapsedTime().asSeconds();
		DeltaTime = (CurrTime - LastTime)*0.001f;
		
		if (dpresed) dtime += DeltaTime*1000;
		else dtime = 0;

		stime += DeltaTime*1000;
		//
		if (dtime >= 0.3f) {
			dpresed = false;
			dtime = 0;
		}

		//if (stime >= 0.01f) {
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			Draw();
			Core->Step();
			stime = 0;
		//}
		LastTime = CurrTime;
		//
		glFlush();
		App.display();
	}
	//

	/* End */

	Terminate(EXIT_SUCCESS);
	LOG_DEBUG("End programm successfuly");
	//
	return 0;
}
