#ifndef CPP_UIMENUS
#define CPP_UIMENUS

//----------------------------------------------------------------------------//
// FlatButton (UI Element)
//----------------------------------------------------------------------------//

UIFlatButton::UIFlatButton (const bool List) {
	Leav  = true;
	Enter = false;
	Click = false;
	InRect = false;
	Left = 100; Top = 100;
	Width = 100;
	TextShadows[0] = false; TextShadows[1] = false;
	TextShadows[2] = false; TextShadows[3] = false;
	Menu = 0; ShowIcon = List;
	Checked = false;
	//
	if (List) {
		LeavParam  = "listitemleav";
		EnterParam = "listitementer";
		ClickParam = "listitementer";
	} else {
		LeavParam  = "flatbuttonleav";
		EnterParam = "flatbuttonenter";
		ClickParam = "flatbuttonclick";
	}
}

void UIFlatButton::Init() {
	//
	UIButton::Init();
	//
	if (Menu != 0) {
		Menu->Shader = Shader;
		Menu->Theme  = Theme;
		Menu->Manager = Manager;
		Menu->Init();
	}
}

void UIFlatButton::Draw (int x, int y) {
	//
	UIButton::Draw (x, y);
	
	//
	if (ShortKey.size() != 0) {
		if (Menu == 0) DrawText (alRight, ShortKey, ShortKey.size());
		else DrawText (alRight, ShortKey, ShortKey.size(), 12);
	}
}

//----------------------------------------------------------------------------//
// TallFlatButton (UI Element)
//----------------------------------------------------------------------------//

UITallFlatButton::UITallFlatButton () {
	Leav  = true;
	Enter = false;
	Click = false;
	InRect = false;
	Left = 100; Top = 100;
	Width = 100;
	TextShadows[0] = false; TextShadows[1] = false;
	TextShadows[2] = false; TextShadows[3] = false;
	Checked = false; NoDrawText = true;
	//
	LeavParam  = "tallflatbuttonleav";
	EnterParam = "tallflatbuttonenter";
	ClickParam = "tallflatbuttonclick";
	//
}

UITallFlatButton::~UITallFlatButton() {
	delete QuadElement;
}

void UITallFlatButton::Init() {
	//
	UIButton::Init();
	//
	// Mask
	eOffsetsX[0] = Theme->Data[LeavParam].Func["mask"][0].Num / Theme->Skin->Width;
	eOffsetsY[0] = Theme->Data[LeavParam].Func["mask"][1].Num / Theme->Skin->Height;
	eWidths  [0] = Theme->Data[LeavParam].Func["mask"][2].Num / Theme->Skin->Width;
	eHeights [0] = Theme->Data[LeavParam].Func["mask"][3].Num / Theme->Skin->Height;

	//
	// Border
	eOffsetsX[1] = Theme->Data[LeavParam].Func["border"][0].Num / Theme->Skin->Width;
	eOffsetsY[1] = Theme->Data[LeavParam].Func["border"][1].Num / Theme->Skin->Height;
	eWidths  [1] = Theme->Data[LeavParam].Func["border"][2].Num / Theme->Skin->Width;
	eHeights [1] = Theme->Data[LeavParam].Func["border"][3].Num / Theme->Skin->Height;
	eiWidths [1] = floor(Theme->Data[LeavParam].Func["border"][2].Num);
	eiHeights[1] = floor(Theme->Data[LeavParam].Func["border"][3].Num);
	//
	TextColors2[0]   = Theme->Data[LeavParam].Func["descrcolor"][0].Num/255.0f;
	TextColors2[1]   = Theme->Data[LeavParam].Func["descrcolor"][1].Num/255.0f;
	TextColors2[2]   = Theme->Data[LeavParam].Func["descrcolor"][2].Num/255.0f;
	//
	TextColors2[3]   = Theme->Data[EnterParam].Func["descrcolor"][0].Num/255.0f;
	TextColors2[4]   = Theme->Data[EnterParam].Func["descrcolor"][1].Num/255.0f;
	TextColors2[5]   = Theme->Data[EnterParam].Func["descrcolor"][2].Num/255.0f;
	//
	TextColors2[6]   = Theme->Data[ClickParam].Func["descrcolor"][0].Num/255.0f;
	TextColors2[7]   = Theme->Data[ClickParam].Func["descrcolor"][1].Num/255.0f;
	TextColors2[8]   = Theme->Data[ClickParam].Func["descrcolor"][2].Num/255.0f;
	//
	QuadElement = new BaseObject();
	QuadElement->Init();

	QuadElement->Data    = GUIElementDat;
	QuadElement->mCamera = ScreenCamera;
	QuadElement->Shader  = Shader;
	//
	//Texture = Textures.TestSprite;
}

