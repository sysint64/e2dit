#ifndef CPP_GROUPED
#define CPP_GROUPED

//
//----------------------------------------------------------------------------//
// GroupedElements (UI Element)
//----------------------------------------------------------------------------//

UIGroupedElements::~UIGroupedElements() {
	//
	for (int i = 0; i < Elements.size(); i++)
		delete Elements[i];
	
	delete SplitElement;
	//free(Elements);
}

void UIGroupedElements::Init() {
	//* Inherited
	//UICheckButton::Init();
	if (Inited) return;
	Inited = true;
	
	// Split
	Splits [0] = Theme->Data["general"].Func["split"][0].Num / Theme->Skin->Width;
	Splits [1] = Theme->Data["general"].Func["split"][1].Num / Theme->Skin->Width;
	Splits [2] = Theme->Data["general"].Func["split"][2].Num / Theme->Skin->Width;
	Splits [3] = Theme->Data["general"].Func["split"][3].Num / Theme->Skin->Width;
	iSplits[0] = floor(Theme->Data["general"].Func["split"][2].Num);
	iSplits[1] = floor(Theme->Data["general"].Func["split"][3].Num);
	//---
	if (Parent != 0) {
		GenWidth = Parent->Padding[0];
		GW0 = Parent->Padding[0]*2-10;
		if (Align == alCenter) GenWidth = Parent->Padding[0]*2-5;
		CountFS = 0;
		Clicked = false;
		//
		for (int i = 0; i < Elements.size(); i++) {
			//
			if (!dynamic_cast<UIDropMenu*> (Elements[i])) {
				Elements[i]->DrawAlign = alCenter;
			} else {
				if (!((UIDropMenu*) Elements[i])->isMenu)
					Elements[i]->DrawAlign = alCenter;
			}
			//
			Elements[i]->Shader = Shader;
			Elements[i]->Theme  = Theme;
			Elements[i]->Manager = Manager;
			//
			if (!(Align == alCenter && !Elements[i]->FixSize)) GenWidth += Elements[i]->Width;
			else CountFS++;
			//
			if (Height < Elements[i]->Height)
				Height = Elements[i]->Height;
		}
		Height = 21;
		GenWidth -= 5;
		//
		if (Elements.size() != 0 && Elements.size() != 1) {
			if (!dynamic_cast<UIDropMenu*> (Elements[0])) {
				Elements[0]->DrawAlign = alLeft;
			} else {
				if (!((UIDropMenu*) Elements[0])->isMenu)
					Elements[0]->DrawAlign = alLeft;
			}
			//
			if (dynamic_cast <UIButton*> (Elements[0])) {
				if (Align == alCenter && Elements[Elements.size()-1]->FixSize)	
					GenWidth += 5;
			}
			//
			if (!dynamic_cast<UIDropMenu*> (Elements[Elements.size()-1])) {
				Elements[Elements.size()-1]->DrawAlign = alRight;
			} else {
				if (!((UIDropMenu*) Elements[Elements.size()-1])->isMenu)
					Elements[Elements.size()-1]->DrawAlign = alRight;
			}
			//
			if (dynamic_cast <UIButton*> (Elements[Elements.size()-1])) {
				if (Align == alCenter && Elements[Elements.size()-1]->FixSize)	
					GenWidth += 5;
			}
		}
	}
	//
	for (int i = 0; i < Elements.size(); i++) {
		Elements[i]->Init();
	}
	//
	mx = 0;
	my = 0;
	mShift = mouseNone;
	//
	SplitElement = new BaseObject();
	SplitElement->Init();
	//
	SplitElement->Data    = GUIElementDat;
	SplitElement->mCamera = ScreenCamera;
	SplitElement->Shader  = Shader;
}

//
void UIGroupedElements::RenderSplit(const int x) {
	//
	if (dynamic_cast<UIDropMenu*> (Elements[0]) && ((UIDropMenu*) Elements[0])->isMenu)
		return;

	if (dynamic_cast<UICheckBox*> (Elements[0]))
		return;
	//
	glUniform2f (Shader->Locations->GetData("Size")  , Splits[2], Splits[3]);
	glUniform2f (Shader->Locations->GetData("Offset"), Splits[0], Splits[1]);
	//
	SplitElement->SetPosition (glm::vec2(x+iSplits[0], WindowHeight-iSplits[1]-Top));
	SplitElement->SetSize (glm::vec2(iSplits[0], iSplits[1]));
	SplitElement->BaseRender();
}

