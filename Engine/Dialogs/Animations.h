//* Author : Kabilin Andrey, 2013-2014 *//

#ifndef H_ANIMATIONS
#define H_ANIMATIONS
//
class AnimationsDialog {
public:
	UIData		*Data;
	UIDialog	*Dialog;

	//
	AnimationsDialog()  { Data = new UIData(); }
	~AnimationsDialog() { delete Data; }
	//
	void Init();
	void Open();
	//
	// Events
	static void OnOKClick     (UIElement *Sender);
	static void OnCancelClick (UIElement *Sender);
};

AnimationsDialog *animationsDialog;

#endif