void UITallFlatButton::Draw (int x, int y) {
	//
	UIButton::Draw (x, y);
	//
	int n  = 0;
	int tn = 0;
	int to = 0;
	
	if (Enter) { n = 3; tn = 1; to = 2; }
	if (Click) { n = 6; tn = 2; to = 4; }
	
	Shader->UnApplyShader();
	//
	TexAtlasAlphaShader->ApplyShader();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Texture->Handle);

	glUniform1i (TexAtlasAlphaShader->Locations->GetData("aTex"), ThemeTexID);
	glUniform1i (TexAtlasAlphaShader->Locations->GetData("Texture"), 1);
	glUniform1f (TexAtlasAlphaShader->Locations->GetData("Alpha"), 1.0f);
	//
	glUniform2f (TexAtlasAlphaShader->Locations->GetData("Size")  , tWidth  , tHeight);
	glUniform2f (TexAtlasAlphaShader->Locations->GetData("Offset"), tOffsetX, tOffsetY);
	glUniform2f (TexAtlasAlphaShader->Locations->GetData("aSize")  , eWidths  [0], eHeights [0]);
	glUniform2f (TexAtlasAlphaShader->Locations->GetData("aOffset"), eOffsetsX[0], eOffsetsY[0]);
	//
	/*float ratio = (float) Texture->Height / (float) Texture->Width;

	if (ratio == 1.0f) {
		QuadElement->SetSize(glm::vec2(eiWidths[1], eiHeights[1]));
	} else if (ratio < 1.0f) {
		QuadElement->SetSize(glm::vec2(eiWidths[1], eiHeights[1]*ratio));
	} else {
		QuadElement->SetSize(glm::vec2(eiWidths[1]/ratio, eiHeights[1]));
	}

	float tilex = Left+2+iWidths[1]/2+QuadElement->Size[0]/2;
	float tiley = WindowHeight-eiHeights[1]-Top-2-iWidths[1]/2-QuadElement->Size[1]/2;*/

	//QuadElement->SetPosition(glm::vec2(tilex, tiley));
	//PreviewBorder->Top  = (PreviewPanel->Height >> 1) - (PreviewBorder->Height >> 1);
	//PreviewBorder->Left = (PreviewPanel->Width  >> 1) - (PreviewBorder->Width  >> 1);
	QuadElement->BaseRender();
	//
	TexAtlasAlphaShader->UnApplyShader();
	//
	glBegin2D();
	glActiveTexture(GL_TEXTURE0);
	//
	string Text = wstring2string (Caption);
	int tx = Left+40; int ty = 14;
	//
	if (TextShadows[tn]) {
		glColor3f(ShadowColors[n], ShadowColors[n+1], ShadowColors[n+2]);
		glRasterPos2f(tx+ShadowOffsets[to], WindowHeight-Top-ty+ShadowOffsets[to+1]);
		ftglRenderFont(Theme->Font, Text.c_str(), RENDER_ALL);
	}
	//
	
	glPushMatrix();
	glColor3f(TextColors[n], TextColors[n+1], TextColors[n+2]);
	glTranslatef(tx+TextOffsets[to], WindowHeight-Top-ty+TextOffsets[to+1], 0);
	ftglRenderFont(Theme->Font, Text.c_str(), RENDER_ALL);
	glPopMatrix();
	//
	//
	Text = wstring2string (Descr);
	ty = 30;
	//
	if (TextShadows[tn]) {
		glColor3f(ShadowColors[n], ShadowColors[n+1], ShadowColors[n+2]);
		glRasterPos2f(tx+ShadowOffsets[to], WindowHeight-Top-ty+ShadowOffsets[to+1]);
		ftglRenderFont(Theme->Font, Text.c_str(), RENDER_ALL);
	}
	//
	glActiveTexture(GL_TEXTURE0);
	glColor3f(TextColors2[n], TextColors2[n+1], TextColors2[n+2]);
	glTranslatef(tx+TextOffsets[to], WindowHeight-Top-ty+TextOffsets[to+1], 0);
	ftglRenderFont(Theme->Font, Text.c_str(), RENDER_ALL);
	//
	glEnd2D();
	Shader->ApplyShader();
	//
	glUniform2f (Shader->Locations->GetData("Size")  , eWidths  [1], eHeights [1]);
	glUniform2f (Shader->Locations->GetData("Offset"), eOffsetsX[1], eOffsetsY[1]);
	//
	QuadElement->SetPosition (glm::vec2(Left+2, WindowHeight-eiHeights[1]-Top-2));
	QuadElement->SetSize (glm::vec2(eiWidths[1], eiHeights[1]));
	QuadElement->BaseRender();
	//
}