//
void UIGroupedElements::Draw (int x, int y) {
	//
	if (!Enabled || (Parent != 0 && !Parent->Enabled))
		glUniform1f (TexAtlasShader->Locations->GetData("Alpha"), 0.5f);

	if (Align == alRight)
		x += Parent->Width-GenWidth-15-Parent->ScrollWidth;
		//
	if (Align == alCenter) {
		//
		int cw = 0;
		int fi = 0;
		vector<int> fis;
		//
		for (int i = 0; i < Elements.size(); i++) {
			//
			if (!Elements[i]->FixSize) {
				//if (CountFS != 0) Elements[i]->Width = (Parent->Width-GenWidth-Parent->ScrollWidth) >> (CountFS >> 1);
				//else Elements[i]->Width = (Parent->Width-GenWidth-Parent->ScrollWidth);
				Elements[i]->Width = (Parent->Width-Parent->ScrollWidth)/Elements.size();

				fi = i;
				fis.push_back(i);
			}
			//
			cw += Elements[i]->Width;
		}
		//
		//if (cw < Parent->Width-GW0-Parent->ScrollWidth) Elements[fi]->Width += (Parent->Width-GW0-Parent->ScrollWidth)-cw;
		//if (cw > Parent->Width-GW0-Parent->ScrollWidth) Elements[fi]->Width -= cw-(Parent->Width-GW0-Parent->ScrollWidth);
		for (int i = 0; i < fis.size(); i++) {
			if (cw < Parent->Width-GW0-Parent->ScrollWidth) {
				Elements[fis[i]]->Width += ((Parent->Width-GW0-Parent->ScrollWidth)-cw)/fis.size();
				cw += ((Parent->Width-GW0-Parent->ScrollWidth)-cw)/fis.size();
			}

			if (cw > Parent->Width-GW0-Parent->ScrollWidth) {
				Elements[fis[i]]->Width -= (cw-(Parent->Width-GW0-Parent->ScrollWidth))/fis.size();
				cw -= (cw-(Parent->Width-GW0-Parent->ScrollWidth))/fis.size();
			}
		}

		if (cw < Parent->Width-GW0-Parent->ScrollWidth) Elements[fi]->Width += (Parent->Width-GW0-Parent->ScrollWidth)-cw;
		if (cw > Parent->Width-GW0-Parent->ScrollWidth) Elements[fi]->Width -= cw-(Parent->Width-GW0-Parent->ScrollWidth);
	}
	//
	Left = x; Top = y;
	AbsoluteLeft = x; AbsoluteTop = y;
	//
	if (Elements.size() <= 1) return;
	//
	Elements[0]->Draw(Left, Top);
	int ex = Left+Elements[0]->Width;
	//RenderSplit(ex);
	
	for (int i = 1; i < Elements.size()-1; i++) {
		Elements[i]->Draw(ex, Top);
		ex += Elements[i]->Width;
		//RenderSplit(ex);
	}
	//
	Width = ex-Left+Elements[Elements.size()-1]->Width;
	Elements[Elements.size()-1]->Draw(ex, Top);
	//
	ex = Left+Elements[0]->Width;
	
	if (!Transparent) {
		if (!Elements[0]->Enabled) glUniform1f (TexAtlasShader->Locations->GetData("Alpha"), 0.5f);
		RenderSplit(ex);
		if (!Elements[0]->Enabled) glUniform1f (TexAtlasShader->Locations->GetData("Alpha"), 1.0f);
	}

	for (int i = 1; i < Elements.size()-1; i++) {
		ex += Elements[i]->Width;
		
		if (!Transparent) {
			if (!Elements[i]->Enabled) glUniform1f (TexAtlasShader->Locations->GetData("Alpha"), 0.5f);
			RenderSplit(ex);
			if (!Elements[i]->Enabled) glUniform1f (TexAtlasShader->Locations->GetData("Alpha"), 1.0f);
		}

	}
	//
	bool isCheck = dynamic_cast<UICheckButton*>(Elements[0]);
	//
	ex = Left;
	Cursor = curNormal;
	//
	if (!DialogOpened || inDialog)
	if (PointInRect(mx, my, ex+10, Top, Elements[0]->Width+1-10, Elements[0]->Height) && Elements[0]->Enabled) {
		Elements[0]->Enter = true;
		Cursor = Elements[0]->Cursor;
		
		/*if (mShift == mouseLeft && (!Clicked || Elements[0]->Click)) {
			//
			Clicked = true;
			if (isCheck)
			for (int k = 0; k < Count; k++) {
				if (dynamic_cast<UICheckButton*>(Elements[k]))
					((UICheckButton*)Elements[k])->Checked = false;
			}
			
			if (!isCheck) Elements[0]->Click = true;
			else ((UICheckButton*)Elements[0])->Checked = true;
		} else Elements[0]->Click = false;*/
	} else {
		Elements[0]->Enter = false;
		Elements[0]->Click = false;
	}
	
	ex += Elements[0]->Width;
	
	//
	if (!DialogOpened || inDialog)
	for (int i = 1; i < Elements.size()-1; i++) {
		//
		isCheck = dynamic_cast<UICheckButton*>(Elements[i]);
		//
		if (PointInRect(mx, my, ex+2, Top, Elements[i]->Width-1, Elements[0]->Height) && Elements[i]->Enabled) {
			Elements[i]->Enter = true;
			Cursor = Elements[i]->Cursor;
			
			/*if (mShift == mouseLeft && (!Clicked || Elements[i]->Click)) {
				//
				Clicked = true;
				if (isCheck)
				for (int k = 0; k < Count; k++) {
					if (dynamic_cast<UICheckButton*>(Elements[k]))
						((UICheckButton*)Elements[k])->Checked = false;
				}
			
				if (!isCheck) Elements[i]->Click = true;
				else ((UICheckButton*)Elements[i])->Checked = true;
			} else Elements[i]->Click = false;*/
		} else {
			Elements[i]->Enter = false;
			Elements[i]->Click = false;
		}
		
		ex += Elements[i]->Width;
	}
	//
	isCheck = dynamic_cast<UICheckButton*>(Elements[Elements.size()-1]);
	//
	if (!DialogOpened || inDialog)
	if (PointInRect(mx, my, ex+2, Top, Elements[Elements.size()-1]->Width-3, Elements[0]->Height) && Elements[Elements.size()-1]->Enabled) {
		Elements[Elements.size()-1]->Enter = true;
		Cursor = Elements[Elements.size()-1]->Cursor;
		
		/*if (mShift == mouseLeft && (!Clicked || Elements[Count-1]->Click)) {
			//
			Clicked = true;
			if (isCheck)
			for (int k = 0; k < Count; k++) {
				if (dynamic_cast<UICheckButton*>(Elements[k]))
					((UICheckButton*)Elements[k])->Checked = false;
			}
			
			if (!isCheck) Elements[Count-1]->Click = true;
			else ((UICheckButton*)Elements[Count-1])->Checked = true;
		} else Elements[Count-1]->Click = false;*/
	} else {
		Elements[Elements.size()-1]->Enter = false;
		Elements[Elements.size()-1]->Click = false;
	}
	//
	if (!Enabled || (Parent != 0 && !Parent->Enabled))
		glUniform1f (TexAtlasShader->Locations->GetData("Alpha"), 1.0f);
}

// Events

//Events
void UIGroupedElements::DblClick  (int x, int y, int Shift) {
	//
	for (int i = 0; i < Elements.size(); i++) {
		if (!Elements[i]->Visible)
			continue;
		
		Elements[i]->DblClick(x, y, Shift);
	}
}

