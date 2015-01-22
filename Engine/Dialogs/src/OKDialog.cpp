#ifndef CPP_OKDIALOG
#define CPP_OKDIALOG 1

//
void OKDialog::Init() {
	Dialog       = (UIDialog*) Data->Elements["okdialog"];
	ErrorIcon    =  (UIImage*) Data->Elements["ierror"];
	InfoIcon     =  (UIImage*) Data->Elements["iinfo"];
	WarningIcon  =  (UIImage*) Data->Elements["iwarning"];
	MessageLabel =  (UILabel*) Data->Elements["lmessage"];

	Data->Elements["bok"]->OnClick = OnOKClick;
}

void OKDialog::Open (wstring msg, int icon) {
	Dialog->Left = (WindowWidth  >> 1) - (Dialog->Width  >> 1);
	Dialog->Top  = (WindowHeight >> 1) - (Dialog->Height >> 1);
	Dialog->Open();

	ErrorIcon  ->Visible = false;
	InfoIcon   ->Visible = false;
	WarningIcon->Visible = false;

	switch (icon) {
		case iWarning : WarningIcon->Visible = true; break;
		case iError   : ErrorIcon  ->Visible = true; break;
		case iInfo    : InfoIcon   ->Visible = true; break;
	}

	MessageLabel->Caption = msg;
}

void OKDialog::OnOKClick (UIElement *Sender) {
	okDialog->Dialog->Close();
}

#endif
