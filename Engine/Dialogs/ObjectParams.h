//* Author : Kabilin Andrey, 2013-2014 *//

#ifndef H_OBJECT_PARAMS_DIALOG
#define H_OBJECT_PARAMS_DIALOG
//
class ObjectParamsDialog {
public:
	UIData  *Data;
	UIDialog  *Dialog;

	//
	ObjectParamsDialog()  { Data = new UIData(); }
	~ObjectParamsDialog() { delete Data; }
	//
	void Init();
	void Open();
	//
	// Events
	static void OnOKClick     (UIElement *Sender);
	static void OnCancelClick (UIElement *Sender);
};

ObjectParamsDialog *objectParamsDialog;

#endif
