#ifndef H_OKDIALOG
#define H_OKDIALOG 1
//
#define iWarning	0
#define iError		1
#define iInfo		2

class OKDialog {
private:
	UIDialog  *Dialog;
	//
public:
	UIData  *Data;
	UIImage *ErrorIcon;
	UIImage *InfoIcon;
	UIImage *WarningIcon;
	UILabel *MessageLabel;

	//
	OKDialog()  { Data = new UIData(); }
	~OKDialog() { delete Data; }
	//
	// Events
	static void OnOKClick (UIElement *Sender);
	//
	void Init();
	void Open (wstring msg, int icon);
	//
	void KeyPressed (Uint16 key);
};

OKDialog *okDialog;

#endif
