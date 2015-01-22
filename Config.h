#ifndef H_CONFIG
#define H_CONFIG
//

// Static
static int		WindowWidth  = 1400;
static int		WindowHeight = 840;
static int		ScreenWidth;
static int		ScreenHeight;
static int		OGLMajor = 3;
static int		OGLMinor = 3;
static int		AntiAliasing = 16;
static int		MouseX, MouseY;
static int		Shift;
static float	CurrTime, DeltaTime, LastTime;
static float	FPS;
static string	CurrentTheme = "Hexagon";
static string	lang = "eng";
static bool		DialogOpened;
static bool		HideCursor = false;
static string	ExeDir;
const float		pi = 3.14159265358979323846264338327950f;
static int		LastTexID = 0;
static int		LastTexN  = 0;

// Extension
static bool		VAOEXT = true;
char			*LastErrMsg;
sf::Window 		*MainWindow;
sf::WindowHandle aWindowHandle;

#ifdef _win_
	#define DirSlash '\\'
#endif

#ifdef _linux_
	#define DirSlash '/'

	#define _NET_WM_STATE_REMOVE 0    /* remove/unset property */
	#define _NET_WM_STATE_ADD    1    /* add/set property */
	#define _NET_WM_STATE_TOGGLE 2    /* toggle property  */
#endif

#ifdef _darwin_
	#define DirSlash '/'
#endif

#define PtrSize 8
// Definitions
#define ProgramName "Game Engineer"
#define Uint16 unsigned short
#define BYTE   unsigned char

#define ARRAYSIZE(a) ((sizeof(a) / sizeof(*(a))) / static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

#endif