//----------------------------------------------------------------------------//
// ListMenu (UI Element)
//----------------------------------------------------------------------------//

UIListMenu::~UIListMenu() {
	//
	for (int i = 0; i < Items.size(); i++)
		delete Items[i];

	for (int i = 0; i < 11; i++)
		delete DrawElements[i];
	//
	//free (Items);
}

void UIListMenu::Clear() {
	//
	for (int i = 0; i < Items.size(); i++)
		delete Items[i];
	//
	Items.clear();
	Count = 0;
	//UIElement *Item = Items.back();
	//
	/*while (!Items.empty()) {
		UIElement *Item = Items.back();
		Items.pop_back();
	}*/
}

void UIListMenu::Progress() {
	for (int i = 0; i < 11; i++)
		DrawElements[i]->Step(1.0f);
}

void UIListMenu::Draw(int x, int y) {
	//
	if (Align == alCenter)
		Width = Parent->Width-(Parent->Padding[0] << 1)-Parent->ScrollWidth;
	//
	Left = x; Top = y;
	//
	// Draw bg
	//
	if (Popup) {
		//
		int bx = Left-(iWidths[0] >> 1);
		int by = WindowHeight-Top-iHeights[0];
		//
		if (DrawAlign == alTop || DrawAlign == alAll) {
			//
			// Left
			//
			DrawElements[0]->SetPosition (glm::vec2(bx, by));
			DrawElements[0]->SetSize (glm::vec2(iWidths[0], iHeights[0]));
			//
			glUniform2f (Shader->Locations->GetData("Size")  , Widths  [0], Heights [0]);
			glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[0], OffsetsY[0]);
			//
			DrawElements[0]->BaseRender();
			//
			// Center
			//
			glUniform2f (Shader->Locations->GetData("Size")  , Widths  [1], Heights [1]);
			glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[1], OffsetsY[1]);
			//
			DrawElements[1]->SetPosition (glm::vec2(bx+iWidths[1], by));
			DrawElements[1]->SetSize (glm::vec2(Width-iWidths[2], iHeights[1]));
			DrawElements[1]->BaseRender();
			//
			// Right
			//
			glUniform2f (Shader->Locations->GetData("Size")  , Widths  [2], Heights [2]);
			glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[2], OffsetsY[2]);
			//
			DrawElements[2]->SetPosition (glm::vec2(bx+Width, by));
			DrawElements[2]->SetSize (glm::vec2(iWidths[2], iHeights[2]));
			DrawElements[2]->BaseRender();
			//
		}
		//
		by = WindowHeight-Top-Height+iHeights[1];
		// Left
		//
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [3], Heights [3]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[3], OffsetsY[3]);
		//
		DrawElements[3]->SetPosition (glm::vec2(bx, by));
		DrawElements[3]->SetSize (glm::vec2(iWidths[3], Height-iHeights[1]-iWidths[7]));
		DrawElements[3]->BaseRender();
		//
		// Center
		//
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [4], Heights [4]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[4], OffsetsY[4]);
		//
		DrawElements[4]->SetPosition (glm::vec2(bx+iWidths[4], by));
		DrawElements[4]->SetSize (glm::vec2(Width-iWidths[5], Height-iHeights[1]-iWidths[7]));
		DrawElements[4]->BaseRender();
		//
		// Right
		//
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [5], Heights [5]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[5], OffsetsY[5]);
		//
		DrawElements[5]->SetPosition (glm::vec2(bx+Width, by));
		DrawElements[5]->SetSize (glm::vec2(iWidths[5], Height-iHeights[1]-iWidths[7]));
		DrawElements[5]->BaseRender();
		//
		//
		by = WindowHeight-Top-Height;
		//
		if (DrawAlign == alBottom || DrawAlign == alAll) {
			//
			// Left
			//
			glUniform2f (Shader->Locations->GetData("Size")  , Widths  [6], Heights [6]);
			glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[6], OffsetsY[6]);
			//
			DrawElements[6]->SetPosition (glm::vec2(bx, by));
			DrawElements[6]->SetSize (glm::vec2(iWidths[6], iHeights[6]));
			DrawElements[6]->BaseRender();
			//
			// Center
			//
			glUniform2f (Shader->Locations->GetData("Size")  , Widths  [7], Heights [7]);
			glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[7], OffsetsY[7]);
			//
			DrawElements[7]->SetPosition (glm::vec2(bx+iWidths[7], by));
			DrawElements[7]->SetSize (glm::vec2(Width-iWidths[8], iHeights[7]));
			DrawElements[7]->BaseRender();
			//
			// Right
			//
			glUniform2f (Shader->Locations->GetData("Size")  , Widths  [8], Heights [8]);
			glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[8], OffsetsY[8]);
			//
			DrawElements[8]->SetPosition (glm::vec2(bx+Width, by));
			DrawElements[8]->SetSize (glm::vec2(iWidths[8], iHeights[8]));
			DrawElements[8]->BaseRender();
			//
		}
	}
	//
	// Draw Items
	//
	int ey = y+iHeights[1];
	
	//
	for (int i = 0; i < Items.size(); i++) {
		//
		if (!Items[i]->Visible)
			continue;
		//
		
		if (Items[i]->Caption == L"-") {
			ey += 6;
			//
			glUniform2f (Shader->Locations->GetData("Size")  , Widths  [11], Heights [11]);
			glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[11], OffsetsY[11]);
			//
			DrawElements[10]->SetPosition (glm::vec2(x+2, WindowHeight-ey+3));
			DrawElements[10]->SetSize (glm::vec2(Width-4, iHeights[11]));
			DrawElements[10]->BaseRender();
		} else {
			Items[i]->Width = Width;
			Items[i]->Draw(x, ey);
			ey += Items[i]->Height+1;
		}
	}
	//
	Height = ey-y+iHeights[7];
	ey = y+iHeights[1];
	//
	//
	//
	Enter = false;
	
	if (!DialogOpened || inDialog)
	for (int i = 0; i < Items.size(); i++) {
		//
		if (!Items[i]->Visible)
			continue;
		//
		bool cond = (PointInRect(mx, my, x-iWidths[3], ey, Items[i]->Width+iWidths[3]+iWidths[5]-1, Items[i]->Height) && Items[i]->Enabled);
		
		if (Parent != 0) cond &= PointInRect(mx, my, Parent->Left, Parent->Top, Parent->Width, Parent->Width);
		if (cond) Items[i]->InRect = true;
		else Items[i]->InRect = false;
		//
		if (Items[i]->Menu != 0) {
			cond |= (PointInRect(mx, my, x+Items[i]->Width+iWidths[3], ey, Items[i]->Menu->Width-1, Items[i]->Menu->Height) && Items[i]->Enter);
			//
			int n = 9;
			if (Items[i]->Enter) n = 10;
			//
			glUniform2f (Shader->Locations->GetData("Size")  , Widths  [n], Heights [n]);
			glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[n], OffsetsY[n]);
			//
			DrawElements[9]->SetPosition (glm::vec2(x+Width-iWidths[5]-iWidths[3], WindowHeight-Items[i]->Height-ey));
			DrawElements[9]->SetSize (glm::vec2(iWidths[n], iHeights[n]));
			DrawElements[9]->BaseRender();
			//
		}
		//
		if (CheckList) {
			if (Items[i]->Checked) {
				Items[i]->Enter = true;
				//
				if (LastCheck == 0)
					LastCheck = Items[i];
				//
			}
			//
			//if (cond) Enter = true;
			if (cond && mShift == mouseLeft) {
				//
				if (LastCheck != 0) LastCheck->Enter   = false;
				if (LastCheck != 0) LastCheck->Checked = false;

				Items[i]->Checked = true;
				Items[i]->Enter   = true;
				LastCheck = Items[i]; SelItem = i;
			}
			//
			if (Items[i]->Caption == L"-") ey += 6;
			else ey += Items[i]->Height+1;
			//
			continue;
		}
		//
		if (cond) {
			//
			Enter = true;
			//
			if (!Items[i]->Enter) {
				Delay = 0;
				Items[i]->Enter = true;
				SelItem = i;
			}
			
			//
			if (Items[i]->Menu != 0 && Delay >= 0.00025f) {
				//
				Items[i]->Menu->Left = x+Items[i]->Width+iWidths[3];
				Items[i]->Menu->Top  = ey;
				//
				Manager->DrawStack.push_back(Items[i]->Menu);
				//
			} else Delay += DeltaTime;
			//
			
			if (mShift == mouseLeft) Items[i]->Click = true;
			else Items[i]->Click = false;
		} else {
			Items[i]->Enter = false;
			Items[i]->Click = false;
			//
			if (Items[i]->Menu != 0) {
				//
				for (int j = 0; j < Items[i]->Menu->Items.size(); j++) {
					Items[i]->Menu->Items[j]->Enter = false;
					Items[i]->Menu->Items[j]->Click = false;
				}
				//
			}
			//Delay = 0;
		}
		
		if (Items[i]->Caption == L"-") ey += 6;
		else ey += Items[i]->Height+1;
	}
	//
	mShift = mouseNone;
	//
}

