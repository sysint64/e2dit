//* Author : Kabilin Andrey, 2013-2014 *//

#ifndef H_ADD_VAR_DIALOG
#define H_ADD_VAR_DIALOG
//
class AddVarDialog {
private:
	wstring		SelectText;
	
public:
	UIData		*Data;
	UIDialog	*Dialog;
	UIDropMenu	*TypeList;
	UIEdit		*Name;
	UICheckBox	*Global;
	//
	AddVarDialog()  { Data = new UIData(); }
	~AddVarDialog() { delete Data; }
	//
	void Init();
	void Open();
	//
	// Events
	static void OnOKClick     (UIElement *Sender);
	static void OnCancelClick (UIElement *Sender);
};

AddVarDialog *addVarDialog;

#endif
