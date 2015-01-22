#ifndef CPP_UI
#define CPP_UI


//* UITheme *//

void UITheme::Update() {
	Skin->Filter = GL_NEAREST;
	Skin->WrapS = GL_NONE;
	Skin->WrapT = GL_NONE;
	Skin->BuildMipmaps = false;
	Skin->Update();
}

//
void UIElement::MouseUp   (int x, int y, int Shift) {
	//
	//NowClick = false;
	//
	if (Enabled && Enter/* && PointInRect (x, y, Left, Top, Width, Height)*/) {
		mShift = mouseNone;
		Click = false;
		Enter = false;

		if (OnClick != 0)
			OnClick (this);
		
		//NowClick = true;		
	}
}
//
void UIElement::MouseDown (int x, int y, int Shift) {
	//
	if (Visible)
	if (OnMouseDown != 0 && Enabled && PointInRect (x, y, Left, Top, Width, Height))
		OnMouseDown (this, x, y, Shift);
	//
}

//
void UIElement::DblClick (int x, int y, int Shift) {
	//
	if (Visible)
	if (OnDblClick != 0 && Enabled && PointInRect (x, y, Left, Top, Width, Height))
		OnDblClick (this, x, y, Shift);
	//
}
//
void UIElement::KeyPressed (Uint16 key) {
	//
	if (Visible)
	if (OnKeyPressed != 0 && Enabled && NowClick)
		OnKeyPressed (this, key);
}

void UIElement::SetFocused() {
	//
	if (!DialogOpened || inDialog) {
		Focused = true;
	}
}

void UIElement::UnFocused() {
	//
	/*Focused = false;
	//
	if (OnUnFocused != 0)
		OnUnFocused (this);*/
	Manager->UnFocusedElements.push_back(this);
}

//* UIManager *//

//
UIManager::UIManager() {
	mx = 0; my = 0; mShift = mouseNone;
	FocusedElement = 0; EnabledScissor = false;
}

UIManager::~UIManager() {
	//
	for (int i = 0; i < Elements.size(); i++) {
		delete Elements[i];
	}
}

// Initialize
void UIManager::Init() {
	for (int i = 0; i < Elements.size(); i++) {
		Elements[i]->Theme = Theme;
		Elements[i]->Shader = Shader;
		Elements[i]->ColorShader = ColorShader;
		Elements[i]->Focused = false;
		Elements[i]->Init();
	}
}

// Scissor test
void UIManager::PushScissor (int sx, int sy, int sw, int sh) {
	if (ScissorStack.size() == 0)
		glEnable (GL_SCISSOR_TEST);

	ScissorStack.push_back(glm::vec4(sx, sy, sx+sw, sy+sh));
	SetScissor();
}

void UIManager::SetScissor () {
	int scx = ScissorStack.back()[0]; int scy = ScissorStack.back()[1];
	int scw = ScissorStack.back()[2]; int sch = ScissorStack.back()[3];

	if (ScissorStack.size() >= 2)
	for (int i = 0; i < ScissorStack.size(); i++) {
		if (scx < ScissorStack[i][0]) scx = ScissorStack[i][0];
		if (scy < ScissorStack[i][1]) scy = ScissorStack[i][1];

		if (scw > ScissorStack[i][2]) scw = ScissorStack[i][2];
		if (sch > ScissorStack[i][3]) sch = ScissorStack[i][3];
	}

	glScissor (scx, scy, scw-scx, sch-scy);
}

void UIManager::PopScissor() {
	ScissorStack.pop_back();

	if (ScissorStack.size() == 0) glDisable (GL_SCISSOR_TEST);
	else SetScissor();
}