// Events

void UIListMenu::MouseMove (int x, int y, int Shift) {
	mx = x; my = y;
	//
	for (int i = 0; i < Items.size(); i++) {
		Items[i]->MouseMove(x, y, Shift);
		if (Items[i]->Menu != 0) Items[i]->Menu->MouseMove(x, y, Shift);
	}
}

void UIListMenu::MouseDown (int x, int y, int Shift) {
	//
	mShift = Shift;
	//
	for (int i = 0; i < Items.size(); i++) {
		//
		Items[i]->MouseDown(x, y, Shift);
		if (Items[i]->Menu != 0) Items[i]->Menu->MouseDown(x, y, Shift);
	}
}

void UIListMenu::DblClick (int x, int y, int Shift) {
	//
	for (int i = 0; i < Items.size(); i++) {
		//
		Items[i]->DblClick (x, y, Shift);
		if (Items[i]->Menu != 0) Items[i]->Menu->DblClick (x, y, Shift);
	}
}

void UIListMenu::MouseUp (int x, int y, int Shift) {
	if (DropMenu != 0 && !DropMenu->Checked) {
		mShift = mouseNone;
		return;
	}
	//
	for (int i = 0; i < Items.size(); i++) {
		Items[i]->MouseUp(x, y, Shift);

		if (Items[i]->Enter) {
			if (DropMenu != 0)
				DropMenu->Checked = false;
		}

		if (Items[i]->Menu != 0) Items[i]->Menu->MouseUp(x, y, Shift);
	}
	
	mShift = mouseNone;
}

