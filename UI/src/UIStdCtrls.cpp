//
#ifndef CPP_UISTDCTRLS
#define CPP_UISTDCTRLS

//----------------------------------------------------------------------------//
// UIImage
//----------------------------------------------------------------------------//

void UIImage::Draw (int x, int y) {
	//
	Left = x; Top = y;
	AbsoluteLeft = x; AbsoluteTop = y;
	//
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Texture->Handle);
	glUniform1i (Shader->Locations->GetData("Texture"), 1);
	//
	glUniform2f (Shader->Locations->GetData("Size")  , tWidth, tHeight);
	glUniform2f (Shader->Locations->GetData("Offset"), OffsetX, OffsetY);

	QuadElement->SetPosition (glm::vec2(Left, WindowHeight-Height-Top));
	QuadElement->SetSize (glm::vec2(Width, Height));
	QuadElement->BaseRender();
	//
	glUniform1i (Shader->Locations->GetData("Texture"), ThemeTexID);
	//glActiveTexture(GL_TEXTURE0);
}

//----------------------------------------------------------------------------//
// UIColorPanel
//----------------------------------------------------------------------------//

void UIColorPanel::Draw (int x, int y) {
	//
	Left = x; Top = y;
	AbsoluteLeft = x; AbsoluteTop = y;
	//
	QuadElement->SetPosition (glm::vec2(Left, WindowHeight-Height-Top));
	QuadElement->SetSize     (glm::vec2(Width, Height));
	//
	if (ShowGrid) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Textures::ImageGrid->Handle);
		glUniform1i (Shader->Locations->GetData("Texture"), 1);
		//
		glUniform2f (Shader->Locations->GetData("Size")  , tWidth, tHeight);
		glUniform2f (Shader->Locations->GetData("Offset"), 0.f, 0.f);

		QuadElement->BaseRender();
		//
		glUniform1i (Shader->Locations->GetData("Texture"), ThemeTexID);
	} else QuadElement->UpdateModelMatrix();

	Shader->UnApplyShader();
	ColorShader->ApplyShader();

	glUniformMatrix4fv (ColorShader->Locations->GetData("MVP"), 1, GL_FALSE, &(QuadElement->MVPMatrix[0][0]));
	glUniform4fv (ColorShader->Locations->GetData("Color"), 1, &Color[0]);
	QuadElement->EmptyRender();
	ColorShader->UnApplyShader();

	Shader->ApplyShader();
}

void UIColorPanel::Update() {
	tWidth  = (float) Width  / (float) Textures::ImageGrid->Width;
	tHeight = (float) Height / (float) Textures::ImageGrid->Height;
}

//----------------------------------------------------------------------------//
// UIHr
//----------------------------------------------------------------------------//

void UIHr::Draw (int x, int y) {
	//
	Left = x; Top = y;
	AbsoluteLeft = x; AbsoluteTop = y;
	//
	glUniform2f (Shader->Locations->GetData("Size")  , tWidth, tHeight);
	glUniform2f (Shader->Locations->GetData("Offset"), OffsetX, OffsetY);
	//
	QuadElement->SetPosition (glm::vec2 (2, WindowHeight-Height-Top));
	QuadElement->SetSize (glm::vec2 (Parent->Width-4, iHeight));
	QuadElement->BaseRender();
	//
}

//----------------------------------------------------------------------------//
// UILabel
//----------------------------------------------------------------------------//

void UILabel::Draw (int x, int y) {
	//
	Left = x; Top = y;
	AbsoluteLeft = x; AbsoluteTop = y;
	// Draw Text
	string Text = wstring2string(Caption);
	float bounds[6];
	
	ftglGetFontBBox(Theme->Font, Text.c_str(), Caption.size(), bounds);
	int twidth = bounds[3]-bounds[0];
	//
	int tx = Left;
	//
	switch (TextAlign) {
		case alLeft   : break;
		case alRight  : tx += (Width-twidth)-TextOffset[0]-TextOffset[0]; break;
		case alCenter : tx += (Width-twidth) >> 1; break;
	}
	//
	Shader->UnApplyShader();
	glBegin2D();
	//
	if (TextShadow) {
		glColor3f(ShadowColor[0], ShadowColor[1], ShadowColor[2]);
		glRasterPos2f(tx+ShadowOffset[0], WindowHeight-Top-(Height >> 1)-4+ShadowOffset[1]);
		ftglRenderFont(Theme->Font, Text.c_str(), RENDER_ALL);
	}
	//
	
	glColor3f(TextColor[0], TextColor[1], TextColor[2]);
	glActiveTexture(GL_TEXTURE0);
	glTranslatef(tx+TextOffset[0], WindowHeight-Top-(Height >> 1)-4+TextOffset[1], 0);
	ftglRenderFont(Theme->Font, Text.c_str(), RENDER_ALL);
	glEnd2D();
	Shader->ApplyShader();
}

//----------------------------------------------------------------------------//
// UIButton
//----------------------------------------------------------------------------//

UIButton::UIButton() {
	Leav  = true;
	Enter = false;
	Click = false;
	Left = 0; Top = 0;
	IconOffset[0] = -1; IconOffset[1] = -1;
	Width = 100; IcoOffset = 0;
	TextShadows[0] = false; TextShadows[1] = false;
	TextShadows[2] = false; TextShadows[3] = false;
	//
	LeavParam  = "buttonleav";
	EnterParam = "buttonenter";
	ClickParam = "buttonclick";
	//
	TextAlign = alCenter;
	ShowIcon = false;
	NoDrawText = false;
	mShift = mouseNone;
	//
}

