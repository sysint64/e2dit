#ifndef CPP_GROUPES
#define CPP_GROUPES

template <class ItemType>
void BaseGroupes<ItemType>::UpdateGroup() {
	ItemsList->Clear();

	for (int i = 0; i < Data[CurId->wText].Items.size(); i++) {
		UIFlatButton *Item = new UIFlatButton (false);

		Item->Caption = Data[CurId->wText].Items[i].Name;
		//Item->OnKeyPressed = EditItemName;
		//Item->OnMouseDown = SelectItem;
		//Item->OnDblClick = EditItemNameDbl;

		Item->ShowIcon = true;
		Item->IconOffset [0] = 5;
		Item->IconOffset [1] = 3;

		ItemsList->AddItem (Item);
		CurItemId = Item->Id;
	}

	ItemsList->Init();
}

template <class ItemType>
void BaseGroupes<ItemType>::AddGroup() {
	if (Data.count(CurId->wText) > 0) {
		CurId->wText = LastId;
		return;
	}

	BaseGroup<ItemType> NewGroup;
	NewGroup.Name = CurId->wText;
	CurItemId = 0;
	Data[CurId->wText] = NewGroup;

	//
	UIFlatButton *Item = new UIFlatButton (true);
	Item->Caption = CurId->wText;
	Item->OnClick = OnSetGroupEvent;
	LastId = CurId->wText;

	GroupesList->AddItem(Item);
	GroupesList->Init();

	UpdateGroup();
	DeleteButton->Enabled = true;
}

template <class ItemType>
void BaseGroupes<ItemType>::SetGroup(UIElement *Sender) {
	LastId = CurId->wText;
	CurId->wText = ((UIButton*) Sender)->Caption;
	CurItemId = 0;
	UpdateGroup();
}

template <class ItemType>
void BaseGroupes<ItemType>::ChangeName() {
	if (Data.count(CurId->wText) > 0) {
		CurItemId = 0;
		UpdateGroup();
		return;
	}

	Data[CurId->wText] = Data[LastId];
	Data.erase(LastId);
	//
	for (int i = 0; i < GroupesList->Items.size(); i++) {
		if (GroupesList->Items[i]->Caption == LastId) {
			GroupesList->Items[i]->Caption = CurId->wText;
			break;
		}
	}

	LastId = CurId->wText;
}

template <class ItemType>
void BaseGroupes<ItemType>::DeleteGroup() {
	if (Data.size() == 1)
		return;

	GroupesList->Clear();
	Data.erase(CurId->wText);

	for (typename GroupesData::const_iterator it = Data.begin(); it != Data.end(); it++) {
		UIFlatButton *Item = new UIFlatButton (true);
		Item->Caption = it->first;
		Item->OnClick = OnSetGroupEvent;

		GroupesList->AddItem(Item);
	}

	GroupesList->Init();
	CurId->wText = LastId;

	if (Data.size() <= 1)
		DeleteButton->Enabled = false;

	CurItemId = 0;
	UpdateGroup();
}

#endif
