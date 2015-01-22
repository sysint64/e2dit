//* Author : Kabilin Andrey, 2013-2014 *//

#ifndef CPP_RETOPOLOGY_DIALOG
#define CPP_RETOPOLOGY_DIALOG

//
void RetopologyDialog::Init() {
	Dialog    =   (UIDialog*) Data->Elements["retopologydialog"];
	ItemsList = (UIListMenu*) Data->Elements["lmpolys"];

	Data->Elements["bcancel"]->OnClick = OnCancelClick;
	Data->Elements["bok"]    ->OnClick = OnOKClick;
}

void RetopologyDialog::Open (BaseGameObject *Obj) {
	Dialog->Left = (WindowWidth  >> 1) - (Dialog->Width  >> 1);
	Dialog->Top  = (WindowHeight >> 1) - (Dialog->Height >> 1);

	Dialog->Open();
	RetopologyObj = Obj;

	// Init List
	ItemsList->Clear();
	SelectedPolyId = -1;

	UIFlatButton *Item = new UIFlatButton (false);

	Item->Caption = L"Main";
	Item->ShowIcon = true;
	Item->IconOffset [0] = 0;
	Item->IconOffset [1] = 3;

	ItemsList->AddItem (Item);
	Item->Id = -1;
	Item->Checked = true;

	// Events
	Item->OnClick = SelectPoly;

	//
	for (int i = 0; i < RetopologyObj->PolysList.size(); i++) {
		UIFlatButton *Item = new UIFlatButton (false);

		Item->Caption = RetopologyObj->PolysList[i]->Name;
		Item->ShowIcon = true;
		Item->IconOffset [0] = 5;
		Item->IconOffset [1] = 3;

		ItemsList->AddItem (Item);
		Item->Id = i;

		// Events
		Item->OnClick = SelectPoly;		
	}

	ItemsList->Init();
}

void RetopologyDialog::SelectPoly (UIElement *Sender) {
	retopologyDialog->SelectedPolyId = Sender->Id;
}

// Events
//
void RetopologyDialog::OnCancelClick (UIElement *Sender) {
	retopologyDialog->Dialog->Close();
}

void RetopologyDialog::OnOKClick (UIElement *Sender) {
	if (retopologyDialog->SelectedPolyId == -1) {
		retopologyDialog->RetopologyObj->CopyFromDataRender();
		retopologyDialog->RetopologyObj->Triangulate5();
	} else {
		retopologyDialog->RetopologyObj->CopyFromDataPolygon (retopologyDialog->RetopologyObj->PolysList[retopologyDialog->SelectedPolyId]);
		retopologyDialog->RetopologyObj->Triangulate5();
	}

	retopologyDialog->RetopologyObj->BuildDataRender();
	retopologyDialog->Dialog->Close();
}

#endif
