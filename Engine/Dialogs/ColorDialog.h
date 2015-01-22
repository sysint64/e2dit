//* Author : Kabilin Andrey, 2013-2014 *//

#ifndef H_COLOR_DIALOG
#define H_COLOR_DIALOG
//
// Color Pallete Code
// HSV 0: H; 1: S; 2: B;
// RGB 3: R; 4: G; 5: B;
// Lab 6: L; 7: a; 8: b;
//
class ColorDialog {
private:
	BaseObject	*QuadElement;
	char		 ColorPallete;
	int			 ScrollX, ScrollY, ScrollZ;
	int			 MaxX   , MaxY   , MaxZ;
	//
	bool		 ColorLineClick;
	bool		 ColorPickerClick;
	//
	int			 PosX0, PosY0, PosZ0;
	int			 cx, cy;
	float		 Alpha;
	//
	glm::vec3	 HSB, HSB2, RGB;
	bool Clicked, InPicker;
	wstring		 LastHex;
	//
	List<glm::vec4*> *nLastColors;

public:
	UIData    *Data;
	UIDialog  *Dialog;
	UIPanel	  *ColorLine;
	UIPanel	  *ColorPicker;
	UIImage	  *Cursor;
	UIImage	  *CursorLine;
	VoidEvent Callback;
	//
	UIEdit	  *eHSB_H, *eHSB_S, *eHSB_B;
	UIEdit	  *eRGB_R, *eRGB_G, *eRGB_B;
	UIEdit	  *eAlpha, *eHex;
	//
	UIColorPanel *CurColor;
	UIColorPanel *NewColor;
	//
	UIColorPanel *LastColors[7];
	//
	glm::vec4  Color;
	//
	ColorDialog()  { InPicker = false; Data = new UIData(); Callback = 0; }
	~ColorDialog() { delete Data; }
	//
	void Init();
	void Open (glm::vec4 Color, VoidEvent Callback = 0);
	void Render();
	//
	// Events
	static void OnOKClick        (UIElement *Sender);
	static void OnCancelClick    (UIElement *Sender);
	static void OnCurColorClick  (UIElement *Sender);
	static void OnLastColorClick (UIElement *Sender);
	//
	static void OnHSB_HClick (UIElement *Sender, int x, int y, int shift);
	static void OnHSB_SClick (UIElement *Sender, int x, int y, int shift);
	static void OnHSB_BClick (UIElement *Sender, int x, int y, int shift);

	static void OnRGB_RClick (UIElement *Sender, int x, int y, int shift);
	static void OnRGB_GClick (UIElement *Sender, int x, int y, int shift);
	static void OnRGB_BClick (UIElement *Sender, int x, int y, int shift);
	//
	static void OnChangeHSB_H (UIElement *Sender);
	static void OnChangeHSB_S (UIElement *Sender);
	static void OnChangeHSB_B (UIElement *Sender);
	//
	static void OnChangeRGB_R (UIElement *Sender);
	static void OnChangeRGB_G (UIElement *Sender);
	static void OnChangeRGB_B (UIElement *Sender);
	static void OnChangeAlpha (UIElement *Sender);
	static void OnChangeHex   (UIElement *Sender);
	//
	void UI_UncheckHSB() {
		Data->Elements["bhsb_h"]->Checked = false;
		Data->Elements["bhsb_s"]->Checked = false;
		Data->Elements["bhsb_b"]->Checked = false;
	}

	void UI_UncheckRGB() {
		Data->Elements["brgb_r"]->Checked = false;
		Data->Elements["brgb_g"]->Checked = false;
		Data->Elements["brgb_b"]->Checked = false;
	}
	//
	void MouseDown  (int x, int y, int shift);
	void MouseUp    (int x, int y, int shift);
	void SetCursorZ  (const int Sz);
	void SetCursorXY (const int Sx, const int Sy);
	inline void SetCursorsPos();
	void UpdateHSBnRGB_Z();
	void UpdateHSBnRGB_XY();
	inline void UpdateEdits() {
		eHSB_H->wText = IntToWStr(HSB2[0]); eRGB_R->wText = IntToWStr(RGB[0]*255.f);
		eHSB_S->wText = IntToWStr(HSB2[1]); eRGB_G->wText = IntToWStr(RGB[1]*255.f);
		eHSB_B->wText = IntToWStr(HSB2[2]); eRGB_B->wText = IntToWStr(RGB[2]*255.f);
		//
		eHex->wText  = ByteToHex(RGB[0]*255.f);
		eHex->wText += ByteToHex(RGB[1]*255.f);
		eHex->wText += ByteToHex(RGB[2]*255.f);
	}
};

ColorDialog *colorDialog;

#endif
