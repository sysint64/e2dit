#ifndef CPP_YESNODIALOG
#define CPP_YESNODIALOG 1

//
void YesNoDialog::Init() {
	Dialog = (UIDialog*) Data->Elements["setpolygondialog"];
}

void YesNoDialog::Open (wstring msg, int icon) {
	Dialog->Left = (WindowWidth  >> 1) - (Dialog->Width  >> 1);
	Dialog->Top  = (WindowHeight >> 1) - (Dialog->Height >> 1);
	Dialog->Open();
}

#endif