void UIGroupedElements::MouseMove (int x, int y, int Shift) {
	mx = x;
	my = y;
	//
	for (int i = 0; i < Elements.size(); i++) {
		Elements[i]->MouseMove(x, y, Shift);
	}
}

void UIGroupedElements::MouseDown (int x, int y, int Shift) {
	//
	mShift = Shift;
	bool setfocused = false;
	//
	//if (!DialogOpened || inDialog)
	//
	int ex = Left;
	for (int i = 0; i < Elements.size(); i++) {
		bool isCheck = dynamic_cast<UICheckButton*>(Elements[i]);
		//
		if (Elements[i]->Enter) {
			
				//
				Clicked = true;
				//if (isCheck)
				for (int k = 0; k < Elements.size(); k++) {
					if (dynamic_cast<UICheckButton*>(Elements[k]))
						((UICheckButton*)Elements[k])->Checked = false;
				}
			
				if (!isCheck) Elements[i]->Click = true;
				else ((UICheckButton*)Elements[i])->Checked = true;
		}
		
		ex += Elements[i]->Width;
		Elements[i]->MouseDown(x, y, Shift);
	}
}

void UIGroupedElements::MouseUp (int x, int y, int Shift) {
	//
	for (int i = 0; i < Elements.size(); i++) {
		Elements[i]->MouseUp(x, y, Shift);
	}
	
	mShift = mouseNone;
	Clicked = false;
}

// Keyboard
void UIGroupedElements::KeyPressed (Uint16 key) {
	//
	for (int i = 0; i < Elements.size(); i++) {
		Elements[i]->KeyPressed(key);
	}
}

void UIGroupedElements::TextEntered (Uint16 key) {
	//
	for (int i = 0; i < Elements.size(); i++) {
		Elements[i]->TextEntered(key);
	}
}

//* UIPANEL

UIPanel::UIPanel() {
	//
	AllowResize = true;   ShowScroll = true;
	CanHide		= false;  isOpened	 = true;
	UseInColor	= false;  DrawQuad	 = true;
	//
	Join = 0; TabJoin = 0; UseActionColor = false;
	ElemsOffset = 0; Clicked = false;
}