//
UIButton::~UIButton() {
	//
	delete LeftElement;
	delete RightElement;
	delete MiddleElement;
	delete IconElement;
	delete IconElement2;
}

// Render button
void UIButton::Draw (int x, int y) {
	//
	Left = x; Top = y;
	AbsoluteLeft = x; AbsoluteTop = y;
	//
	if (!Enabled || (Parent != 0 && !Parent->Enabled))
		glUniform1f (TexAtlasShader->Locations->GetData("Alpha"), DisabledAlpha);
	
	int n  = 0;
	int tn = 0;
	int to = 0;
	
	if (Enter) { n = 3; tn = 1; to = 2; }
	if (Click) { n = 6; tn = 2; to = 4; }
	//
	int cWidth = Width-iWidths[n]-1;
	int poffset = 0;

	if (Transparent) goto DrawContent;
	//
	if (DrawAlign == alLeft)   Left   += iWidths[n]+iWidths[n+2];
	//if (DrawAlign == alLeft)   cWidth += iWidths[n]+iWidths[n+2];
	if (DrawAlign == alCenter) cWidth -= iWidths[n]-iWidths[n]-iWidths[n+2]-iWidths[n+2];
	//
	if (DrawAlign == alLeft || DrawAlign == alAll) {
		//
		glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(LeftElement->MVPMatrix[0][0]));
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [n], Heights [n]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[n], OffsetsY[n]);
		//
		LeftElement->SetPosition (glm::vec2(Left, WindowHeight-iHeights[n]-Top));
		LeftElement->SetSize (glm::vec2(iWidths[n], iHeights[n]));
		LeftElement->BaseRender();
	}
	//
	glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(MiddleElement->MVPMatrix[0][0]));
	glUniform2f (Shader->Locations->GetData("Size")  , Widths  [n+1], Heights [n+1]);
	glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[n+1], OffsetsY[n+1]);
	//
	if (dynamic_cast<UICheckButton*> (this) && Parent != 0)
		poffset = 1;
	//
	MiddleElement->SetPosition (glm::vec2(Left+iWidths[n+1]-poffset, WindowHeight-iHeights[n+1]-Top));
	MiddleElement->SetSize (glm::vec2(cWidth-iWidths[n+2]+poffset, iHeights[n+1]));
	MiddleElement->BaseRender();
	//
	if (DrawAlign == alRight || DrawAlign == alAll) {
		//
		glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(RightElement->MVPMatrix[0][0]));
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [n+2], Heights [n+2]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[n+2], OffsetsY[n+2]);
		//
		RightElement->SetPosition (glm::vec2(Left+cWidth, WindowHeight-iHeights[n+2]-Top));
		RightElement->SetSize (glm::vec2(iWidths[n+2], iHeights[n+2]));
		RightElement->BaseRender();
	}
	//

DrawContent:
	if (Transparent && !Enter && !Click)
		glUniform1f (TexAtlasShader->Locations->GetData("Alpha"), DisabledAlpha);

	string Text = wstring2string(Caption);
	int toffset = 0;

	if (ShowIcon) {
		//
		int io[2];

		if (ShowClickIcon && Click) {
			io[0] = ClickIconOffset[0];
			io[1] = ClickIconOffset[1];
		} else {
			io[0] = IconOffset[0];
			io[1] = IconOffset[1];
		}

		if (Transparent && !Enabled) {
			io[0] = ClickIconOffset[0];
			io[1] = ClickIconOffset[1];
		} else {
			io[0] = IconOffset[0];
			io[1] = IconOffset[1];
		}
		//
		if (io[0] >= 0 && io[1] >= 0) {
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, Textures::Icons->Handle);
			glUniform1i (Shader->Locations->GetData("Texture"), 2);
			//
			glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(IconElement->MVPMatrix[0][0]));
			glUniform2f (Shader->Locations->GetData("Size")  , Icons.Width, Icons.Height);
			glUniform2f (Shader->Locations->GetData("Offset"), Icons.OffsetsX[io[0]][io[1]], Icons.OffsetsY[io[0]][io[1]]);
			//
			int iOffset = IcoOffset;
			
			if (DrawAlign == alCenter) iOffset += iWidths[n];
			if (DrawAlign == alRight ) iOffset += iWidths[n];

			IconElement->SetPosition (glm::vec2(Left+2+iOffset, WindowHeight-Height+2-Top));
			IconElement->SetSize (glm::vec2(Icons.SizeIcon, Icons.SizeIcon));
			IconElement->BaseRender();

			glUniform1i (Shader->Locations->GetData("Texture"), ThemeTexID);
		}
		//
		toffset = 18;
	}

	if (ShowIcon2) {
		//
		if (Icon2Offset[0] >= 0 && Icon2Offset[1] >= 0) {
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, Textures::Icons->Handle);
			glUniform1i (Shader->Locations->GetData("Texture"), 2);
			//
			glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(IconElement2->MVPMatrix[0][0]));
			glUniform2f (Shader->Locations->GetData("Size")  , Icons.Width, Icons.Height);
			glUniform2f (Shader->Locations->GetData("Offset"), Icons.OffsetsX[Icon2Offset[0]][Icon2Offset[1]], Icons.OffsetsY[Icon2Offset[0]][Icon2Offset[1]]);
			//
			int iOffset = 0;
			
			if (DrawAlign == alCenter) iOffset += iWidths[n];
			if (DrawAlign == alRight ) iOffset += iWidths[n];

			IconElement2->SetPosition (glm::vec2(Left+20+iOffset, WindowHeight-Height+2-Top));
			IconElement2->SetSize (glm::vec2(Icons.SizeIcon, Icons.SizeIcon));
			IconElement2->BaseRender();

			glUniform1i (Shader->Locations->GetData("Texture"), ThemeTexID);
		}
		//
		toffset = 38;
	}

	if (!NoDrawText)
		DrawText (TextAlign, Text, Caption.size(), toffset);
	//
	if (!Enabled || (Parent != 0 && !Parent->Enabled) || Transparent)
		glUniform1f (TexAtlasShader->Locations->GetData("Alpha"), 1.0f);
}

