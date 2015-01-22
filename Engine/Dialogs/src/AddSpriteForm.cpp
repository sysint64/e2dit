//* Author : Kabilin Andrey, 2013-2014 *//

#ifndef CPP_ADDSPRITEFORM_2D
#define CPP_ADDSPRITEFORM_2D

//
void AddSpriteForm::Init() {
	Dialog		=     (UIDialog*) Data->Elements["addspritedialog"];
	AtlasImage	=      (UIImage*) Data->Elements["texture"];
	BrowsePanel =      (UIPanel*) Data->Elements["pbrowse"];
	GridImage   = (UIColorPanel*) Data->Elements["transparent"];

	PosX		=   (UIEdit*) Data->Elements["eposx"];
	PosY		=   (UIEdit*) Data->Elements["eposy"];
	SizeW		=   (UIEdit*) Data->Elements["esizew"];
	SizeH		=   (UIEdit*) Data->Elements["esizeh"];
	NameField	=	(UIEdit*) Data->Elements["ename"];
	//
	Data->Elements["bcancel"]->OnClick = OnCancelClick;
	//Data->Elements["bfinish"]->OnClick = OnFinishClick;
	Data->Elements["bchange"]->OnClick = OnChangeClick;
	Data->Elements["badd"]   ->OnClick = OnAddClick;
}

void AddSpriteForm::Step() {
}

