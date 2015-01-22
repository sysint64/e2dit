#ifndef H_YESNODIALOG
#define H_YESNODIALOG 1
//
class YesNoDialog {
private:
	UIDialog  *Dialog;
	//
public:
	UIData  *Data;
	UIImage *ErrorIcon;
	UIImage *InfoIcon;
	UIImage *WarningIcon;

	//
	YesNoDialog()  { Data = new UIData(); }
	~YesNoDialog() { delete Data; }
	//
	// Events 
	static void OnYesClick (UIElement *Sender);
	static void OnNoClick  (UIElement *Sender);
	//
	void Init();
	void Open (wstring msg, int icon);
	//
	void KeyPressed (Uint16 key);
};

YesNoDialog *yesNoDialog;

#endif