//
inline void glTextPos2f(float x, float y) {
	//
	glBegin2D();
	glRasterPos2f(x, y);
	glEnd2D();
}

// Render Text
void UIButton::DrawText (int Align, string Text, int Size, int Offset) {
	//
	int n  = 0;
	int tn = 0;
	int to = 0;
	
	if (Enter) { n = 3; tn = 1; to = 2; }
	if (Click) { n = 6; tn = 2; to = 4; }
	
	// Draw Text
	float bounds[6];
	
	ftglGetFontBBox(Theme->Font, Text.c_str(), Size, bounds);
	int twidth = bounds[3]-bounds[0];
	//
	int tx = Left;
	//
	if (Align == alLeft) {
		tx += iWidths[n];
		if (DrawAlign != alLeft & DrawAlign != alAll) tx += iWidths[n+2];
		if (Offset != 0) tx += Offset;
	}
	//
	if (Align == alRight) {
		if (DrawAlign == alCenter) tx += (Width-twidth)-iWidths[n+2];
		else if (DrawAlign == alRight) tx += (Width-twidth)-iWidths[n+2]-iWidths[n];
		else if (DrawAlign == alLeft) tx += (Width-twidth)-iWidths[n+2]-iWidths[n]-iWidths[n];
		else tx += (Width-twidth)-iWidths[n+2]-TextOffsets[to]-TextOffsets[to];//-iWidths[n];
		if (Offset != 0) tx -= Offset;
	}
	//
	if (Align == alCenter) {
		if (DrawAlign == alCenter) tx += (Width+iWidths[n+2]+iWidths[n]-twidth) >> 1;
		else if (DrawAlign == alRight) tx += (Width+iWidths[n+2]-twidth) >> 1;
		else if (DrawAlign == alLeft) tx += (Width-iWidths[n]-twidth) >> 1;
		else tx += (Width-twidth) >> 1;
	}
	//
	Shader->UnApplyShader();
	glBegin2D();
	//
	if (TextShadows[tn]) {
		glColor3f(ShadowColors[n], ShadowColors[n+1], ShadowColors[n+2]);
		glRasterPos2f(tx+ShadowOffsets[to], WindowHeight-Top-(Height >> 1)-4+ShadowOffsets[to+1]);
		ftglRenderFont(Theme->Font, Text.c_str(), RENDER_ALL);
	}
	//

	glActiveTexture(GL_TEXTURE0);
	glColor3f(TextColors[n], TextColors[n+1], TextColors[n+2]);

	//glRasterPos2f(tx+TextOffsets[to], WindowHeight-Top-(Height >> 1)-4+TextOffsets[to+1]);
	glTranslatef(tx+TextOffsets[to], WindowHeight-Top-(Height >> 1)-4+TextOffsets[to+1], 0);
	ftglRenderFont(Theme->Font, Text.c_str(), RENDER_ALL);

	glEnd2D();

	Shader->ApplyShader();
}

//----------------------------------------------------------------------------//
// UICheckButton
//----------------------------------------------------------------------------//

UICheckButton::UICheckButton (const bool Std) {
	Leav  = true;
	Enter = false;
	Click = false;
	Checked = false;
	Left = 0; Top = 0;
	Width = 100;
	TextShadows[0] = false; TextShadows[1] = false;
	TextShadows[2] = false; TextShadows[3] = false;
	
	//
	Checked = false;
	//
	if (!Std) {
		LeavParam  = "checkbuttonleav";
		EnterParam = "checkbuttonenter";
		ClickParam = "checkbuttonclick";
	}
}

void UICheckButton::Draw(int x, int y) {
	//
	if (!dynamic_cast<UIGroupedElements*>(Parent) && Enter && mShift == mouseLeft) {
		Checked = !Checked;
		mShift = mouseNone;
	}
	//
	Click = Checked;
	UIButton::Draw(x, y);
}

//----------------------------------------------------------------------------//
// UICheckBox
//----------------------------------------------------------------------------//

UICheckBox::UICheckBox() {
	Leav  = true;
	Enter = false;
	Click = false;
	Checked = false;
	Left = 0; Top = 0;
	Caption = L"Checkbox";
}