void AddSpriteForm::Render() {
	//
	if (!Dialog->isOpened) return;
	x0 = BrowsePanel->AbsoluteLeft;
	y0 = BrowsePanel->AbsoluteTop;

	// Draw Grid
	glBegin2D();
	glDisable (GL_LINE_SMOOTH);
	
	BrowsePanel->Manager->PushScissor (x0, WindowHeight-BrowsePanel->Height-y0+BrowsePanel->ScrollHeight,
										   BrowsePanel->Width-BrowsePanel->ScrollWidth,
										   BrowsePanel->Height-BrowsePanel->ScrollHeight);

	glLineWidth(0.5);
	//glColor3f (61.0f/255.0f, 61.0f/255.0f, 61.0f/255.0f);
	glColor4f (0, 0, 0, 0.3f);

	glEnable (GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);
	glColorMask(false, false, false, false);

	glBegin (GL_QUADS);
		glVertex2i (x0+Select.x1-BrowsePanel->ScrollX, WindowHeight-y0-Select.y1+BrowsePanel->ScrollY);
		glVertex2i (x0+Select.x2-BrowsePanel->ScrollX, WindowHeight-y0-Select.y1+BrowsePanel->ScrollY);
		glVertex2i (x0+Select.x2-BrowsePanel->ScrollX, WindowHeight-y0-Select.y2+BrowsePanel->ScrollY);
		glVertex2i (x0+Select.x1-BrowsePanel->ScrollX, WindowHeight-y0-Select.y2+BrowsePanel->ScrollY);
	glEnd();

	glBegin (GL_QUADS);
		glVertex2i (AtlasImage->AbsoluteLeft, WindowHeight-AtlasImage->AbsoluteTop);
		glVertex2i (AtlasImage->AbsoluteLeft+AtlasImage->Width, WindowHeight-AtlasImage->AbsoluteTop);
		glVertex2i (AtlasImage->AbsoluteLeft+AtlasImage->Width, WindowHeight-AtlasImage->AbsoluteTop-AtlasImage->Height);
		glVertex2i (AtlasImage->AbsoluteLeft, WindowHeight-AtlasImage->AbsoluteTop-AtlasImage->Height);
	glEnd();

	glStencilFunc(GL_NOTEQUAL, 0, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
	glColorMask(true, true, true, true);

	glBegin (GL_QUADS);
		glVertex2i (AtlasImage->AbsoluteLeft, WindowHeight-AtlasImage->AbsoluteTop);
		glVertex2i (AtlasImage->AbsoluteLeft+AtlasImage->Width, WindowHeight-AtlasImage->AbsoluteTop);
		glVertex2i (AtlasImage->AbsoluteLeft+AtlasImage->Width, WindowHeight-AtlasImage->AbsoluteTop-AtlasImage->Height);
		glVertex2i (AtlasImage->AbsoluteLeft, WindowHeight-AtlasImage->AbsoluteTop-AtlasImage->Height);
	glEnd();

	glDisable (GL_STENCIL_TEST);

	glColor3fv (&MapEditor->RoomBorderColor[0]);
	glBegin (GL_LINE_LOOP);
		glVertex2i (AtlasImage->AbsoluteLeft, WindowHeight-AtlasImage->AbsoluteTop);
		glVertex2i (AtlasImage->AbsoluteLeft+AtlasImage->Width, WindowHeight-AtlasImage->AbsoluteTop);
		glVertex2i (AtlasImage->AbsoluteLeft+AtlasImage->Width, WindowHeight-AtlasImage->AbsoluteTop-AtlasImage->Height);
		glVertex2i (AtlasImage->AbsoluteLeft, WindowHeight-AtlasImage->AbsoluteTop-AtlasImage->Height);
	glEnd();

	glColor3fv (&MapEditor->SelectColor[0]);
	glBegin (GL_LINE_LOOP);
		glVertex2i (x0+Select.x1-BrowsePanel->ScrollX, WindowHeight-y0-Select.y1+BrowsePanel->ScrollY);
		glVertex2i (x0+Select.x2-BrowsePanel->ScrollX, WindowHeight-y0-Select.y1+BrowsePanel->ScrollY);
		glVertex2i (x0+Select.x2-BrowsePanel->ScrollX, WindowHeight-y0-Select.y2+BrowsePanel->ScrollY);
		glVertex2i (x0+Select.x1-BrowsePanel->ScrollX, WindowHeight-y0-Select.y2+BrowsePanel->ScrollY);
	glEnd();

	BrowsePanel->Manager->PopScissor();
	glEnd2D();
}

void AddSpriteForm::Open() {
	AtlasImage->Texture->LoadFromFile((finder->Path+DirSlash+finder->FileName).c_str());
	AtlasImage->Texture->Update();
	AtlasImage->Width  = AtlasImage->Texture->Width;
	AtlasImage->Height = AtlasImage->Texture->Height;
	//
	GridImage->Width  = AtlasImage->Width;
	GridImage->Height = AtlasImage->Height;
	GridImage->Update();

	Dialog->Width  = clamp (AtlasImage->Width+275, 520, WindowWidth-60);
	Dialog->Height = clamp (AtlasImage->Height+60, 300, WindowHeight-60);

	Dialog->Left = (WindowWidth  >> 1) - (Dialog->Width  >> 1);
	Dialog->Top  = (WindowHeight >> 1) - (Dialog->Height >> 1);
	Dialog->Open();

	//
	Select.x1 = AtlasImage->Left;
	Select.x2 = AtlasImage->Left+AtlasImage->Width;

	Select.y1 = AtlasImage->Top;
	Select.y2 = AtlasImage->Top+AtlasImage->Height;
	//
	NameField->wText = finder->NameField->wText;
	isMouseDown = false; Texture = 0;
}

// Form Events ------------------>

void AddSpriteForm::OnCancelClick (UIElement *Sender) {
	addSpriteForm->Dialog->Close();
	addSpriteForm->Texture = 0;
}

void AddSpriteForm::OnFinishClick (UIElement *Sender) {
	addSpriteForm->Dialog->Close();
	addSpriteForm->Texture = 0;
}

void AddSpriteForm::OnAddClick (UIElement *Sender) {
	bool UpdateTexture = false;

	if (addSpriteForm->Texture == 0) {
		addSpriteForm->Texture = new Texture2D();
		addSpriteForm->Texture->LoadFromFile ((finder->Path+DirSlash+finder->FileName).c_str());
		addSpriteForm->Texture->Update();
		//
		//MapEditor->SpritesGroups[MapEditor->CurSpriteGroup].Items.push_back (addSpriteForm->Texture);
	}

	int w = abs(addSpriteForm->Select.x2-addSpriteForm->Select.x1);
	int h = abs(addSpriteForm->Select.y2-addSpriteForm->Select.y1);
	//
	if (w % 2 > 0) w++;
	if (h % 2 > 0) h++;

	UITallFlatButton *Item = new UITallFlatButton();
	//
	Item->Texture  = addSpriteForm->Texture;
	Item->Caption  = addSpriteForm->NameField->wText;
	Item->Descr    = StrToWStr (IntToStr (w)+" x "+IntToStr (h));
	//
	Item->SpriteW = w;
	Item->SpriteH = h;
	//
	Item->tWidth   = (float) w / (float) addSpriteForm->Texture->Width;
	Item->tHeight  = (float) h / (float) addSpriteForm->Texture->Height;

	Item->tOffsetX =  float (addSpriteForm->Select.x1-addSpriteForm->AtlasImage->Left) / (float) addSpriteForm->Texture->Width;
	Item->tOffsetY =  float (addSpriteForm->Select.y1-addSpriteForm->AtlasImage->Top)  / (float) addSpriteForm->Texture->Height;
	//
	Item->OnMouseDown = MapEditor2D::SelectSprite;
	Item->OnDblClick  = MapEditor2D::AddSpriteOnMap;
	Item->OnClick = 0;
	//
	BaseGameObject *NewTemplate = new BaseGameObject();
	NewTemplate->Init();
	//
	NewTemplate->Data     = SpriteDat;
	NewTemplate->Shader   = TexAtlasShader;
	NewTemplate->mCamera  = MapEditor->Camera;
	NewTemplate->Texture  = addSpriteForm->Texture;
	NewTemplate->Size     = glm::vec2(w, h);
	NewTemplate->tSize    = glm::vec2(Item->tWidth  , Item->tHeight);
	NewTemplate->tOffset  = glm::vec2(Item->tOffsetX, Item->tOffsetY);
	NewTemplate->SetOffset (glm::vec2 (MapEditor->Left, 0));
	
	// Init Collision Polygon
	//
	PolyData *CollisionPoly = new PolyData();
	CollisionPoly->NoDelete = true;
	CollisionPoly->Type = pdtBox;
	CollisionPoly->BoxSize = NewTemplate->Size;
	CollisionPoly->CircleRadius = min (NewTemplate->Size.x, NewTemplate->Size.y) / 2;
	CollisionPoly->Name = L"Collision";

	CollisionPoly->Insert (glm::vec2 (-NewTemplate->Size.x/2, -NewTemplate->Size.y/2));
	CollisionPoly->Insert (glm::vec2 (-NewTemplate->Size.x/2,  NewTemplate->Size.y/2));
	CollisionPoly->Insert (glm::vec2 ( NewTemplate->Size.x/2,  NewTemplate->Size.y/2));
	CollisionPoly->Insert (glm::vec2 ( NewTemplate->Size.x/2, -NewTemplate->Size.y/2));
	
	//CollisionPoly.Vertices.push_back (glm::vec2 (-NewTemplate->Size.x/2, -NewTemplate->Size.y/2));
	//CollisionPoly.Vertices.push_back (glm::vec2 (-NewTemplate->Size.x/2,  NewTemplate->Size.y/2));
	//CollisionPoly.Vertices.push_back (glm::vec2 ( NewTemplate->Size.x/2,  NewTemplate->Size.y/2));
	//CollisionPoly.Vertices.push_back (glm::vec2 ( NewTemplate->Size.x/2, -NewTemplate->Size.y/2));

	NewTemplate->PolysList.push_back(CollisionPoly);

	LabelData PivotLabel;
	PivotLabel.NoDelete = true;
	PivotLabel.Dir  = glm::vec2 (0, 1);
	PivotLabel.Name = L"Pivot";
	NewTemplate->LabelsList.push_back(PivotLabel);
	//
	NewTemplate->Material->Copy (MapEditor->SpriteMaterial);

	NewTemplate->Material->Uniforms->SetTex2D ("Texture", addSpriteForm->Texture);
	NewTemplate->Material->Uniforms->SetVec2f ("Size"   , NewTemplate->tSize);
	NewTemplate->Material->Uniforms->SetVec2f ("Offset" , NewTemplate->tOffset);
	NewTemplate->Template = NewTemplate;
	//
	if (NewTemplate->Vars->Exist("Texture"))
		NewTemplate->Vars->SetTex2D("Texture", NewTemplate->Texture);
	//
	Item->CustomData = NewTemplate;
	MapEditor->ObjTemplates.push_back (NewTemplate);
	//
	SpriteGroupItem gItem;
	gItem.Name    = Item->Caption;
	gItem.Texture = Item->Texture;

	gItem.TexSize[0] = Item->SpriteW;
	gItem.TexSize[1] = Item->SpriteH;

	gItem.TexOffset[0] = Item->tOffsetX;
	gItem.TexOffset[1] = Item->tOffsetY;

	MapEditor->SpritesGroups[MapEditor->CurSpriteGroup].Items.push_back (gItem);

	MapEditor->SpritesList->AddItem (Item);
	MapEditor->SpritesList->Init();
}

inline void AddSpriteFormOpen() {
	if (Textures::TestExistTex->LoadFromFile (finder->FileName.c_str())) {
		addSpriteForm->Open();
	} else ShowError (L"I Can't load texture :("); // FIXME Make normal error messages
}

void AddSpriteForm::OnChangeClick (UIElement *Sender) {
	addSpriteForm->Texture = 0;
	finder->Open (ftOpenPicture, AddSpriteFormOpen);
	addSpriteForm->Dialog->Close();
}

// <------------------ Form Events

// Events ----------------------->

void AddSpriteForm::KeyPressed (Uint16 key) {

}

void AddSpriteForm::MouseDown (int x, int y, int shift) {
	//
	if (!Dialog->isOpened) return;
	if (shift == mouseLeft && PointInRect (x, y, x0, y0, BrowsePanel->Width-16, BrowsePanel->Height-16)) {
		isMouseDown = true;
		//
		if (x < AtlasImage->AbsoluteLeft) x = AtlasImage->AbsoluteLeft;
		if (y < AtlasImage->AbsoluteTop)  y = AtlasImage->AbsoluteTop;
		//
		if (x > AtlasImage->AbsoluteLeft+AtlasImage->Width ) x = AtlasImage->AbsoluteLeft+AtlasImage->Width;
		if (y > AtlasImage->AbsoluteTop +AtlasImage->Height) y = AtlasImage->AbsoluteTop +AtlasImage->Height;
		//
		Select.x1 = x-x0+BrowsePanel->ScrollX; Select.y1 = y-y0+BrowsePanel->ScrollY;
		Select.x2 = x-x0+BrowsePanel->ScrollX; Select.y2 = y-y0+BrowsePanel->ScrollY;
		//
		int tx = min (Select.x1, Select.x2);
		int ty = min (Select.y1, Select.y2);

		PosX->wText = StrToWStr(IntToStr(tx-AtlasImage->Left));
		PosY->wText = StrToWStr(IntToStr(ty-AtlasImage->Top));
		//
		SizeW->wText = StrToWStr(IntToStr(abs(Select.x2-Select.x1)));
		SizeH->wText = StrToWStr(IntToStr(abs(Select.y2-Select.y1)));
	}
}

void AddSpriteForm::MouseUp (int x, int y, int shift) {
	isMouseDown = false;
}

void AddSpriteForm::MouseMove (int x, int y, int shift) {
	//
	if (!Dialog->isOpened) return;
	if (isMouseDown) {
		if (x < AtlasImage->AbsoluteLeft) x = AtlasImage->AbsoluteLeft;
		if (y < AtlasImage->AbsoluteTop ) y = AtlasImage->AbsoluteTop;

		if (x > AtlasImage->AbsoluteLeft+AtlasImage->Width ) x = AtlasImage->AbsoluteLeft+AtlasImage->Width;
		if (y > AtlasImage->AbsoluteTop +AtlasImage->Height) y = AtlasImage->AbsoluteTop +AtlasImage->Height;

		Select.x2 = x-x0+BrowsePanel->ScrollX;
		Select.y2 = y-y0+BrowsePanel->ScrollY;
		//
		int tx = min (Select.x1, Select.x2);
		int ty = min (Select.y1, Select.y2);

		PosX->wText = StrToWStr(IntToStr(tx-AtlasImage->Left));
		PosY->wText = StrToWStr(IntToStr(ty-AtlasImage->Top));
		//
		SizeW->wText = StrToWStr(IntToStr(abs(Select.x2-Select.x1)));
		SizeH->wText = StrToWStr(IntToStr(abs(Select.y2-Select.y1)));
	}
}

// <----------------------- Events

#endif
