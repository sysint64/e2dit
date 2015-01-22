//* Author : Kabilin Andrey, 2013-2014 *//

#ifndef H_ADDSPRITEFORM_2D
#define H_ADDSPRITEFORM_2D

struct Recti {
	int x1, y1, x2, y2;
};
//
class AddSpriteForm {
private:
	UIDialog  *Dialog;
	Texture2D *Texture;
	int		   x0, y0;
	bool	   isMouseDown;
	//
public:
	UIData  *Data;
	UIImage *AtlasImage;
	UIPanel	*BrowsePanel;
	UIEdit	*PosX;  UIEdit	*PosY;
	UIEdit	*SizeW; UIEdit	*SizeH;
	UIEdit	*NameField;

	UIColorPanel *GridImage;

	bool	 Snap;
	int		 SnapStep;
	int		 Zoom;
	Recti	 Select;

	//
	AddSpriteForm()  { Data = new UIData(); Zoom = 1; }
	~AddSpriteForm() { delete Data; }
	//
	// Events
	static void OnCancelClick (UIElement *Sender);
	static void OnFinishClick (UIElement *Sender);
	static void OnAddClick    (UIElement *Sender);
	static void OnChangeClick (UIElement *Sender);
	//
	void Init();
	void Step();
	void Render();
	void Open();
	//
	void KeyPressed (Uint16 key);
	void MouseDown  (int x, int y, int shift);
	void MouseUp    (int x, int y, int shift);
	void MouseMove  (int x, int y, int shift);
};

AddSpriteForm *addSpriteForm;

#endif
