#ifndef H_UISTDCTRLS
#define H_UISTDCTRLS
//
struct Color { float r,g,b; };
//
class UILabel : public UIElement {
private:
	float TextColor [3]; float ShadowColor [3];
	float TextOffset[2]; float ShadowOffset[2];
	bool  TextShadow;
	//
public:
	int		TextAlign;
	wstring	Caption;
	//
	UILabel() : TextAlign (alLeft) {}
	virtual void Init();
	virtual void Draw (int x, int y);
};
//
class UIImage : public UIElement {
private:
	BaseObject	*QuadElement;
	//
public:
	float tWidth , tHeight;
	float OffsetX, OffsetY;
	int itWidth, itHeight;
	int itOffsetX, itOffsetY;
	glm::vec4 Color;
	//
	Texture2D *Texture;
	//
	void Update();
	virtual void Init();
	virtual void Draw (int x, int y);
};

//
class UIColorPanel : public UIElement {
private:
	BaseObject *QuadElement;
	//
public:
	glm::vec4 Color;
	int itWidth, itHeight;
	float tWidth , tHeight;
	bool ShowGrid;
	//
	UIColorPanel() : ShowGrid(true), Color(glm::vec4 (0, 0, 0, 1)) {}
	//
	virtual void Init();
	virtual void Draw (int x, int y);
	inline  void Update();
};

//
class UIHr : public UIElement {
private:
	BaseObject	*QuadElement;
	//
public:
	int   iWidth , iHeight;
	float tWidth , tHeight;
	float OffsetX, OffsetY;
	//
	~UIHr() { delete QuadElement; }
	virtual void Init();
	virtual void Draw (int x, int y);
};
//
class UICustomButton : public UIElement {
//protected:
	//
	
public:
	//void (*)() OnClick;
	//int Top, Left, Width;
	UICustomButton() {}
	//
	virtual void MouseDown (int x, int y, int Shift) {}
	virtual void MouseMove (int x, int y, int Shift) {}
	virtual void MouseUp   (int x, int y, int Shift) {}
};

// UIButton - standart button
class UIButton : public UICustomButton {
protected:
	int   iWidths [12];    int   iHeights[12];
	float Widths  [12];    float Heights [12];
	float OffsetsX[12];    float OffsetsY[12];
	//
	float TextColors [12]; float ShadowColors [12];
	float TextOffsets[16]; float ShadowOffsets[16];
	bool  TextShadows[4];
	//
	string LeavParam;
	string EnterParam;
	string ClickParam;
	//
	BaseObject	*LeftElement;
	BaseObject	*RightElement;
	BaseObject	*MiddleElement;
	BaseObject	*IconElement;
	BaseObject	*IconElement2;
	//
	bool NoDrawText;
	int  IcoOffset;
	//
	void DrawText (int Align, string Text, int Size, int Offset = 0);
	
public:
	int TextAlign;
	wstring Caption;
	//
	UIButton();
	~UIButton();
	//
	virtual void Init();
	virtual void Draw (int x, int y);
	virtual void MouseDown (int x, int y, int Shift) {
		//if (DialogOpened && !inDialog)
		//	return;

		mShift = Shift;
		UIElement::MouseDown (x, y, Shift);
	}

	virtual void MouseUp (int x, int y, int Shift) {
		//if (DialogOpened && !inDialog)
		//	return;

		mShift = mouseNone;
		UIElement::MouseUp (x, y, Shift);
	}

	virtual void DblClick (int x, int y, int Shift) {
		//if (DialogOpened && !inDialog)
		//	return;

		UIElement::DblClick (x, y, Shift);
	}
};

// UICheckButton - checked button
class UICheckButton : public UIButton {
public:
	UIElement *Join;
	//
	UICheckButton (const bool Std = false);
	virtual void Draw (int x, int y);
};

// UICheckBox
class UICheckBox : public UIElement {
private:
	int   iWidths [6];    int   iHeights[6];
	float Widths  [6];    float Heights [6];
	float OffsetsX[6];    float OffsetsY[6];
	float TextColors [6]; float ShadowColors [6];
	float TextOffsets[4]; float ShadowOffsets[4];
	bool  TextShadows[2];
	//
	BaseObject	*BoxElement;
	
public:
	wstring Caption;
	//bool    Checked;
	//
	UICheckBox();
	~UICheckBox() { delete BoxElement; }
	//
	virtual void Init();
	virtual void Draw(int x, int y);
	virtual void MouseDown (int x, int y, int Shift);
};

// UIEdit
const wchar_t SplitChars[] = L" ,.;:?'!|/\\~*+-=(){}<>[]#%&^@$№`\"";
class UIEdit : public UIElement {
private:
	int   iWidths [17];    int   iHeights[17];
	float Widths  [17];    float Heights [17];
	float OffsetsX[17];    float OffsetsY[17];
	//
	float TextColors [12]; float ShadowColors [12];
	float TextOffsets[16]; float ShadowOffsets[16];
	bool  TextShadows[4];  float RectColor[4];
	float RectOffset[2];
	//
	bool  Stick;
	bool  Clicked;
	float StickCounter;
	int   sPos, lPos, sLen;
	int   selPos;
	int   cWidth;
	int   sx, sy;
	int   lx, dx, ly;
	int   tx, ty;
	int   nchinrect;
	int   SelStart, SelEnd;
	int	  toffset;
	//
	BaseObject	*LeftElement;
	BaseObject	*RightElement;
	BaseObject	*MiddleElement;
	//
	BaseObject	*SelectRectElement;
	BaseObject	*StickElement;
	//
	BaseObject	*LeftArrow;
	BaseObject	*RightArrow;
	
	//
	void  CopyText();
	void  PasteText();
	float ParseExpr();

public:
	string   Text;
	wstring wText;
	wstring  Before;
	wstring  After;
	KeyEvent OnTextEntered;
	NotifyEvent OnChange;
	//
	bool  IntFilter;
	bool  FloatFilter;
	int   iMinVal, iMaxVal;
	float fMinVal, fMaxVal;
	int TrackWay;
	int TrackStep;
	//
	bool  TrackBar;
	//
	UIEdit();
	~UIEdit() {
		delete LeftElement;
		delete RightElement;
		delete MiddleElement;
		//
		delete SelectRectElement;
		delete StickElement;
	}
	//
	virtual void Init();
	virtual void Draw(int x, int y);
	virtual void KeyPressed(Uint16 key);
	virtual void TextEntered(Uint16 key);
	virtual void MouseDown (int x, int y, int Shift);
	virtual void MouseUp (int x, int y, int Shift);
	virtual void MouseMove (int x, int y, int Shift);
	virtual void DblClick  (int x, int y, int Shift);
	//
	virtual void Resized(int Width, int Height);
	//
	inline void MoveStickLeft();
	inline void MoveStickRight();
	inline void SetStickPos(int x, int y);
};

#endif