//
void UIListMenu::KeyPressed (Uint16 key) {
	//
	for (int i = 0; i < Items.size(); i++) {
		Items[i]->KeyPressed (key);
		if (Items[i]->Menu != 0) Items[i]->Menu->KeyPressed (key);
	}
	//
	if (!Enter) return;
	//
	if (key == keyUpArrow) {
		SelItem--;
		if (SelItem < 0) SelItem = Items.size()-1;
	}
	//
	if (key == keyDownArrow) {
		SelItem++;
		if (SelItem >= Items.size()) SelItem = 0;
	}
	//
}

//
void UIDropMenu::Init() {
	//
	if (isMenu) {
		LeavParam  = "flatbuttonleav";
		EnterParam = "flatbuttonenter";
		ClickParam = "flatbuttonclick";
	} else {
		LeavParam  = "droplistleav";
		EnterParam = "droplistenter";
		ClickParam = "droplistclick";
	}
	//
	UIButton::Init();
	//
	mOffsetX = Theme->Data["droplistleav"].Func["mark"][0].Num / Theme->Skin->Width;
	mOffsetY = Theme->Data["droplistleav"].Func["mark"][1].Num / Theme->Skin->Height;
	mWidth   = Theme->Data["droplistleav"].Func["mark"][2].Num / Theme->Skin->Width;
	mHeight  = Theme->Data["droplistleav"].Func["mark"][3].Num / Theme->Skin->Height;
	miWidth  = floor(Theme->Data["droplistleav"].Func["mark"][2].Num);
	miHeight = floor(Theme->Data["droplistleav"].Func["mark"][3].Num);
	//---
	Menu->Manager = Manager;
	Menu->Shader = Shader;
	Menu->Theme  = Theme;
	Menu->DropMenu = this;
	//
	//if (!isMenu) Menu->Width = Width+2;
	
	Menu->Init();
	Menu->inDialog = inDialog;
	//
	if (!isMenu) {
		for (int i = 0; i < Menu->Count; i++) {
			//
			if (i == SelItem) {
				//Caption = Menu->Items[i]->Caption;
				break;
			}
			//
		}
		
		TextAlign = alLeft;
	}
	//Menu->DrawAlign = alBottom;
	//
	ArrowElement = new BaseObject();
	ArrowElement->Init();
	//
	ArrowElement->Data    = GUIElementDat;
	ArrowElement->mCamera = ScreenCamera;
	//
}

