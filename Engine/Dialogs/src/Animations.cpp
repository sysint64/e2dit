//* Author : Kabilin Andrey, 2013-2014 *//

#ifndef CPP_ANIMATIONS
#define CPP_ANIMATIONS

void AnimationsDialog::Init() {
	Dialog = (UIDialog*) Data->Elements["animationsdialog"];
	//
	Data->Elements["bok"]    ->OnClick = OnOKClick;
	Data->Elements["bcancel"]->OnClick = OnCancelClick;
}

void AnimationsDialog::Open() {
	Dialog->Left = (WindowWidth  >> 1) - (Dialog->Width  >> 1);
	Dialog->Top  = (WindowHeight >> 1) - (Dialog->Height >> 1);
	Dialog->Open();
}

void AnimationsDialog::OnOKClick (UIElement *Sender) {
	animationsDialog->Dialog->Close();
}

void AnimationsDialog::OnCancelClick (UIElement *Sender) {
	animationsDialog->Dialog->Close();
}

#endif
