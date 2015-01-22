//* Author : Kabilin Andrey, 2013-2014 *//

#ifndef CPP_ADD_UNIFORM_DIALOG
#define CPP_ADD_UNIFORM_DIALOG

void AddUniformDialog::Init() {
	Dialog = (UIDialog*) Data->Elements["adduniformdialog"];
	//
	Data->Elements["bok"]    ->OnClick = OnOKClick;
	Data->Elements["bcancel"]->OnClick = OnCancelClick;
}

void AddUniformDialog::Open() {
	Dialog->Left = (WindowWidth  >> 1) - (Dialog->Width  >> 1);
	Dialog->Top  = (WindowHeight >> 1) - (Dialog->Height >> 1);
	Dialog->Open();
}

void AddUniformDialog::OnOKClick (UIElement *Sender) {
	addUniformDialog->Dialog->Close();
}

void AddUniformDialog::OnCancelClick (UIElement *Sender) {
	addUniformDialog->Dialog->Close();
}

#endif