void UICheckBox::Draw (int x, int y) {
	//
	Left = x; Top = y;
	AbsoluteLeft = x; AbsoluteTop = y;
	//
	int n  = 0;
	int tn = 0;
	int sn = 0;
	int to = 0;
	
	if (Checked) { n  = 1; }
	if (Enter)   { n += 2; tn = 3; sn = 1; to = 2; }

	//
	glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(BoxElement->MVPMatrix[0][0]));
	glUniform2f (Shader->Locations->GetData("Size")  , Widths  [n], Heights [n]);
	glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[n], OffsetsY[n]);
	//
	BoxElement->SetPosition (glm::vec2(Left, WindowHeight-iHeights[0]-Top));
	BoxElement->SetSize (glm::vec2(iWidths[n], iHeights[n]));
	BoxElement->Render();
	//
	// Draw Text
	float bounds[6];
	string Text = wstring2string(Caption);
	
	ftglGetFontBBox(Theme->Font, Text.c_str(), Caption.size(), bounds);
	int twidth = bounds[3]-bounds[0];
	//Width  = iWidths [0];

	//
	Shader->UnApplyShader();
	glBegin2D();
	
	if (TextShadows[sn]) {
		glColor3f(ShadowColors[tn], ShadowColors[tn+1], ShadowColors[tn+2]);
		glRasterPos2f(Left+iWidths [0]+ShadowOffsets[to], WindowHeight-Top-(Height >> 1)-4+ShadowOffsets[to+1]);
		ftglRenderFont(Theme->Font, Text.c_str(), RENDER_ALL);
	}
	//
	glColor3f(TextColors[tn], TextColors[tn+1], TextColors[tn+2]);
	//glRasterPos2f(Left+Width+TextOffsets[to], WindowHeight-Top-(Height >> 1)-4+TextOffsets[to+1]);
	glTranslatef(Left+iWidths [0]+TextOffsets[to], WindowHeight-Top-(Height >> 1)-4+TextOffsets[to+1], 0);
	ftglRenderFont(Theme->Font, Text.c_str(), RENDER_ALL);
	//
	
	//Width = (iWidths [0]+twidth+TextOffsets[to]);

	glEnd2D();
	Shader->ApplyShader();
}

// Mouse down
void UICheckBox::MouseDown (int x, int y, int Shift) {
	if (!Enter) return;
	Checked = !Checked;
}

//----------------------------------------------------------------------------//
// UIEdit
//----------------------------------------------------------------------------//

UIEdit::UIEdit() {
	Leav  = true; Enter = false; Click = false;
	Left = 0; Top = 0; Width = 100; Stick = false;
	sx = 0; sy = 0; tx = 0; ty = 0;
	//
	TextShadows[0] = false; TextShadows[1] = false;
	TextShadows[2] = false; StickCounter = 0;
	nchinrect = 0; sLen = 0; lPos = 0; selPos = 0;
	//
	Cursor = curIBeam;
	SelStart = 0; SelEnd = 0;
	After  = L""; Before = L"";
	OnChange = 0; IntFilter = false;
	OnTextEntered = 0;
	FloatFilter = false;

	TrackBar = false;   Clicked = false;
	iMinVal  = -999999; iMaxVal = 999999;
	fMinVal  = -999999; fMaxVal = 999999.f;
	//
	TrackWay = 0; TrackStep = 2;
}

//
float GetTextWidth(FTGLfont *Font, wstring Text) {
	int n  = Text.size();
	string EncodeText = wstring2string(Text);
	//
	float bounds[6];
	
	ftglGetFontBBox(Font, EncodeText.c_str(), n, bounds);
	float twidth = bounds[3]-bounds[0];
	if (Text[n-1] == ' ') twidth += 3; // Edit
	//
	return twidth;
}

