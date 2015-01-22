//* Author : Kabilin Andrey, 2013-2014 *//

#ifndef CPP_OBJECT_PARAMS_DIALOG
#define CPP_OBJECT_PARAMS_DIALOG

void ObjectParamsDialog::Init() {
	Dialog = (UIDialog*) Data->Elements["adduniformdialog"];
	//
	Data->Elements["bok"]    ->OnClick = OnOKClick;
	Data->Elements["bcancel"]->OnClick = OnCancelClick;
}

void ObjectParamsDialog::Open() {
	Dialog->Left = (WindowWidth  >> 1) - (Dialog->Width  >> 1);
	Dialog->Top  = (WindowHeight >> 1) - (Dialog->Height >> 1);
	Dialog->Open();
}

void ObjectParamsDialog::OnOKClick (UIElement *Sender) {
	objectParamsDialog->Dialog->Close();
}

void ObjectParamsDialog::OnCancelClick (UIElement *Sender) {
	objectParamsDialog->Dialog->Close();
}

#endif
