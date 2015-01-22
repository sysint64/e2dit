//* Author : Kabilin Andrey, 2013-2014 *//

#ifndef H_USER_PREFERENCES_2D
#define H_USER_PREFERENCES_2D

class PreferencesDialog {
public:
	UIData  *Data;
	UIDialog  *Dialog;

	//
	PreferencesDialog()  { Data = new UIData(); }
	~PreferencesDialog() { delete Data; }
	//
	void Init();
	void Open();
	//
	// Events
	static void OnOKClick     (UIElement *Sender);
	static void OnCancelClick (UIElement *Sender);
};

PreferencesDialog *preferencesDialog;

#endif