void UIDropMenu::Progress() {
	ArrowElement->Step(1.0f);
	Menu->Progress();
}

//
void UIDropMenu::Draw (int x, int y) {
	//
	Click = Checked;
	UIButton::Draw(x, y);
	//
	// Mark
	// Shader
	if (!isMenu) {
		//
		glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(ArrowElement->MVPMatrix[0][0]));
		glUniform2f (Shader->Locations->GetData("Size")  , mWidth  , mHeight);
		glUniform2f (Shader->Locations->GetData("Offset"), mOffsetX, mOffsetY);
		//
		int markx = x;
		if (DrawAlign == alLeft) markx = x+iWidths[0];
		//
		ArrowElement->SetPosition (glm::vec2(markx+Width-iWidths[0]-iWidths[2]-iWidths[2], WindowHeight-y-miHeight));
		ArrowElement->SetSize (glm::vec2(miWidth, miHeight));
		ArrowElement->Render();
		//
	}
	
	//
	if (Checked) {
		//
		if (DrawAlign == alLeft) Menu->Left = x+11;
		else Menu->Left = x+1;

		Menu->Top  = y+Height;
		Manager->DrawStack.push_back(Menu);
		//
		if (Menu->Enter || PointInRect (mx, my, Menu->Left, Menu->Top, Menu->Width-1, Menu->Height+12)) {
			Enter = true;
		}
		//Enter = Menu->Enter;
	}
	//
	if (!Enter) {
		//
		if (Checked)
		for (int i = 0; i < Menu->Count; i++)
			Menu->Items[i]->Enter = false;
		//
		Checked = false;
	}
	//
}

// Events

void UIDropMenu::MouseMove (int x, int y, int Shift) {
	mx = x;
	my = y;
	//
	Menu->MouseMove (x, y, Shift);
}

void UIDropMenu::MouseDown (int x, int y, int Shift) {
	//
	mShift = Shift;

	if (Checked) {
		//
		for (int i = 0; i < Menu->Count; i++) {
			//
			//if (i == SelItem) {
			if (Menu->Items[i]->Enter) {
				if (Menu->Items[i]->Menu != 0 && Menu->Items[i]->InRect) return;
				if (!isMenu && ShowText) {
					SelItem = i;
					Caption = Menu->Items[i]->Caption;
				}
				
				Menu->Items[i]->MouseUp (x, y, Shift);
				Menu->Items[i]->Enter = false;
				break;
			}
			//
		}
		//
		Checked = false;
	}
	
	Menu->MouseDown(x, y, Shift);
	if (Enter && !Checked) Checked = true;
	//
}

void UIDropMenu::MouseUp (int x, int y, int Shift) {
	//
	Menu->MouseUp(x, y, Shift);
	mShift = mouseNone;
}

//
void UIDropMenu::KeyPressed (Uint16 key) {
	//
	if (!Enter) return;
	//
	Menu->KeyPressed(key);
}

#endif
