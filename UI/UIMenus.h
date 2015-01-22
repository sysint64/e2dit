#ifndef H_UIMENUS
#define H_UIMENUS

// UIFlatButton - flat button
class UIListMenu;
class UIDropMenu;
class UIFlatButton : public UIButton {
public:
	UIListMenu *Menu;
	string ShortKey;
	bool InRect;
	//
	UIFlatButton (const bool List = false);
	~UIFlatButton() { if (Menu != 0) delete Menu; }
	//
	virtual void Init();
	virtual void Draw (int x, int y);
};

class UITallFlatButton : public UIFlatButton {
private:
	int   eiWidths [2];    int   eiHeights[2];
	float eWidths  [2];    float eHeights [2];
	float eOffsetsX[2];    float eOffsetsY[2];
	float TextColors2[12];
	//
	BaseObject	*QuadElement;
	//
public:
	float tWidth  , tHeight;
	float tOffsetX, tOffsetY;
	int   SpriteW, SpriteH;
	Texture2D *Texture;
	wstring Descr;
	//
	UITallFlatButton ();
	~UITallFlatButton();
	//
	virtual void Init();
	virtual void Draw (int x, int y);
};


// UIListMenu
class UIListMenu : public UIElement {
public:
	bool  Popup;
	float Delay;
	//
	int   iWidths [12];    int   iHeights[12];
	float Widths  [12];    float Heights [12];
	float OffsetsX[12];    float OffsetsY[12];
	//
	bool isPopup;
	int  SelItem;
	bool Clicked;
	
	//
	BaseObject	*DrawElements[11];
	UIFlatButton *LastCheck;
	//
public:
	Texture2D	*Icon;
	UIDropMenu  *DropMenu;
	~UIListMenu();
	UIListMenu(bool popup = false) : Popup(popup) { Delay = 0.0f; SelItem = -1; LastCheck = 0; mShift = mouseNone; DropMenu = 0; Clicked = false; }
	vector<UIFlatButton*> Items;
	int Count;
	bool CheckList;
	//
	inline void AddItem (UIFlatButton *Item) {
		//
		if (Item == 0) return;
		Item->Id = Items.size();
		Items.push_back(Item);
		Count = Items.size();
	}
	//
	inline void AddItem (UIFlatButton *Item, int pos) {
		//
		Item->Id = Items.size();
		Items.insert (Items.begin()+pos, Item);
		Count = Items.size();
		//
		for (int i = 0; i < Items.size(); i++) {
			Items[i]->Id = i;
		}
	}
	//
	inline void CheckItem (UIFlatButton *Item) {
		Item->Checked = true;
		Item->Enter = true;
		//
		if (LastCheck != 0) {
			LastCheck->Checked = false;
			LastCheck->Enter = false;
			LastCheck = Item;
		} else LastCheck = Item;
	}
	//
	void Clear();
	void Deselect() {
		LastCheck = 0;

		for (int i = 0; i < Items.size(); i++) {
			Items[i]->Checked = false;
			Items[i]->Enter   = false;
		}
	}

	virtual void Init();
	virtual void Draw(int x, int y);
	//
	virtual void MouseDown (int x, int y, int Shift);
	virtual void MouseMove (int x, int y, int Shift);
	virtual void MouseUp   (int x, int y, int Shift);
	virtual void DblClick  (int x, int y, int Shift);
	//
	virtual void KeyPressed (Uint16 key);
	virtual void Progress ();
};

//
class UIIconedMenuItem : public UIButton {
public:
	int	id;
	wstring	Name;
	wstring	Descr;
};

class UIIconedMenu : public UIElement {
};

// Drop Menu (UI Element)
class UIDropMenu : public UIButton {
private:
	int   miWidth;    int   miHeight;
	float mWidth;     float mHeight;
	float mOffsetX;   float mOffsetY;
	//
	BaseObject	*ArrowElement;
	//
public:
	bool isMenu;
	UIListMenu *Menu;
	int	    SelItem;
	bool Droped;
	bool Checked;
	bool ShowText;
	
	UIDropMenu (bool menu = false) : isMenu(menu), ShowText(true) { SelItem = 0; }
	~UIDropMenu() { delete ArrowElement; delete Menu; }
	//
	virtual void Init();
	virtual void Draw(int x, int y);
	//
	virtual void MouseDown (int x, int y, int Shift);
	virtual void MouseMove (int x, int y, int Shift);
	virtual void MouseUp   (int x, int y, int Shift);
	//
	virtual void KeyPressed (Uint16 key);
	virtual void Progress ();
};

#endif