UIPanel::~UIPanel() {
	delete QuadElement;
	delete Split;
	delete Arrow;

	for (int i = 0; i < 6; i++) {
		delete ScrollBg [i];
		delete ScrollBtn[i];
	}
}
//
void UIPanel::UpdateAlign(int *x, int *y) {
	int ParentWidth  = WindowWidth;
	int ParentHeight = WindowHeight;
	//
	int x0 = 0; int mWidth  = 0;
	int y0 = 0; int mHeight = 0;
	//
	if (Parent != 0) {
		ParentWidth  = Parent->Width;
		ParentHeight = Parent->Height;
		//
		int mWidth  = 0;
		int mHeight = 0;
		int mTop	= 0;
		int mLeft	= 0;
		//
		//
		for (int i = 0; i < Id; i++) {
			//
			//break;
			//if (!dynamic_cast<UIPanel*> (Parent))
			//	break;
			//
			if (!((UIPanel*)Parent)->Elements[i]->Visible)
				continue;
			//
			if (((UIPanel*)Parent)->Elements[i]->Align == alNone)
				continue;
			//
			switch (((UIPanel*)Parent)->Elements[i]->Align) {
				//
				case alTop :
					y0 += ((UIPanel*)Parent)->Elements[i]->Height;
					mTop += ((UIPanel*)Parent)->Elements[i]->Height;
					break;
				//
				case alBottom :
					mHeight += ((UIPanel*)Parent)->Elements[i]->Height;
					break;
				//
				case alLeft :
					x0 += ((UIPanel*)Parent)->Elements[i]->Width;
					mLeft += ((UIPanel*)Parent)->Elements[i]->Width;
					break;
				//
				case alRight :
					mWidth += ((UIPanel*)Parent)->Elements[i]->Width;
					break;
			}
			//
		}
		//
		switch (Align) {
			case alCenter :
				Width  = ParentWidth-mWidth-x0-Parent->ScrollWidth;
				Height = ParentHeight-mHeight-y0-Parent->ScrollHeight;
				*x = Parent->Left+x0; *y += y0;
				//
				tmpPos[0] = x0;
				tmpPos[1] = y0;
				//
				break;

			case alTop :
				Width  = ParentWidth-mWidth-x0-Parent->ScrollWidth;
				*x = Parent->Left+x0; *y += y0;
				tmpPos[0] = x0;
				tmpPos[1] = y0;
				//
				break;

			case alBottom :
				Width  = ParentWidth-mWidth-x0-Parent->ScrollWidth;
				*x = Parent->Left+x0; *y += y0+ParentHeight-Height-mTop-mHeight;
				//x = x0; y = ParentHeight-mHeight-Height;
				//x = 0; y = 0;
				tmpPos[0] = x0;
				tmpPos[1] = y0;
				//
				break;

			case alLeft :
				Height = ParentHeight-mHeight-y0-Parent->ScrollHeight;
				*x = Parent->Left+x0; *y += y0;
				//
				tmpPos[0] = x0;
				tmpPos[1] = y0;
				//
				break;

			case alRight :
				Height = ParentHeight-mHeight-y0;
				*x = Parent->Left+ParentWidth-mWidth-Width; *y += y0;
				//
				break;
		}
	} else {
		//
		for (int i = 0; i < Id; i++) {
			//
			if (Manager->Elements[i]->Align == alNone)
				continue;
			//
			switch (Manager->Elements[i]->Align) {
				//
				case alTop :
					y0 += Manager->Elements[i]->Height;
					break;
				//
				case alBottom :
					mHeight += Manager->Elements[i]->Height;
					break;
				//
				case alLeft :
					x0 += Manager->Elements[i]->Width;
					break;
				//
				case alRight :
					mWidth += Manager->Elements[i]->Width;
					break;
			}
			//
		}
		//
		switch (Align) {
			case alCenter :
				Width  = ParentWidth-mWidth-x0;
				Height = ParentHeight-mHeight-y0;
				*x = x0; *y = y0;
				//
				break;

			case alTop :
				Width  = ParentWidth-mWidth-x0;
				*x = x0; *y = y0;
				//
				break;

			case alBottom :
				Width  = ParentWidth-mWidth-x0;
				*x = x0; *y = ParentHeight-mHeight-Height;
				//
				break;

			case alLeft :
				Height = ParentHeight-mHeight-y0;
				*x = x0; *y = y0;
				//
				break;

			case alRight :
				Height = ParentHeight-mHeight-y0;
				*x = ParentWidth-mWidth-Width; *y = y0;
				//
				break;
		}
	}
}
//
void UIPanel::Draw (int x, int y) {
	//
	if (AllowResize && isOpened) {
		if (Align == alTop || Align == alBottom) {
			//
			if (PointInRect(mx, my, splitx, splity-4, splitw, 6))
				Cursor = curVDoubleArrow;

			if (SplitClick) {
				SetSysCursor(curVDoubleArrow, true);
				//
				if (Align == alTop)    Height = CurSize+my-cy;
				if (Align == alBottom) Height = CurSize+cy-my;
			}
			//
			clamp (&Height, MinSize, MaxSize);
		}

		//
		if (Align == alLeft || Align == alRight) {
			//
			if (PointInRect(mx, my, splitx-4, splity, 6, splitw))
				Cursor = curHDoubleArrow;
			//
			if (SplitClick) {
				SetSysCursor(curHDoubleArrow, true);
				//
				if (Align == alLeft)  Width = CurSize+mx-cx;
				if (Align == alRight) Width = CurSize+cx-mx;
			}
			//
			clamp (&Width, MinSize, MaxSize);
		}
	}
	//
	UpdateAlign(&x, &y);
	//
	Left = x; Top = y;
	AbsoluteLeft = x; AbsoluteTop = y;

	AbsolutePos[0] = x;
	AbsolutePos[1] = y;
	//
	int swv = iHeights[4];
	int swh = iHeights[4];
	int ScrollOffset = 0;
	//
	int scx, scy, scw, sch;
	//
	if (DrawQuad) {
		Shader->UnApplyShader();
		ColorShader->ApplyShader();

		if (UseInColor) glUniform4fv (ColorShader->Locations->GetData("Color"), 1, &ColorsIn[0]);
		else if (UseActionColor) glUniform4fv (ColorShader->Locations->GetData("Color"), 1, &ActionColor[0]);
		else glUniform4fv (ColorShader->Locations->GetData("Color"), 1, &Colors[0]);

		QuadElement->SetPosition (glm::vec2(Left, WindowHeight-Height-Top));
		QuadElement->SetSize (glm::vec2(Width, Height));
		QuadElement->BaseRender();

		ColorShader->UnApplyShader();
	}

	// Draw Text
	if (CanHide) {
		ElemsOffset  = 24;
		ScrollOffset = 24;
		//
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
		tx = Left;
		glBegin2D();
		//
		if (TextShadow) {
			glColor3f(ShadowColor[0], ShadowColor[1], ShadowColor[2]);
			glRasterPos2f(tx+ShadowOffset[0], WindowHeight-Top-18+ShadowOffset[1]);
			ftglRenderFont(Theme->Font, Text.c_str(), RENDER_ALL);
		}
		//
		
		glActiveTexture(GL_TEXTURE0);
		glColor3f(TextColor[0], TextColor[1], TextColor[2]);
		glTranslatef(tx+TextOffset[0]+20, WindowHeight-Top-18+TextOffset[1],0);
		ftglRenderFont(Theme->Font, Text.c_str(), RENDER_ALL);
		glEnd2D();
		//
		Shader->ApplyShader();
		//
		if (!DialogOpened || inDialog)
		if (mShift == mouseLeft && !Clicked) {
			//
			if (PointInRect(mx, my, Left, Top+8, Width, 14)) {
				isOpened = !isOpened;

				if (!isOpened) {
					PanelSize = Height;
					Height = 28;
					//
					for (int i = 0; i < Elements.size(); i++) {
						Elements[i]->Focused = false;
						Elements[i]->Enter = false;
						Manager->FocusedElement = 0;
					}
					//
				} else Height = PanelSize;
			}
			//
			Clicked = true;
		}
		//
		int n = 0;
		if (!isOpened) n = 1;
		//
		// Arrow
		glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(Arrow->MVPMatrix[0][0]));
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [14+n], Heights [14+n]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[14+n], OffsetsY[14+n]);
		//
		Arrow->SetPosition (glm::vec2(Left+6, WindowHeight-18-Top));
		Arrow->SetSize (glm::vec2(iWidths[14], iHeights[14]));
		Arrow->BaseRender();
		//
	} else Shader->ApplyShader();
	//
	MaxX = Width;
	MaxY = Height;
	//
	for (int i = 0; i < Elements.size(); i++) {
		//
		int ex = Elements[i]->Left;
		int ey = Elements[i]->Top;
		//
		// FIXME! --->
		if (dynamic_cast<UIPanel*>(Elements[i])) {
			ex = Elements[i]->tmpPos[0];
			ey = Elements[i]->tmpPos[1];
		}
		/*if (dynamic_cast <UIPanel*> (Elements[i])) {
			Elements[i]->Visible = false;
			Elements[i]->Draw(x+ex, y+ey);
			Elements[i]->Visible = true;
			//
			//ex = Elements[i]->Left;
			ey = Elements[i]->Top-Top;
		}*/
		//
		//printf("%d\n", Elements[i]->Width);

		if (Elements[i]->Align != alTop && Elements[i]->Align != alBottom && Elements[i]->Align != alCenter)
			MaxX = max (MaxX, ex+Elements[i]->Width+5);
		//
		if (Elements[i]->Align != alLeft && Elements[i]->Align != alRight)
			MaxY = max (MaxY, ey+Elements[i]->Height+5+ElemsOffset);
		//
		/*Elements[i]->Left = ex;
		Elements[i]->Top  = ey;
		// <----------
		//
		if (Enter && PointInRect(mx, my, x+ex-hsOffset, y+ey-vsOffset, Elements[i]->Width-1, Elements[0]->Height) && Elements[i]->Enabled) {
			Elements[i]->Enter = true;
			Cursor = Elements[i]->Cursor;
			
			if (mShift == mouseLeft) {
				Elements[i]->Click = true;
			} else Elements[i]->Click = false;
		} else {
			Elements[i]->Enter = false;
			Elements[i]->Click = false;
		}*/
	}
	//
	//exit(0);
	//printf("%d\n", MaxY);
	//exit(0);
	if (!isOpened)
		goto DrawSplit;
	//
	ScrollWidth  = 0;
	ScrollHeight = 0;
	//
	if (ShowScroll) {
		if (MaxY != Height) { ScrollWidth  = iHeights[4]; swh = 0; }
		if (MaxX != Width)  { ScrollHeight = iHeights[4]; swv = 0; }
	}
	//
	hbMin = iWidths[6]+iWidths[8];
	hbMax = Width-iHeights[4]+swh;
	//
	vbMin = iWidths[6]+iWidths[8]+ScrollOffset;
	vbMax = Height-iHeights[4]+swv;
	//ShowScroll = false;
	// Draw Scroll

	// Horizontal
	if (MaxX != Width && ShowScroll) {
		// Left
		glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(ScrollBg[0]->MVPMatrix[0][0]));
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [0], Heights [0]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[0], OffsetsY[0]);
		//
		ScrollBg[0]->SetPosition (glm::vec2(Left, WindowHeight-Height-Top));
		ScrollBg[0]->SetSize (glm::vec2(iWidths[0], iHeights[0]));
		ScrollBg[0]->BaseRender();

		// Center
		//
		glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(ScrollBg[1]->MVPMatrix[0][0]));
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [1], Heights [1]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[1], OffsetsY[1]);
		//
		ScrollBg[1]->SetPosition (glm::vec2(Left+iWidths[0], WindowHeight-Height-Top));
		ScrollBg[1]->SetSize (glm::vec2(Width-iWidths[0]-iWidths[2]-iHeights[4]+swh, iHeights[1]));
		ScrollBg[1]->BaseRender();

		// Right
		//
		glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(ScrollBg[2]->MVPMatrix[0][0]));
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [2], Heights [2]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[2], OffsetsY[2]);
		//
		ScrollBg[2]->SetPosition (glm::vec2(Left+Width-iWidths[0]-iHeights[4]+swh, WindowHeight-Height-Top));
		ScrollBg[2]->SetSize (glm::vec2(iWidths[2], iHeights[2]));
		ScrollBg[2]->BaseRender();
	}

	// Vertical
	
	if (MaxY != Height && ShowScroll) {
		// Bottom
		glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(ScrollBg[3]->MVPMatrix[0][0]));
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [3], Heights [3]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[3], OffsetsY[3]);
		//
		ScrollBg[3]->SetPosition (glm::vec2(Left+Width, WindowHeight-Height+iHeights[2]-Top-swv));
		ScrollBg[3]->SetSize (glm::vec2(iWidths[3], iHeights[3]));
		ScrollBg[3]->BaseRender();

		// Center
		glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(ScrollBg[4]->MVPMatrix[0][0]));
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [4], Heights [4]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[4], OffsetsY[4]);
		//
		ScrollBg[4]->SetPosition (glm::vec2(Left+Width, WindowHeight-Height+iWidths[5]+iHeights[2]-Top-swv));
		ScrollBg[4]->SetSize (glm::vec2(Height-iWidths[5]-iWidths[3]-iHeights[2]+swv-ScrollOffset, iHeights[4]));
		ScrollBg[4]->BaseRender();

		// Top
		glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(ScrollBg[5]->MVPMatrix[0][0]));
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [5], Heights [5]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[5], OffsetsY[5]);
		//
		ScrollBg[5]->SetPosition (glm::vec2(Left+Width, WindowHeight-iWidths[5]-Top-ScrollOffset));
		ScrollBg[5]->SetSize (glm::vec2(iWidths[5], iHeights[5]));
		ScrollBg[5]->BaseRender();
	}

	// Draw Buttons
	// Horizontal
	if (MaxX != Width && ShowScroll) {
		//
		int n = 0;
		if (!DialogOpened || inDialog) {
			int x = (hbMax*100)/MaxX;
			//
			hbSize = (hbMax*x)/100;
			clamp(&hbSize, hbMin, hbMax);
			//
			//if (PointInRect(mx, my, hbOffset+Left, Top+Height-iHeights[6], hbSize, iHeights[6])) {
			//	//
			//	n = 3;
			//}

			bool inRect = PointInRect(mx, my, hbOffset+Left, Top+Height-iHeights[6], hbSize, iHeights[6]);

			if (inRect) {
				n = 3;
				//
			}

			if (mShift == mouseLeft && !Clicked) {
				if (inRect)
					SHClick = true;
			}

			if (SHClick && !SplitClick) {
			//if (n == 3 && mShift == mouseLeft && !SplitClick) {
				hbOffset = lhbOffset+mx-cx;
				clamp(&hbOffset, 0, hbMax-hbSize);
				//
				int py = (hbOffset*100)/hbMax;
				hsOffset = (MaxX*py)/100;
			}
		}
		//
		// Left
		glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(ScrollBtn[0]->MVPMatrix[0][0]));
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [6+n], Heights [6+n]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[6+n], OffsetsY[6+n]);
		//
		ScrollBtn[0]->SetPosition (glm::vec2(hbOffset+Left, WindowHeight-Height-Top));
		ScrollBtn[0]->SetSize (glm::vec2(iWidths[6], iHeights[6]));
		ScrollBtn[0]->BaseRender();

		// Center
		//
		glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(ScrollBtn[1]->MVPMatrix[0][0]));
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [7+n], Heights [7+n]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[7+n], OffsetsY[7+n]);
		//
		ScrollBtn[1]->SetPosition (glm::vec2(hbOffset+Left+iWidths[7], WindowHeight-Height-Top));
		ScrollBtn[1]->SetSize (glm::vec2(hbSize-iWidths[6]-iWidths[8], iHeights[6]));
		ScrollBtn[1]->BaseRender();

		// Right
		//
		glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(ScrollBtn[2]->MVPMatrix[0][0]));
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [8+n], Heights [8+n]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[8+n], OffsetsY[8+n]);
		//
		ScrollBtn[2]->SetPosition (glm::vec2(hbOffset+Left+hbSize-iWidths[6], WindowHeight-Height-Top));
		ScrollBtn[2]->SetSize (glm::vec2(iWidths[8], iHeights[8]));
		ScrollBtn[2]->BaseRender();
	}
	//
	// Vertical
	if (MaxY != Height && ShowScroll) {
		//
		int n = 0;
		if (!DialogOpened || inDialog) {			
			int xx = (vbMax*100)/MaxY;
			//
			vbSize = (vbMax*xx)/100;
			clamp(&vbSize, vbMin, vbMax);
			bool inRect = PointInRect(mx, my, Left+Width-iHeights[6], Top+vbOffset+ScrollOffset, iHeights[6], vbSize);

			if (inRect) {
				n = 3;
				//
			}

			if(mShift == mouseLeft && !Clicked) {
				if (inRect)
					VHClick = true;
			}

			if (VHClick && !SplitClick) {
				n = 3;
				vbOffset = lvbOffset+my-cy;
				clamp(&vbOffset, 0, vbMax-vbSize);
				//
				int py = (vbOffset*100)/vbMax;
				vsOffset = (MaxY*py)/100;
			}
		}
		//
		// Bottom
		glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(ScrollBtn[3]->MVPMatrix[0][0]));
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [6+n], Heights [6+n]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[6+n], OffsetsY[6+n]);
		//
		ScrollBtn[3]->SetPosition (glm::vec2(Left+Width, WindowHeight-(Top+vbOffset+iWidths[6]-iWidths[8]+vbSize)));
		ScrollBtn[3]->SetSize (glm::vec2(iWidths[6], iHeights[6]));
		ScrollBtn[3]->BaseRender();

		// Center
		glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(ScrollBtn[4]->MVPMatrix[0][0]));
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [7+n], Heights [7+n]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[7+n], OffsetsY[7+n]);
		//
		ScrollBtn[4]->SetPosition (glm::vec2(Left+Width, WindowHeight-(Top+vbOffset+vbSize-iWidths[8])));
		ScrollBtn[4]->SetSize (glm::vec2(vbSize-iWidths[8]-iWidths[6]-ScrollOffset, iHeights[7]));
		ScrollBtn[4]->BaseRender();

		// Top
		glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(ScrollBtn[5]->MVPMatrix[0][0]));
		glUniform2f (Shader->Locations->GetData("Size")  , Widths  [8+n], Heights [8+n]);
		glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[8+n], OffsetsY[8+n]);
		//
		ScrollBtn[5]->SetPosition (glm::vec2(Left+Width, WindowHeight-iWidths[8]-Top-vbOffset-ScrollOffset));
		ScrollBtn[5]->SetSize (glm::vec2(iWidths[8], iHeights[8]));
		ScrollBtn[5]->BaseRender();
	}

	//
	scx = Left;				 scy = WindowHeight-Height-Top+ScrollHeight;
	scw = Width-ScrollWidth; sch = Height-ElemsOffset;

	Manager->PushScissor(scx, scy, scw, sch);
	Cursor = curNormal;
	//
	//
	ScrollX = hsOffset;
	ScrollY = vsOffset;
		//bool end = false;

	//for (int i = Elements.size()-1; i >= 0; i--) {
	for (int i = 0; i < Elements.size(); i++) {
		//
		if (!Elements[i]->Visible)
			continue;

		// FIXME! --->
		int ex = Elements[i]->Left;
		int ey = Elements[i]->Top;
		//
		Elements[i]->Draw(x+ex-hsOffset, y+ey-vsOffset+ElemsOffset);

		//
		Elements[i]->Left = ex;
		Elements[i]->Top  = ey;
		// <----------
		//
		if (FreezUI) continue;
		if (DialogOpened && !inDialog)
			continue;

		if (/*PointInRect(mx, my, Left, Top, Width-swh, Height-shh) && */
			PointInRect(mx, my, x+ex-hsOffset, y+ey-vsOffset+ElemsOffset, Elements[i]->Width-1, Elements[i]->Height) &&
			Elements[i]->Enabled)
		{
			//
			Elements[i]->Enter = true;
			Cursor = Elements[i]->Cursor;
			
			if (mShift == mouseLeft) {
				Elements[i]->Click = true;
			} else Elements[i]->Click = false;
			//
			//end = true;
			//break;
		} else {
			Elements[i]->Enter = false;
			Elements[i]->Click = false;
		}
		//
		//if (end) break;
	}
	//
	Manager->PopScissor();

	if (mShift == mouseLeft && ! Clicked)
		Clicked = true;
	//