//
void UIEdit::Draw (int x, int y) {
	//
	if (Transparent) {  DrawAlign = alCenter; }
	if (!Enabled || (Parent != 0 && !Parent->Enabled))
		glUniform1f (TexAtlasShader->Locations->GetData("Alpha"), DisabledAlpha);
	//
	if (Align == alRight)
		x += Parent->Width-Width-Parent->Padding[0]-Parent->ScrollWidth;
	//
	if (Align == alCenter)
		Width = Parent->Width-(Parent->Padding[0] << 1)+1-Parent->ScrollWidth;
	//
	if (Left != x || Top != y)  {
		sx = iWidths[9];
		tx = x+iWidths[12]+iWidths[12];
		if (DrawAlign == alLeft) tx += iWidths[12]+3;
		//
		Left = x; Top = y;
	}
	AbsoluteLeft = x; AbsoluteTop = y;
	//
	tx = tx+toffset;
	//if (TrackBar) tx = Left;
	ty = WindowHeight-Top-(iHeights[1] >> 1)-4;
	sy = y+((iHeights[1]-iHeights[12]) >> 1);
	// Flickering sticks

	if (TrackBar) {
		tx = Left;
		//float bounds[6];
		//ftglGetFontBBox(Theme->Font, s.c_str(), 1, bounds);
		int twidth;

		if (!Focused) twidth = GetTextWidth (Theme->Font, Before+wText+After);
		else twidth = GetTextWidth (Theme->Font, wText);

		//LOG_DEBUG ("TEXT WIDTH : %d\n", tidth)
		/*if (DrawAlign == alCenter) tx += (Width+iWidths[n+2]+iWidths[n]-twidth) >> 1;
		else if (DrawAlign == alRight) tx += (Width+iWidths[n+2]-twidth) >> 1;
		else if (DrawAlign == alLeft) tx += (Width-iWidths[n]-twidth) >> 1;
		else tx += (Width-twidth) >> 1;*/
		tx += (cWidth-twidth) >> 1;
	}

	if (Focused) StickCounter += DeltaTime;
	if (StickCounter > 0.0005f) {
		StickCounter = 0;
		Stick = !Stick;
	}
	
	// Draw
	
	int n  = 0;
	int tn = 0;
	int to = 0;
	
	if (Enter) { n = 3; tn = 1; to = 2; }
	if (Click) { n = 6; tn = 2; to = 4; }
	
	Height = iHeights[n+1];
	cWidth = Width-iWidths[n]-1;
	//
	if (DrawAlign == alLeft)   Left   += iWidths[n]+iWidths[n+2];
	if (DrawAlign == alCenter) cWidth -= iWidths[n]-iWidths[n]-iWidths[n+2]-iWidths[n+2];
	//

	if (Transparent) { goto DrawContent; }
	if (DrawAlign == alLeft || DrawAlign == alAll) {
		//
		glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(LeftElement->MVPMatrix[0][0]));
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [n], Heights [n]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[n], OffsetsY[n]);
		//
		LeftElement->SetPosition (glm::vec2(Left, WindowHeight-iHeights[n]-Top));
		LeftElement->SetSize (glm::vec2(iWidths[n], iHeights[n]));
		LeftElement->BaseRender();
	}
	//
	glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(MiddleElement->MVPMatrix[0][0]));
	glUniform2f (Shader->Locations->GetData("Size")  , Widths  [n+1], Heights [n+1]);
	glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[n+1], OffsetsY[n+1]);
	//
	MiddleElement->SetPosition (glm::vec2(Left+iWidths[n+1], WindowHeight-iHeights[n+1]-Top));
	MiddleElement->SetSize (glm::vec2(cWidth-iWidths[n+2], iHeights[n+1]));
	MiddleElement->BaseRender();
	//
	if (DrawAlign == alRight || DrawAlign == alAll) {
		//
		glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(RightElement->MVPMatrix[0][0]));
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [n+2], Heights [n+2]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[n+2], OffsetsY[n+2]);
		//
		RightElement->SetPosition (glm::vec2(Left+cWidth, WindowHeight-iHeights[n+2]-Top));
		RightElement->SetSize (glm::vec2(iWidths[n+2], iHeights[n+2]));
		RightElement->BaseRender();
	}
	//
	if (Focused) {
		//
		if (DrawAlign == alLeft || DrawAlign == alAll) {
			//
			glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(LeftElement->MVPMatrix[0][0]));
			glUniform2f (Shader->Locations->GetData("Size")  , Widths  [9], Heights [9]);
			glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[9], OffsetsY[9]);
			//
			LeftElement->SetPosition (glm::vec2(Left, WindowHeight-iHeights[9]-Top));
			LeftElement->SetSize (glm::vec2(iWidths[9], iHeights[9]));
			LeftElement->BaseRender();
		}
		//
		glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(MiddleElement->MVPMatrix[0][0]));
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [10], Heights [10]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[10], OffsetsY[10]);
		//
		MiddleElement->SetPosition (glm::vec2(Left+iWidths[10], WindowHeight-iHeights[10]-Top));
		MiddleElement->SetSize (glm::vec2(cWidth-iWidths[11], iHeights[10]));
		MiddleElement->BaseRender();
		//
		if (DrawAlign == alRight || DrawAlign == alAll) {
			//
			glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(RightElement->MVPMatrix[0][0]));
			glUniform2f (Shader->Locations->GetData("Size")  , Widths  [11], Heights [11]);
			glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[11], OffsetsY[11]);
			//
			RightElement->SetPosition (glm::vec2(Left+cWidth, WindowHeight-iHeights[11]-Top));
			RightElement->SetSize (glm::vec2(iWidths[11], iHeights[11]));
			RightElement->BaseRender();
		}
		//
	}
	//
	if (TrackBar) {
		n = 0;
		if (Enter) { n = 1; }
		//
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [13+n], Heights [13+n]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[13+n], OffsetsY[13+n]);
		//
		if (DrawAlign == alAll || DrawAlign == alLeft)
			LeftArrow->SetPosition (glm::vec2(Left+iWidths[13+n], WindowHeight-iHeights[13+n]-Top-(iHeights[13+n]>>1)));

		if (DrawAlign == alRight || DrawAlign == alCenter)
			LeftArrow->SetPosition (glm::vec2(Left+iWidths[13+n]+iWidths[0], WindowHeight-iHeights[13+n]-Top-(iHeights[13+n]>>1)));

		//LeftArrow->SetPosition (glm::vec2(Left+iWidths[13+n], WindowHeight-iHeights[13+n]-Top-(iHeights[13+n]>>1)));
		LeftArrow->SetSize (glm::vec2(iWidths[13+n], iHeights[13+n]));
		LeftArrow->BaseRender();
		//
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [15+n], Heights [15+n]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[15+n], OffsetsY[15+n]);
		//

		if (DrawAlign == alAll || DrawAlign == alRight)
			RightArrow->SetPosition (glm::vec2(Left+Width-(iWidths[15+n]<<1)-1, WindowHeight-iHeights[15+n]-Top-(iHeights[15+n]>>1)));

		if (DrawAlign == alLeft || DrawAlign == alCenter)
			RightArrow->SetPosition (glm::vec2(Left+Width-(iWidths[15+n]<<1)-iWidths[11]-1, WindowHeight-iHeights[15+n]-Top-(iHeights[15+n]>>1)));

		RightArrow->SetSize (glm::vec2(iWidths[15+n], iHeights[15+n]));
		RightArrow->BaseRender();
	}
	//
