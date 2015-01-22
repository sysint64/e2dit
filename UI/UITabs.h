#ifndef H_UITABS
#define H_UITABS

class UITabSplit   : public UIElement {};
class UITabElement : public UIElement {
private:
	BaseObject *QuadElement;
	float TextColor[3];

public:
	bool CanCheck;
	wstring Caption;

	 UITabElement() { Checked = false; CanCheck = false; }
	~UITabElement() { delete QuadElement; }
	//
	virtual void Init();
	virtual void Draw(int x, int y);
	//UITabButton
};

class UITab  : public UIButton {
public:
	bool Active;
	vector<UIElement*> Elements;
	
	UITab() {
		Active = false;

		LeavParam  = "tabbuttonleav";
		EnterParam = "tabbuttonenter";
		ClickParam = "tabbuttonclick";
	}

	inline void AddElement (UIElement *Element) {
		if (Element == 0) return;
		Elements.push_back(Element);
	}
};

class UITabs : public UIElement {
private:
	int   cx, cy;
	bool  Clicked;

	int    shHeight, TabOffset;
	int    iWidths[2];  int   iHeights[2];
	float   Widths[2];  float  Heights[2];
	float OffsetsX[2];  float OffsetsY[2];

	BaseObject *Panel;
	BaseObject *Split;
	UITab	   *ActiveTab;

public:
	vector<UITab*> Tabs;
	int			   TabsOffset;

	 UITabs() { Align = alTop; Clicked = false; mShift = mouseNone; ActiveTab = 0; TabsOffset = 5; }
	~UITabs() { delete Panel; delete Split; }

	virtual void Init();
	virtual void Draw (int x, int y);
			void DrawTabElements (int id, int x, int y);
	inline  void DrawFront(int x, int y);

	virtual void DblClick  (int x, int y, int Shift);
	virtual void MouseDown (int x, int y, int Shift);
	virtual void MouseMove (int x, int y, int Shift);
	virtual void MouseUp   (int x, int y, int Shift);
	//
	virtual void KeyPressed (Uint16 key);
	virtual void TextEntered (Uint16 key);
	//
	inline void AddTab (UITab *Tab) {
		if (Tab == 0) return;
		Tabs.push_back(Tab);
	}
};

#endif
