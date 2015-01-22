//* Author : Kabilin Andrey, 2013-2014 *//

#ifndef H_RETOPOLOGY_DIALOG
#define H_RETOPOLOGY_DIALOG

//
class RetopologyDialog {
private:
	UIDialog  *Dialog;
	int SelectedPolyId;
	//
public:
	UIData     *Data;
	UIListMenu *ItemsList;
	BaseGameObject *RetopologyObj;

	//
	RetopologyDialog()  { Data = new UIData(); }
	~RetopologyDialog() { delete Data; }
	//
	// Events
	static void OnCancelClick (UIElement *Sender);
	static void OnOKClick     (UIElement *Sender);
	static void SelectPoly    (UIElement *Sender);
	//
	void Init();
	void Open (BaseGameObject *Obj);
};

RetopologyDialog *retopologyDialog;

#endif
