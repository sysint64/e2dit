#ifndef H_UI
#define H_UI

#define DisabledAlpha 0.65f
//
class UIElement;
typedef void (*VoidEvent)();
typedef void (*NotifyEvent)(UIElement *Sender);
typedef void (*MouseEvent)(UIElement *Sender, int x, int y, int shift);
typedef void (*KeyEvent)(UIElement *Sender, Uint16 key);

static bool FreezUI = false;
// Alignment
enum alignment { alNone, alAll, alRight, alCenter, alLeft, alTop, alBottom, alHalfRight, alHalfLeft };

class UITheme : public DataMap {
public:
	Texture2D	*Skin;
	FTGLfont	*Font;

	//
	UITheme() {
		Skin = new Texture2D();
	}

	//
	~UITheme() {
		delete Skin;
		ftglDestroyFont (Font);
	}
	//
	void Update();
};

static bool NowClick = false;
//
class UIManager;
class UIElement {
public:
	bool 	Inited;
	int mx, my, mShift;
	//bool	Click;
	int		Width, Height;
	int		Left, Top;
	int		AbsoluteLeft, AbsoluteTop;
	bool	Leav, Enter, Click;
	bool	Focused;
	string	Hint;
	bool	Visible;
	bool	Enabled;
	int		Cursor;
	int		DrawAlign;
	int		Align;
	int		Id;
	int		IconOffset[2];
	int		Icon2Offset[2];
	int		ClickIconOffset[2];
	bool	ShowIcon;
	bool	ShowIcon2;
	bool	ShowClickIcon;
	bool	FixSize;
	UIManager* Manager;
	int		Padding[2];
	int		ScrollWidth, ScrollHeight;
	bool	updPos;
	int		tmpPos[2];
	bool	inDialog;
	bool	Transparent;
	bool	Checked;
	string  Name;
	glm::vec2	AbsolutePos;
	//
	BaseShader *Shader;
	BaseShader *ColorShader;
	UITheme *Theme;
	UIElement  *Parent;
	void	*CustomData;
	//
	// EventsFuncs
	NotifyEvent OnClick;
	NotifyEvent OnProgress;
	MouseEvent OnMouseDown;
	MouseEvent OnDblClick;
	KeyEvent   OnKeyPressed;
	NotifyEvent OnUnFocused;
	
	//
	UIElement() {
		Enabled = true; ShowIcon  = false;
		Visible = true; ShowIcon2 = false;
		ShowClickIcon = false;
		Cursor  = curNormal;
		Focused = false;
		DrawAlign = alAll;
		Align = alNone;
		Parent = 0;
		Padding[0] = 10;
		Padding[1] = 10;
		FixSize = false;
		Width = 0; Height = 0;
		Left = 0; Top = 0;
		updPos = false;
		OnClick = 0;
		OnProgress = 0;
		OnMouseDown = 0;
		OnDblClick = 0;
		OnKeyPressed = 0;
		OnUnFocused = 0;
		CustomData = 0;
		inDialog = false;
		NowClick = false;
		Transparent = false;
		Inited = false;
	}

	virtual ~UIElement() {}
	
	//*
	//
	void SetFocused();
	void UnFocused();
	virtual void Init() { }
	virtual void Free() {}
	virtual void Draw (int x, int y) {}
	
	// Events
	virtual void DblClick  (int x, int y, int Shift);
	//
	virtual void MouseDown (int x, int y, int Shift);
	virtual void MouseMove (int x, int y, int Shift) {}
	virtual void MouseUp   (int x, int y, int Shift);
	virtual void KeyDown     (int key) {}
	virtual void Resized     (int Width, int Height) {}
	virtual void KeyPressed  (Uint16 key);
	virtual void TextEntered (Uint16 key) {}
	//
	virtual void Progress() {
		if (OnProgress != 0)
			OnProgress (this);
	}
};

//
class UIManager {
private:	
	int mx, my, mShift;
	
public:
	vector<UIElement*> Elements;
	vector<UIElement*> DrawStack;
	vector<UIElement*> UnFocusedElements;
	vector<glm::vec4>  ScissorStack;
	UIElement	*FocusedElement;
	BaseShader	*Shader;
	BaseShader	*ColorShader;
	UITheme		*Theme;
	//
	//
	//GLuint	SizesID, OffsetsID;
	glm::vec4 	 Scissor;
	bool		 EnabledScissor;
	
	UIManager();
	~UIManager();
	void Init();
	void Draw();
	inline void AddUIElement(UIElement *el) {
		el->Manager = this;
		el->Id = Elements.size();
		Elements.push_back(el);
	}
	
	//Events
	void DblClick  (int x, int y, int Shift);
	//
	void MouseDown (int x, int y, int Shift);
	void MouseMove (int x, int y, int Shift);
	void MouseUp   (int x, int y, int Shift);
	//
	void KeyDown (int key);
	void Resized     (int Width, int Height);
	void KeyPressed  (Uint16 key);
	void TextEntered (Uint16 key);
	void Progress();
	//
	inline void PushScissor (int sx, int sy, int sw, int sh);
	inline void SetScissor();
	inline void PopScissor();
};
//
bool PointInRect (int X, int Y, int Xe, int Ye, int W, int H);
//
// Include UI
#include "UIStdCtrls.h"
#include "UIMenus.h"
#include "UIGrouped.h"
#include "UITabs.h"

// Sources
#include "src/UIPreComputed.cpp"
#include "src/UIStdCtrls.cpp"
#include "src/UIMenus.cpp"
#include "src/UIGrouped.cpp"
#include "src/UITabs.cpp"

#include "src/UI.cpp"

#endif
