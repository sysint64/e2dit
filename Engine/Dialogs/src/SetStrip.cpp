//* Author : Kabilin Andrey, 2013-2014 *//

#ifndef CPP_ANIMATIONS
#define CPP_ANIMATIONS

void SetStripDialog::Init() {
	Dialog = (UIDialog*) Data->Elements["animationsdialog"];
	//
	//Data->Elements["bok"]    ->OnClick = OnOKClick;
	//Data->Elements["bcancel"]->OnClick = OnCancelClick;
}

void SetStripDialog::Open() {
	Dialog->Left = (WindowWidth  >> 1) - (Dialog->Width  >> 1);
	Dialog->Top  = (WindowHeight >> 1) - (Dialog->Height >> 1);
	Dialog->Open();
}

void SetStripDialog::OnOKClick (UIElement *Sender) {
	setStripDialog->Dialog->Close();
}

void SetStripDialog::OnCancelClick (UIElement *Sender) {
	setStripDialog->Dialog->Close();
}

#endif
