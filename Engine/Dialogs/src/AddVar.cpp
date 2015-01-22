//* Author : Kabilin Andrey, 2013-2014 *//

#ifndef CPP_ADD_VAR_DIALOG
#define CPP_ADD_VAR_DIALOG

void AddVarDialog::Init() {
	Dialog		=   (UIDialog*) Data->Elements["addvardialog"];
	TypeList	= (UIDropMenu*) Data->Elements["dmtypes"];
	Name		=     (UIEdit*) Data->Elements["ename"];
	Global		= (UICheckBox*) Data->Elements["cbglobal"];
	//
	SelectText  = TypeList->Caption;
	//
	Data->Elements["bok"]    ->OnClick = OnOKClick;
	Data->Elements["bcancel"]->OnClick = OnCancelClick;
}

void AddVarDialog::Open() {
	// Init
	TypeList->Caption = SelectText;
	Global->Checked = false;
	Name->wText = L"";
	//
	Dialog->Left = (WindowWidth  >> 1) - (Dialog->Width  >> 1);
	Dialog->Top  = (WindowHeight >> 1) - (Dialog->Height >> 1);
	Dialog->Open();
}

void AddVarDialog::OnOKClick (UIElement *Sender) {
	addVarDialog->Dialog->Close();
}

void AddVarDialog::OnCancelClick (UIElement *Sender) {
	addVarDialog->Dialog->Close();
}

#endif
