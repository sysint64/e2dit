//* Author : Kabilin Andrey, 2013-2014 *//

#ifndef H_ADD_UNIFORM_DIALOG
#define H_ADD_UNIFORM_DIALOG
//
class AddUniformDialog {
public:
	UIData  *Data;
	UIDialog  *Dialog;

	//
	AddUniformDialog()  { Data = new UIData(); }
	~AddUniformDialog() { delete Data; }
	//
	void Init();
	void Open();
	//
	// Events
	static void OnOKClick     (UIElement *Sender);
	static void OnCancelClick (UIElement *Sender);
};

AddUniformDialog *addUniformDialog;

#endif
