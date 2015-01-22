//* Author : Kabilin Andrey, 2013-2014 *//

#ifndef CPP_MATRIX_DIALOG
#define CPP_MATRIX_DIALOG

void MatrixDialog::Init() {
	Dialog = (UIDialog*) Data->Elements["adduniformdialog"];
	//
	Data->Elements["bok"]    ->OnClick = OnOKClick;
	Data->Elements["bcancel"]->OnClick = OnCancelClick;
}

void MatrixDialog::Open() {
	Dialog->Left = (WindowWidth  >> 1) - (Dialog->Width  >> 1);
	Dialog->Top  = (WindowHeight >> 1) - (Dialog->Height >> 1);
	Dialog->Open();
}

void MatrixDialog::OnOKClick (UIElement *Sender) {
	matrixDialog->Dialog->Close();
}

void MatrixDialog::OnCancelClick (UIElement *Sender) {
	matrixDialog->Dialog->Close();
}

#endif