DrawSplit:
	//
	//mShift = mouseNone;
	// Draw Split
	//
	int n = 12;
	//
	if (Parent != 0 || !AllowResize) n = 13;
	//
	switch (Align) {
		case alTop :
			splitx = Left;  splity = Top+Height-2;
			splitw = Width; splith = iHeights[n];
			Split->SetRotate(pi);
			//
			break;

		case alBottom :
			splitx = Left;  splity = Top+2;
			splitw = Width; splith = iHeights[n];
			//
			break;
		//
		case alLeft :
			splitx = Left+Width-2;  splity = Top;
			splitw = Height; splith = iHeights[n];
			Split->SetRotate(-pi/2);
			//
			break;

		//
		case alRight :
			splitx = Left+2; splity = Top;
			splitw = Height; splith = iHeights[n];
			Split->SetRotate (pi/2);
			//
			break;
			//
		default : return;
	};
	//
	glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(Split->MVPMatrix[0][0]));
	glUniform2f (Shader->Locations->GetData("Size")  , Widths  [n], Heights [n]);
	glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[n], OffsetsY[n]);
	//
	//if (!AllowResize) return;
	//
	int sy = splity;
	int sx = splitx;
	//
	if (Align == alTop  ) sx += Width;
	if (Align == alRight) sy += Height;
	//
	Split->SetPosition (glm::vec2(sx, WindowHeight-sy));
	Split->SetSize (glm::vec2(splitw, splith));
	Split->BaseRender();
	//
	if (DialogOpened && !inDialog) return;
	
	//if (mShift == mouseLeft) Clicked = true;
}

