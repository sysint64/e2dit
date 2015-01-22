//* Author : Kabilin Andrey, 2013-2014 *//

#ifndef CPP_USER_PREFERENCES_2D
#define CPP_USER_PREFERENCES_2D

void PreferencesDialog::Init() {
	Dialog = (UIDialog*) Data->Elements["preferencesdialog"];
	//
	Data->Elements["bok"]    ->OnClick = OnOKClick;
	Data->Elements["bcancel"]->OnClick = OnCancelClick;
}

void PreferencesDialog::Open() {
	Dialog->Left = (WindowWidth  >> 1) - (Dialog->Width  >> 1);
	Dialog->Top  = (WindowHeight >> 1) - (Dialog->Height >> 1);
	Dialog->Open();
}

void PreferencesDialog::OnOKClick (UIElement *Sender) {
	preferencesDialog->Dialog->Close();
}

void PreferencesDialog::OnCancelClick (UIElement *Sender) {
	preferencesDialog->Dialog->mShift = mouseNone;
	preferencesDialog->Dialog->Close();
}

#endif
