//* Author : Kabilin Andrey, 2013-2014 *//

#ifndef H_SET_STIP
#define H_SET_STIP
//
class SetStripDialog {
public:
	UIData		*Data;
	UIDialog	*Dialog;

	//
	SetStripDialog()  { Data = new UIData(); }
	~SetStripDialog() { delete Data; }
	//
	void Init();
	void Open();
	//
	// Events
	static void OnOKClick     (UIElement *Sender);
	static void OnCancelClick (UIElement *Sender);
};

SetStripDialog *setStripDialog;

#endif