//Events
void UIPanel::DblClick  (int x, int y, int Shift) {
	//
	if (!isOpened)
		return;
	//
	for (int i = 0; i < Elements.size(); i++) {
		if (!Elements[i]->Visible)
			continue;
		
		if (PointInRect (x, y, AbsolutePos[0], AbsolutePos[1], Width, Height))
			Elements[i]->DblClick(x, y, Shift);
	}
}

//
void UIPanel::MouseDown (int x, int y, int Shift) {
	mShift = Shift;
	cx = x; cy = y;
	lhbOffset = hbOffset;
	lvbOffset = vbOffset;
	//mx = x; my = y;
	//Clicked = true;
	
	//
	if (mShift == mouseLeft) {
		if (PointInRect(mx, my, hbOffset+Left, Top+Height-iHeights[6], hbSize, iHeights[6]))
			SHClick = true;

		if (PointInRect(mx, my, Left+Width-iHeights[6], Top+vbOffset, iHeights[6], vbSize))
			VHClick = true;

		if (!AllowResize) goto Event;
		if (Align == alTop || Align == alBottom) {
			//
			if (PointInRect(mx, my, splitx, splity-4, splitw, 6)) {
				CurSize = Height;
				SplitClick = true;
			}
		}

		//
		if (Align == alLeft || Align == alRight) {
			//
			if (PointInRect(mx, my, splitx-4, splity, 6, splitw)) {
				SplitClick = true;
				CurSize = Width;
			}
		}
	}
//
Event:
	//
	//
	//if (!DialogOpened || inDialog)
	for (int i = Elements.size()-1; i >= 0; i--) {
		//
		if (!Elements[i]->Visible)
			continue;
		
		Elements[i]->mShift = mouseNone;
		//if (Enter)
		if (PointInRect (x, y, AbsolutePos[0], AbsolutePos[1], Width, Height))
			Elements[i]->MouseDown(x, y, Shift);
	}
}

