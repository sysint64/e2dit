#ifndef H_GROUPES
#define H_GROUPES

// Item of group
struct SpriteGroupItem {
	wstring		 Name;
	int			 TexSize[2];
	float		 TexOffset[2];
	Texture2D	*Texture;
};

struct PolygonGroupItem {
	wstring			Name;
	PolygonObject  *Data;
};

struct PointGroupItem {
	wstring		 Name;
	PointObject *Data;
};

struct LightGroupItem {
	wstring		 Name;
	LightObject *Data;
};


// Groupes

template <class ItemType>
struct BaseGroup {
public:
	wstring Name;
	vector<ItemType> Items;
};
//
template <class ItemType>
class BaseGroupes {
private:
	wstring		 LastId;
	
public:
	int			 CurItemId;
	UIListMenu	*GroupesList;
	UIListMenu	*ItemsList;
	UIEdit		*CurId;
	UIButton	*DeleteButton;
	NotifyEvent  OnSetGroupEvent;

	typedef BaseGroup<ItemType>      GroupItem;
	typedef map <wstring, GroupItem> GroupesData;
	GroupesData Data;

	virtual void ChangeName();
	virtual void SetGroup(UIElement *Sender);
	virtual void AddGroup();
	virtual void DeleteGroup();
	virtual void UpdateGroup();

	inline void InsertToGroup(ItemType Item) {
		Data[CurId->wText].Items.push_back(Item);
	}

	BaseGroupes(UIListMenu *gList, UIListMenu *iList, UIEdit *eName, UIButton *bDelete)
		: GroupesList(gList), ItemsList(iList), CurId(eName), DeleteButton(bDelete)
	{
		BaseGroup<ItemType> MainGroup;
		CurId->wText = L"Main";
		CurId->Focused = false;
		MainGroup.Name = CurId->wText;
		Data[CurId->wText] = MainGroup;
		LastId = CurId->wText;
	}
};

class SpriteGroupesT : public BaseGroup<SpriteGroupItem> {
public:
};

#include "src/Groupes.cpp"
#endif