// Draw function
void UIManager::Draw() {
	//
	int CursorIcon = curNormal;
	
	Shader->ApplyShader();
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, Theme->Skin->Handle);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, Textures::Icons->Handle);

	GUIElementDat->SetAttrs();
	/*glUniformMatrix4fv (TexAtlasShader->Locations->GetData("ViewMatrix"), 1, GL_FALSE, &(ScreenCamera->ViewMatrix[0][0]));
	glUniformMatrix4fv (TexAtlasShader->Locations->GetData("ProjMatrix"), 1, GL_FALSE, &(ScreenCamera->ProjectionMatrix[0][0]));
	
	glUniformMatrix4fv (TexAtlasAlphaShader->Locations->GetData("ViewMatrix"), 1, GL_FALSE, &(ScreenCamera->ViewMatrix[0][0]));
	glUniformMatrix4fv (TexAtlasAlphaShader->Locations->GetData("ProjMatrix"), 1, GL_FALSE, &(ScreenCamera->ProjectionMatrix[0][0]));

	glUniformMatrix4fv (ColorShader->Locations->GetData("ViewMatrix"), 1, GL_FALSE, &(ScreenCamera->ViewMatrix[0][0]));
	glUniformMatrix4fv (ColorShader->Locations->GetData("ProjMatrix"), 1, GL_FALSE, &(ScreenCamera->ProjectionMatrix[0][0]));*/

	/*GLuint TextureID = glGetUniformLocation(Shader->ProgramObject, "myTextureSampler");
	SizesID   = glGetUniformLocation(Shader->ProgramObject, "Size");
	OffsetsID = glGetUniformLocation(Shader->ProgramObject, "Offset");*/
	
	glUniform1i (Shader->Locations->GetData("Texture"), ThemeTexID);
	glUniform1f (Shader->Locations->GetData("Alpha"), 1.0f);
	//
	for (int i = 0; i < Elements.size(); i++) {
		UIElement *el = Elements[i];
		el->Progress();
		
		if (el->Visible) {
			//
			el->Draw(el->Left, el->Top);
		}
	}
	//
	for (int i = 0; i < DrawStack.size(); i++) {
		DrawStack[i]->Draw(DrawStack[i]->Left, DrawStack[i]->Top);
	}

	DrawStack.clear();
	Shader->UnApplyShader();
	//for (int i = Elements.size()-1; i >= 0; i--) {
	if (!DialogOpened && !FreezUI)
	for (int i = 0; i < Elements.size(); i++) {
		UIElement *el = Elements[i];
		
		if (el->Visible) {
			//
			if (PointInRect(mx, my, el->Left, el->Top, el->Width, el->Height)) {
				if (!el->Enabled || (el->Parent != 0 && !el->Parent->Enabled)) {
					el->Enter = false;
					el->Click = false;

					continue;
				}

				CursorIcon = el->Cursor;
				el->Enter = true;
				
				if (mShift == mouseLeft) el->Click = true;
				else el->Click = false;
				//break;
			} else { el->Enter = false; el->Click = false; }
			//
			//if (dynamic_cast<UIGroupedElements*>(el)) CursorIcon = el->Cursor;
		}
	}

	//SetCursor (CursorIcon);
}

// Events
// Mouse Move
void UIManager::MouseMove (int x, int y, int Shift) {
	mx = x; my = y;
	
	for (int i = 0; i < Elements.size(); i++) {
		Elements[i]->MouseMove(x, y, Shift);
	}
}

void UIManager::MouseDown (int x, int y, int Shift) {
	mShift = Shift;
	bool setfocused = false;
	
	for (int i = Elements.size()-1; i >= 0; i--) {
		Elements[i]->MouseDown(x, y, Shift);
	}
}

void UIManager::DblClick (int x, int y, int Shift) {
	mShift = Shift;
	
	for (int i = 0; i < Elements.size(); i++) {
		Elements[i]->DblClick(x, y, Shift);
	}
}

void UIManager::MouseUp (int x, int y, int Shift) {	
	for (int i = Elements.size()-1; i >= 0; i--) {
		Elements[i]->MouseUp(x, y, Shift);
	}

	for (int i = 0; i < UnFocusedElements.size(); i++) {
		if (!UnFocusedElements[i]->Focused)
			continue;
		
		UnFocusedElements[i]->Focused = false;
		//
		if (UnFocusedElements[i]->OnUnFocused != 0)
			UnFocusedElements[i]->OnUnFocused (UnFocusedElements[i]);
	}
	
	UnFocusedElements.clear();
	mShift = mouseNone;
	NowClick = false;
}

// Keyboard
void UIManager::KeyPressed (Uint16 key) {
	//
	for (int i = 0; i < Elements.size(); i++) {
		Elements[i]->KeyPressed(key);
	}
}

//
void UIManager::Resized (int Width, int Height) {
	//
	for (int i = 0; i < Elements.size(); i++) {
		Elements[i]->Resized(Width, Height);
	}
}

void UIManager::TextEntered (Uint16 key) {
	//
	for (int i = 0; i < Elements.size(); i++) {
		Elements[i]->TextEntered(key);
	}
}

#endif
