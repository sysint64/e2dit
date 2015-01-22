#ifndef H_GROUPED
#define H_GROUPED

// UIGroupedElements
class UIGroupedElements : public UIButton {
private:
	float Splits [4]; 
	int   iSplits[4];
	int	  GenWidth, GW0;
	int	  CountFS; // Count Elements with FixSize = true
	bool  Clicked;
	//
	//void RenderButton(const int i, const int align, const int x);
	BaseObject	*SplitElement;
	void RenderSplit(const int x);
	
public:
	~UIGroupedElements();
	UIGroupedElements() { Count = 0;}
	vector<UIElement*> Elements;
	int	   Count;
	//
	virtual void Init();
	virtual void Draw(int x, int y);
	//
	virtual void DblClick  (int x, int y, int Shift);
	virtual void MouseDown (int x, int y, int Shift);
	virtual void MouseMove (int x, int y, int Shift);
	virtual void MouseUp   (int x, int y, int Shift);
	//
	virtual void KeyPressed (Uint16 key);
	virtual void TextEntered (Uint16 key);
	//
	inline void AddElement(UIElement *Element) {
		if (Element == 0) return;
		Elements.push_back(Element);
		Count = Elements.size();
	}
};

//

class UIPanel : public UIElement {
private:
	BaseObject	*QuadElement;
	BaseObject	*ScrollBg[6];
	BaseObject	*ScrollBtn[6];
	BaseObject	*Split;
	BaseObject	*Arrow;
	//
	int    iWidths[16];  int   iHeights[16];
	float   Widths[16];  float  Heights[16];
	float OffsetsX[16];  float OffsetsY[16];
	float TextColor [3]; float ShadowColor [3];
	float TextOffset[2]; float ShadowOffset[2];
	bool  TextShadow;
	//
	int		MaxX, MaxY;
	int		hbOffset, vbOffset;
	int		lhbOffset, lvbOffset;
	int		hsOffset, vsOffset;
	int		splitx, splity;
	int		splitw, splith;
	int		CurSize;
	int		PanelSize;
	bool	Clicked;
	bool	ScrollClicked;

	void UpdateAlign(int *x, int *y);

protected:
	bool	SHClick, VHClick;
	bool	SplitClick;
	int		ElemsOffset;
	int		cx, cy;

public:
	int		hbSize, hbMin, hbMax;
	int		vbSize, vbMin, vbMax;
	
	bool DrawQuad;
	int ScrollX;
	int ScrollY;
	bool ShowScroll;
	bool AllowResize;
	int MaxSize, MinSize;
	bool CanHide;
	int TextAlign;
	bool UseInColor;
	bool UseActionColor;
	int ScrollOffset;
	//
	float Colors[4];   // RGBA
	float ColorsIn[4]; // RGBA
	float ActionColor[4];
	//
	bool isOpened;
	wstring Caption;
	UIPanel *Join;
	UIElement *TabJoin;
	//
	UIPanel();
	~UIPanel();
	//
	vector<UIElement*> Elements;
	//
	virtual void Draw (int x, int y);
	virtual void Init();
	//inline  void Split (int x, int y);
	//
	inline void AddElement(UIElement *Element) {
		Element->Parent = this;
		Element->Id = Elements.size();
		Elements.push_back(Element);
	}
	//
	//Events
	virtual void DblClick  (int x, int y, int Shift);
	//
	virtual void MouseDown (int x, int y, int Shift);
	virtual void MouseMove (int x, int y, int Shift);
	virtual void MouseUp   (int x, int y, int Shift);
	//
	virtual void KeyDown (int key);
	virtual void Resized     (int Width, int Height);
	virtual void KeyPressed  (Uint16 key);
	virtual void TextEntered (Uint16 key);
	virtual void Progress();
	//
	inline void AddScrollXByPX (int pxval) {
		hbOffset = hbOffset+pxval;
		clamp(&hbOffset, 0, hbMax-hbSize);
		//
		int py = (hbOffset*100)/hbMax;
		hsOffset = (MaxX*py)/100;
	}

	inline int GetScrollYByPX() {
		return vsOffset;
	}

	inline int SetScrollY(int val) {
		vbOffset = val;
		clamp(&vbOffset, 0, vbMax-vbSize);

		int py = (vbOffset*100)/vbMax;
		vsOffset = (MaxY*py)/100;
	}

	inline int SetScrollX(int val) {
		hbOffset = val;
		clamp(&hbOffset, 0, hbMax-hbSize);

		int py = (hbOffset*100)/hbMax;
		hsOffset = (MaxX*py)/100;
	}
};

//
class UIDialog : public UIPanel {
private:
	BaseObject	*FormElements[9];
	//
	int    iWidths[9];  int   iHeights[9];
	float   Widths[9];  float  Heights[9];
	float OffsetsX[9];  float OffsetsY[9];
	float TextColor [3]; float ShadowColor [3];
	float TextOffset[2]; float ShadowOffset[2];
	bool  TextShadow;
	bool  Clicked;
	int	  tLeft, tTop;
	//
public:
	//	
	UIDialog () {
		DrawQuad = false;
		Caption  = L"";
		Clicked  = false;
		isOpened = false;
	}

	~UIDialog() {
		for (int i = 0; i < 9; i++)
			delete FormElements[i];
	}
	//
	virtual void MouseDown (int x, int y, int Shift);
	virtual void MouseUp (int x, int y, int Shift);
	virtual void Draw (int x, int y);
	virtual void Init();
	virtual void Progress();
	void Open()  {
		isOpened = true;
		DialogOpened = isOpened;
		//mShift = mouseNone;
		//SHClick = false; VHClick = false;
		//SplitClick = false;

		mShift = mouseNone;
		SHClick    = false;
		VHClick    = false;
		SplitClick = false;
		LockCursor = false;
		Clicked = false;

		for (int i = 0; i < Elements.size(); i++) {
			Elements[i]->MouseUp   (-1, -1, mouseNone);
			Elements[i]->MouseMove (-1, -1, mouseNone);
		}
	}

	void Close() { isOpened = false; DialogOpened = isOpened; mShift = mouseNone;
	}
};

#endif
