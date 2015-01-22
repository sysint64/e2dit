//* Author : Kabilin Andrey, 2013-2014 *//

#ifndef H_MATRIX_DIALOG
#define H_MATRIX_DIALOG
//
class MatrixDialog {
public:
	UIData   *Data;
	UIDialog *Dialog;

	//
	MatrixDialog()  { Data = new UIData(); }
	~MatrixDialog() { delete Data; }
	//
	void Init();
	void Open();
	//
	// Events
	static void OnOKClick     (UIElement *Sender);
	static void OnCancelClick (UIElement *Sender);
};

MatrixDialog *matrixDialog;

#endif