DrawContent:
	//glEnable(GL_SCISSOR_TEST);
	int sx, sy, sw, sh;
	
	/*if (DrawAlign == alRight)
		glScissor(Left+iWidths[10]+iWidths[n], WindowHeight-iHeights[11]-Top, cWidth-iWidths[10], WindowHeight-Top);
	else if (DrawAlign == alCenter)
		glScissor(Left+iWidths[10]+iWidths[n], WindowHeight-iHeights[11]-Top, cWidth-iWidths[10]-iWidths[n]-iWidths[n+2], WindowHeight-Top);
	else if (DrawAlign == alRight)
		glScissor(Left+iWidths[10], WindowHeight-iHeights[11]-Top, cWidth-iWidths[10]-iWidths[n+2], WindowHeight-Top);
	else glScissor(Left+iWidths[10], WindowHeight-iHeights[11]-Top, cWidth-iWidths[10], WindowHeight-Top);*/

	if (DrawAlign == alRight) {
		sx = Left+iWidths[10]+iWidths[n];    sw = cWidth-iWidths[10];
		sy = WindowHeight-iHeights[11]-Top;  sh = WindowHeight-Top;
	} else if (DrawAlign == alCenter) {
		sx = Left+iWidths[10]+iWidths[n];    sw = cWidth-iWidths[10]-iWidths[n]-iWidths[n+2];
		sy = WindowHeight-iHeights[11]-Top;  sh = WindowHeight-Top;
	} else if (DrawAlign == alRight) {
		sx = Left+iWidths[10];               sw = cWidth-iWidths[10]-iWidths[n+2];
		sy = WindowHeight-iHeights[11]-Top;  sh = WindowHeight-Top;
	} else {
		sx = Left+iWidths[10];               sw = cWidth-iWidths[10];
		sy = WindowHeight-iHeights[11]-Top;  sh = WindowHeight-Top;
	}
	//

	Manager->PushScissor(sx, sy, sw, sh);

	// Draw sel rect
	if (SelStart != SelEnd && Focused) {
		int twidth = GetTextWidth(Theme->Font, wText.substr(0, SelStart));
		int selx1, selx2;
		selx1 = tx+twidth+(iWidths[12] >> 1);
		
		twidth = GetTextWidth(Theme->Font, wText.substr(0, SelEnd));
		selx2 = tx+twidth+(iWidths[12] >> 1);
		
		// swap
		if (selx1 > selx2) {
			int sss = selx1;
			selx1 = selx2;
			selx2 = sss;
		}
		
		// Draw Selection Rect
		// Quad
		Shader->UnApplyShader();
		ColorShader->ApplyShader();
		//
		glUniformMatrix4fv (ColorShader->Locations->GetData("MVP")  , 1, GL_FALSE, &(SelectRectElement->MVPMatrix[0][0]));
		glUniform4fv       (ColorShader->Locations->GetData("Color"), 1, &RectColor[0]); 
		//
		SelectRectElement->SetPosition (glm::vec2(selx1-1, WindowHeight-Top+RectOffset[0]-iHeights[9]));
		SelectRectElement->SetSize (glm::vec2(selx2-selx1+1, iHeights[9]-RectOffset[1]-RectOffset[0]));
		SelectRectElement->BaseRender();

		ColorShader->UnApplyShader();
		Shader->ApplyShader();
		/*glBegin(GL_QUADS);
		
		glVertex2f(selx1, ScreenHeight-Top-RectOffset[0]);
		glVertex2f(selx2, ScreenHeight-Top-RectOffset[0]);
		glVertex2f(selx2, ScreenHeight-iHeights[9]-Top+RectOffset[1]);
		glVertex2f(selx1, ScreenHeight-iHeights[9]-Top+RectOffset[1]);
		
		glEnd();*/
	}

	//
	int twidth = GetTextWidth(Theme->Font, wText.substr(0, sPos));
	sx = twidth;
	//
	if (sx+tx > Left+cWidth-iWidths[11]-iWidths[12]) {
		toffset -= (sx+tx)-(Left+cWidth-iWidths[11]-iWidths[12]);
	}
	//
	if (sx+tx < Left+iWidths[12]) {
		toffset += (Left+iWidths[12])-(sx+tx);
		//toffset = Left-twidth+iWidths[12];
	}
	if (Stick && Focused) {
		//
		glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(StickElement->MVPMatrix[0][0]));
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [12], Heights [12]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[12], OffsetsY[12]);
		//
		StickElement->SetPosition (glm::vec2(tx+sx, WindowHeight-iHeights[12]-Top));
		StickElement->SetSize (glm::vec2(iWidths[12], iHeights[12]));
		StickElement->BaseRender();
		//
	}
	//
	if (!Focused) {
		sPos = 0;
		SelStart = 0;
		SelEnd = 0;
	}
	//
	string s  = wstring2string(wText);
	string at = " "+wstring2string(After);
	string bt = wstring2string(Before)+" ";

	if (!Focused)
		s = bt+s+at;

	Shader->UnApplyShader();
	glBegin2D();
	//
	
	if (TextShadows[tn]) {
		glColor3f(ShadowColors[n], ShadowColors[n+1], ShadowColors[n+2]);
		glRasterPos2f(tx+ShadowOffsets[to], ty+ShadowOffsets[to+1]);
		ftglRenderFont(Theme->Font, s.c_str(), RENDER_ALL);
	}
	//
	glActiveTexture(GL_TEXTURE0);

	if (Transparent) {
		if (Enter) glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		else glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	} else glColor3f(TextColors[n], TextColors[n+1], TextColors[n+2]);

	glTranslatef(tx+TextOffsets[to], ty+TextOffsets[to+1], 0);
	ftglRenderFont(Theme->Font, s.c_str(), RENDER_ALL);

	Manager->PopScissor();
	//
	glEnd2D();
	Shader->ApplyShader();
	//
	if (!Enabled || (Parent != 0 && !Parent->Enabled))
		glUniform1f (TexAtlasShader->Locations->GetData("Alpha"), 1.0f);
}