void UIPanel::MouseMove (int x, int y, int Shift) {
	//
	mx = x; my = y;
	//
	if (!isOpened)
		return;
	//
	for (int i = 0; i < Elements.size(); i++) {
		//
		if (!Elements[i]->Visible)
			continue;
		//
		Elements[i]->MouseMove(x, y, Shift);
	}
}

void UIPanel::MouseUp (int x, int y, int Shift) {
	//
	mShift = mouseNone;
	SHClick    = false;
	VHClick    = false;
	SplitClick = false;
	LockCursor = false;
	Clicked = false;
	//
	if (!isOpened)
		return;
	//
	for (int i = 0; i < Elements.size(); i++) {
		if (!Elements[i]->Visible)
			continue;
		//
		//if (dynamic_cast<UIColorPanel*>(Elements[i]) && x != -1 && Elements[i]->OnClick == 0) exit(0);
		if (dynamic_cast<UIImage*>(Elements[i]) /*|| dynamic_cast<UIColorPanel*>(Elements[i])*/) Elements[i]->MouseUp(x-Left, y-Top, Shift);
		else Elements[i]->MouseUp(x, y, Shift);
	}
}
//
void UIPanel::KeyDown (int key) {
	//
	if (!isOpened)
		return;
	//
	for (int i = 0; i < Elements.size(); i++) {
		//
		if (!Elements[i]->Visible)
			continue;

		Elements[i]->KeyDown(key);
	}
}

void UIPanel::Resized (int Width, int Height) {
	//
	if (!isOpened)
		return;
}

void UIPanel::KeyPressed (Uint16 key) {
	//
	if (!isOpened)
		return;
	//
	for (int i = 0; i < Elements.size(); i++)  {
		//
		if (!Elements[i]->Visible)
			continue;

		Elements[i]->KeyPressed(key);
	}
}

void UIPanel::TextEntered (Uint16 key) {
	//
	if (!isOpened)
		return;
	//
	for (int i = 0; i < Elements.size(); i++) {
		//
		if (!Elements[i]->Visible)
			continue;

		Elements[i]->TextEntered(key);
	}
}

void UIPanel::Progress() {
	if (TabJoin != 0) {
		if (Join != 0 && (!Join->isOpened || !Join->Visible))
			Visible = false;
		else Visible = TabJoin->Checked;
	} else if (Join != 0)
		Visible = Join->Visible && Join->isOpened;
	//
	for (int i = 0; i < Elements.size(); i++)
		Elements[i]->Progress();
	//
}

//
// UIDialog
void UIDialog::MouseUp (int x, int y, int Shift) {
	UIPanel::MouseUp (x, y, Shift);
	//
	Clicked = false;
	mShift = mouseNone;
}

void UIDialog::Progress () {
	//
	for (int i = 0; i < Elements.size(); i++)
		Elements[i]->Progress();
}