//
inline void UIEdit::MoveStickLeft() {
	sPos--;
	
	wchar_t wch = wText[sPos];
	string ss = DecodeUTF8(wch);
	
	float bounds[6];
	ftglGetFontBBox(Theme->Font, ss.c_str(), 1, bounds);
	int twidth = bounds[3]-bounds[0];
	if (wch == ' ') twidth += 3; // Edit
	
	sx -= twidth;
	
	if (sx < Left+iWidths[12]-iWidths[9]) {
		sx = Left+iWidths[12]-iWidths[9];
	}
	
	StickCounter = 0;
	Stick = true;
}

inline void UIEdit::MoveStickRight() {
	sPos++;
	
	wchar_t wch = wText[sPos-1];
	string ss = DecodeUTF8(wch);
	
	float bounds[6];
	ftglGetFontBBox(Theme->Font, ss.c_str(), 1, bounds);
	int twidth = bounds[3]-bounds[0];
	if (wch == ' ') twidth += 3; // Edit
	
	sx += twidth;
	if (sx > Left+cWidth-iWidths[11]-iWidths[12]) sx = Left+cWidth-iWidths[11]-iWidths[12];
	
	StickCounter = 0;
	Stick = true;
}

// Events
void UIEdit::KeyPressed(Uint16 key) {
	//
	if (!Focused) return;
	
	wstring s1;
	wstring s2;

	int sbegin = MIN (SelStart, SelEnd);
	int send   = MAX (SelStart, SelEnd);
	
	if (key == keyC && isKey[keyCtrl]) CopyText();
	if (key == keyV && isKey[keyCtrl]) PasteText();
	if (key == keyDelete) {
		if (SelStart == SelEnd) {
			if (sPos >= wText.size()) return;
			s1 = wText.substr(0, sPos);
			s2 = wText.substr(sPos+1, wText.size());
		} else {
			s1 = wText.substr(0, sbegin);
			s2 = wText.substr(send, wText.size());
			lPos = sPos;
			sPos = SelEnd = SelStart = sbegin;
		}

		wText  = s1+s2;
		StickCounter = 0;
		Stick = true;

		return;
	}
	
	if (key == keyBackspace) {
		if (sPos == 0) return;

		if (SelStart == SelEnd) {
			s1 = wText.substr(0, sPos-1);
			s2 = wText.substr(sPos, wText.size());

			lPos = sPos;
			sPos--;
		} else {
			s1 = wText.substr(0, sbegin);
			s2 = wText.substr(send, wText.size());
			lPos = sPos;
			sPos = SelEnd = SelStart = sbegin;
		}

		wText  = s1+s2;
		StickCounter = 0;
		Stick = true;
		return;
	}
	
	// Move Stick
	if (key == keyLeftArrow) {
		if (sPos != 0) {
			lPos = sPos;
			sPos--;

			if (isKey[keyShift]) {
				if (SelStart == SelEnd)
					SelStart = lPos;

				SelEnd = sPos;
			} else SelEnd = SelStart = sPos;
		}
		
		StickCounter = 0;
		Stick = true;
		return;
	}
	
	if (key == keyRightArrow) {
		if (sPos != wText.size()) {
			lPos = sPos;
			sPos++;

			if (isKey[keyShift]) {
				if (SelStart == SelEnd)
					SelStart = lPos;

				SelEnd = sPos;
			} else SelEnd = SelStart = sPos;
		}
		
		StickCounter = 0;
		Stick = true;
		return;
	}
	
	if (key == keyHome) {
		lPos = sPos;
		sPos = 0;

		if (isKey[keyShift]) {
			if (SelStart == SelEnd)
				SelStart = lPos;
			
			SelEnd = sPos;
		} else SelEnd = SelStart = sPos;
		
		StickCounter = 0;
		Stick = true;
		return;
	}
	
	if (key == keyEnd) {
		lPos = sPos;
		sPos = wText.size();

		if (isKey[keyShift]) {
			if (SelStart == SelEnd)
				SelStart = lPos;
			
			SelEnd = sPos;
		} else SelEnd = SelStart = sPos;
		
		StickCounter = 0;
		Stick = true;
		return;
	}
	//
}

//
void UIEdit::TextEntered (Uint16 key) {
	//
	if (!Focused) return;
	//
	
	Uint8 b1 = key;
	Uint8 b2 = key >> 8;
	
	wchar_t wch = key;
	
	if (((b2 != 0) | (b1 >= 0x20) & (b1 <= 0x7E)) == 0) return;
	if (IntFilter && b1 != '-' && (b1 < '0' || b1 > '9')) return;
	if (FloatFilter && b1 != '.' && b1 != '-' && (b1 < '0' || b1 > '9')) return;
	
	wstring s1;
	wstring s2;

	int sbegin = MIN (SelStart, SelEnd);
	int send   = MAX (SelStart, SelEnd);

	if (SelStart == SelEnd) {
		s1 = wText.substr(0, sPos);
		s2 = wText.substr(sPos, wText.size());
	} else {
		s1 = wText.substr(0, sbegin);
		s2 = wText.substr(send, wText.size());
		lPos = sPos;
		sPos = SelEnd = SelStart = sbegin;
	}

	wText  = s1;
	wText += wch;
	wText += s2;
	//
	//if (IntFilter && WStrToInt(wText) > iMaxVal) wText = IntToWStr(iMaxVal);
	//if (IntFilter && WStrToInt(wText) < iMinVal) wText = IntToWStr(iMinVal);
	//
	StickCounter = 0;
	Stick = true;
	//
	lPos = sPos;
	sPos++;
	//
	if (OnTextEntered != 0)
		OnTextEntered(this, key);
}

void UIEdit::Resized(int Width, int Height) {
}

void UIEdit::SetStickPos(int x, int y) {
	//
	int bmax = x-tx;
	int bwidth = tx;
	wchar_t wc;
	int twidth = GetTextWidth(Theme->Font, wText);

	if (x > tx+twidth) sPos = wText.size();
	else
	//
	for (int i = 0; i < wText.size(); i++) {
		wstring wss = wText.substr(0, i);
		int slength  = wss.size();
		string ss = wstring2string(wss);
		//
		float bounds[6];
		
		ftglGetFontBBox(Theme->Font, ss.c_str(), slength, bounds);
		if (wText[slength-1] == ' ') bounds[3] += 3; // Edit
	
		if (bounds[3]-bounds[0] > bmax) { sPos = i; break; }
	}
	
	StickCounter = 0;
	Stick = true;
}

// Mouse down
void UIEdit::MouseDown (int x, int y, int Shift) {
	if (!Enter) return;
	SetStickPos (x, y);
	SelStart = SelEnd = sPos;
	Clicked = PointInRect(x, y, AbsoluteLeft+14, AbsoluteTop, Width-28, 20);
	//HideCursor = TrackBar && Clicked;

	lx = x; dx = 0;
	ly = y;
}

void UIEdit::MouseUp (int x, int y, int Shift) {
	Clicked = false; FreezUI = false;
	if (TrackBar && dx != 0) {
		HideCursor = false;
		dx         = 0;
		//
		return;
	}

	if (TrackBar && PointInRect(x, y, AbsoluteLeft, AbsoluteTop, 14, 20)) {
		int val = WStrToInt (wText);
		val -= 1;
		clamp (&val, iMinVal, iMaxVal);
		wText = IntToWStr(val);
		
		if (OnChange != 0) OnChange (this);
		return;
	}

	if (TrackBar && PointInRect(x, y, AbsoluteLeft+Width-14, AbsoluteTop, 14, 20)) {
		int val = WStrToInt (wText);
		val += 1;
		clamp (&val, iMinVal, iMaxVal);
		wText = IntToWStr(val);

		if (OnChange != 0) OnChange (this);
		return;
	}

	if (Enter) {
		if (Focused) return;
		SetFocused();

		lPos = sPos;
		sPos = wText.size();
		
		StickCounter = 0;
		Stick = true;
		
		SelStart = 0;
		SelEnd = sPos;
	} else {
		if (IntFilter && WStrToInt(wText) > iMaxVal) wText = IntToWStr(iMaxVal);
		if (IntFilter && WStrToInt(wText) < iMinVal) wText = IntToWStr(iMinVal);

		if (OnChange != 0 && Focused) OnChange (this);
		UnFocused();
	}
}

//
void UIEdit::MouseMove (int x, int y, int Shift) {
	//
	if (Shift != mouseLeft) return;
	if (TrackBar && Clicked && !Focused) {
		int dtw = x-lx;
		int val = WStrToInt (wText);
		TrackWay += dtw;
		FreezUI = true;

		if (TrackWay >=  TrackStep) { val++; dx++; TrackWay = 0; }
		if (TrackWay <= -TrackStep) { val--; dx++; TrackWay = 0; }

		clamp (&val, iMinVal, iMaxVal);
		wText = IntToWStr(val);

		if (OnChange != 0) OnChange (this);
		if (dtw >= 2 || dtw <= -2) {
			HideCursor = true;
			sf::Mouse::setPosition (sf::Vector2i (lx, ly), *MainWindow);
		}
	}
	//
	//lx = x;

	if (!Enter) return;
	
	SetStickPos (x, y);
	SelEnd = sPos;
}

//
void UIEdit::DblClick  (int x, int y, int Shift) {
	if (!Enter) return;
	if (Shift != mouseLeft) return;
	
	int start = sPos;
	if (sPos == wText.size()) start = sPos-1;
	for (int i = start; i <= wText.size(); i++) {
		SelEnd = i;
		bool next = true;

		for (int j = 0; j < ARRAYSIZE(SplitChars); j++)
			if (wText[i] == SplitChars[j]) {
				next = false;
				break;
			}

		if (!next) {
			SelEnd = i;
			break;
		}
	}
	//
	for (int i = start; i >= 0; i--) {
		SelStart = i;
		bool next = true;

		for (int j = 0; j < ARRAYSIZE(SplitChars); j++)
			if (wText[i] == SplitChars[j]) {
				next = false;
				break;
			}

		if (!next) {
			SelStart = i+1;
			break;
		}
	}

	if (SelStart == wText.size()) SelStart = wText.size()-1;
	sPos = SelEnd;
}

//
void UIEdit::CopyText() {
	if (SelStart == SelEnd) return;

	int sbegin = MIN (SelStart, SelEnd);
	int send   = MAX (SelStart, SelEnd);
	//
	wstring text = wText.substr(sbegin, send);
	fs::AddTextToClipboard(wstring2string(text));
}

//
void UIEdit::PasteText() {
	wstring s1;
	wstring s2;

	int sbegin = MIN (SelStart, SelEnd);
	int send   = MAX (SelStart, SelEnd);

	if (SelStart == SelEnd) {
		s1 = wText.substr(0, sPos);
		s2 = wText.substr(sPos, wText.size());
	} else {
		s1 = wText.substr(0, sbegin);
		s2 = wText.substr(send, wText.size());
		lPos = sPos;
		sPos = SelEnd = SelStart = sbegin;
	}

	wstring text = StrToWStr(fs::GetTextFromClipboard());
	wText  = s1;
	wText += text;
	wText += s2;
	//
	StickCounter = 0;
	Stick = true;
	//
	lPos = sPos;
	sPos += text.size();
}

#endif