void UIDialog::Draw (int x, int y) {
	//
	if (!isOpened)
		return;

	// TOP --->
	// Left
	FormElements[0]->UpdateModelMatrix();
	FormElements[0]->SetPosition (glm::vec2(Left-iWidths[0], WindowHeight-Top));
	FormElements[0]->SetSize (glm::vec2(iWidths[0], iHeights[0]));

	glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(FormElements[0]->MVPMatrix[0][0]));
	glUniform2f (Shader->Locations->GetData("Size")  , Widths  [0], Heights [0]);
	glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[0], OffsetsY[0]);
	//
	FormElements[0]->BaseRender();

	// Center
	FormElements[1]->UpdateModelMatrix();
	FormElements[1]->SetPosition (glm::vec2(Left, WindowHeight-Top));
	FormElements[1]->SetSize (glm::vec2(Width, iHeights[1]));

	glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(FormElements[1]->MVPMatrix[0][0]));
	glUniform2f (Shader->Locations->GetData("Size")  , Widths  [1], Heights [1]);
	glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[1], OffsetsY[1]);
	//
	FormElements[1]->BaseRender();

	// Right
	FormElements[2]->UpdateModelMatrix();
	FormElements[2]->SetPosition (glm::vec2(Left+Width, WindowHeight-Top));
	FormElements[2]->SetSize (glm::vec2(iWidths[2], iHeights[2]));

	glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(FormElements[2]->MVPMatrix[0][0]));
	glUniform2f (Shader->Locations->GetData("Size")  , Widths  [2], Heights [2]);
	glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[2], OffsetsY[2]);
	//
	FormElements[2]->BaseRender();
	//
	// < ---

	// Center --->
	// Left

	FormElements[3]->UpdateModelMatrix();
	FormElements[3]->SetPosition (glm::vec2(Left-iWidths[3], WindowHeight-Top-Height));
	FormElements[3]->SetSize (glm::vec2(iWidths[3], Height));

	glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(FormElements[3]->MVPMatrix[0][0]));
	glUniform2f (Shader->Locations->GetData("Size")  , Widths  [3], Heights [3]);
	glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[3], OffsetsY[3]);
	//
	FormElements[3]->BaseRender();

	// Center
	FormElements[4]->UpdateModelMatrix();
	FormElements[4]->SetPosition (glm::vec2(Left, WindowHeight-Top-Height));
	FormElements[4]->SetSize (glm::vec2(Width, Height));

	glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(FormElements[4]->MVPMatrix[0][0]));
	glUniform2f (Shader->Locations->GetData("Size")  , Widths  [4], Heights [4]);
	glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[4], OffsetsY[4]);
	//
	FormElements[4]->BaseRender();

	// Right
	FormElements[5]->UpdateModelMatrix();
	FormElements[5]->SetPosition (glm::vec2(Left+Width, WindowHeight-Top-Height));
	FormElements[5]->SetSize (glm::vec2(iWidths[5], Height));

	glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(FormElements[5]->MVPMatrix[0][0]));
	glUniform2f (Shader->Locations->GetData("Size")  , Widths  [5], Heights [5]);
	glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[5], OffsetsY[5]);
	//
	FormElements[5]->BaseRender();
	//
	// < ---

	// Bottom --->
	// Left

	FormElements[6]->UpdateModelMatrix();
	FormElements[6]->SetPosition (glm::vec2(Left-iWidths[6], WindowHeight-Top-Height-iHeights[6]));
	FormElements[6]->SetSize (glm::vec2(iWidths[6], iHeights[6]));

	glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(FormElements[6]->MVPMatrix[0][0]));
	glUniform2f (Shader->Locations->GetData("Size")  , Widths  [6], Heights [6]);
	glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[6], OffsetsY[6]);
	//
	FormElements[6]->BaseRender();

	// Center
	FormElements[7]->UpdateModelMatrix();
	FormElements[7]->SetPosition (glm::vec2(Left, WindowHeight-Top-Height-iHeights[7]));
	FormElements[7]->SetSize (glm::vec2(Width, iHeights[7]));

	glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(FormElements[7]->MVPMatrix[0][0]));
	glUniform2f (Shader->Locations->GetData("Size")  , Widths  [7], Heights [7]);
	glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[7], OffsetsY[7]);
	//
	FormElements[7]->BaseRender();

	// Right
	FormElements[8]->UpdateModelMatrix();
	FormElements[8]->SetPosition (glm::vec2(Left+Width, WindowHeight-Top-Height-iHeights[8]));
	FormElements[8]->SetSize (glm::vec2(iWidths[8], iHeights[8]));

	glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(FormElements[8]->MVPMatrix[0][0]));
	glUniform2f (Shader->Locations->GetData("Size")  , Widths  [8], Heights [8]);
	glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[8], OffsetsY[8]);
	//
	FormElements[8]->BaseRender();
	//
	// < ---

	// Caption

	//Caption = L"Hello World~!";
	string Text = wstring2string (Caption);
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
	//tx = Left;
	
	Shader->UnApplyShader();
	glBegin2D();
	//
	/*if (TextShadow) {
		glColor3f(ShadowColor[0], ShadowColor[1], ShadowColor[2]);
		glRasterPos2f(tx+ShadowOffset[0], WindowHeight-Top-iHeights[1]+ShadowOffset[1]);
		ftglRenderFont(Theme->Font, Text.c_str(), RENDER_ALL);
	}*/
	//
	
	//glColor3f(TextColor[0], TextColor[1], TextColor[2]);
	glColor3f(1.0f, 1.0f, 1.0f);
	//glRasterPos2f(tx+TextOffset[0]+20, Top-iHeights[1]+TextOffset[1]);
	//glRasterPos2f(100.0f, 100.0f);
	glActiveTexture(GL_TEXTURE0);
	glTranslatef   (tx, WindowHeight-y+5, 0.f);
	ftglRenderFont (Theme->Font, Text.c_str(), RENDER_ALL);
	glEnd2D();

	Shader->ApplyShader();
	UIPanel::Draw (x, y);
	//
	if (Clicked) {
		//
		Left = tLeft+mx-cx;
		Top  = tTop +my-cy;
		//
		clamp (&Left, iWidths[0] , WindowWidth -iWidths [2]-Width);
		clamp (&Top , iHeights[1], WindowHeight-iHeights[7]-Height);
	}
}

void UIDialog::MouseDown (int x, int y, int Shift) {
	UIPanel::MouseDown (x, y, Shift);
	//
	if (PointInRect (x, y, Left, Top-iHeights[1], Width, iHeights[1]) && mShift == mouseLeft) {
		tLeft = Left; tTop = Top;
		Clicked = true;
	}
}

#endif
