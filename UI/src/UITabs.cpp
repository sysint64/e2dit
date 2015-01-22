#ifndef CPP_UITABS
#define CPP_UITABS

//
void UITabs::Draw (int x, int y) {
	//
	int x0 = 0; int mWidth  = 0;
	int y0 = 0; int mHeight = 0;
	Shader->ApplyShader();
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

	int taby = y0+2-23;
	int tabx = TabsOffset;
	//
	for (int i = Tabs.size()-1; i >= 0; i--)
		tabx += Tabs[i]->Width-TabOffset;

	int n = 0;
	for (int i = Tabs.size()-1; i >= 0; i--) {
		tabx -= Tabs[i]->Width-TabOffset;

		if (!Tabs[i]->Active) {
			if (!DialogOpened || inDialog)
			if (PointInRect(mx, my, tabx+(TabOffset >> 1), taby, Tabs[i]->Width-TabOffset-1, Tabs[i]->Height) && Tabs[i]->Enabled) {
				Tabs[i]->Enter = true;

				if (PointInRect(cx, cy, tabx+(TabOffset >> 1), taby, Tabs[i]->Width-TabOffset-1, Tabs[i]->Height))
				if (mShift == mouseLeft && (!Clicked || Tabs[i]->Click)) {
					Clicked = true;
					Tabs[i]->Active = true;

					if (ActiveTab != 0) {
						ActiveTab->Active = false;
						ActiveTab->Click  = false;
						ActiveTab->Enter  = false;
					}
				}

			} else Tabs[i]->Enter = false;

			Tabs[i]->Draw(tabx, taby);
		}
	}

	Panel->SetPosition (glm::vec2 (0, WindowHeight-iHeights[0]-y0+2));
	Panel->SetSize (glm::vec2 (WindowWidth, iHeights[0]));

	glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(Panel->MVPMatrix[0][0]));
	glUniform2f (Shader->Locations->GetData("Size")  , Widths  [0], Heights [0]);
	glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[0], OffsetsY[0]);
	//
	Panel->BaseRender();

	taby = y0+2-23;
	tabx = x0+TabsOffset;
	//
	for (int i = 0; i < Tabs.size(); i++) {
		if (Tabs[i]->Active) {
			Tabs[i]->Click = true;
			Tabs[i]->Draw(tabx, taby);
			ActiveTab = Tabs[i];

			DrawTabElements(i, x0, y0);

			break;
		}

		tabx += Tabs[i]->Width-TabOffset;
	}
	Shader->UnApplyShader();
}

void UITabs::DrawFront (int x, int y) {

}

void UITabs::DrawTabElements (int id, int x, int y) {

	int taby = y+2;
	int tabx = x;

	for (int i = 0; i < Tabs[id]->Elements.size(); i++) {
		UIElement *Elem = Tabs[id]->Elements[i];

		if (dynamic_cast<UITabElement*>(Elem)) {
			UITabElement *Elem = (UITabElement*)Tabs[id]->Elements[i];

			if (!DialogOpened || inDialog)
			if (PointInRect(mx, my, tabx, taby, Elem->Width, Elem->Height+25) && Elem->Enabled) {
				Elem->Enter = true;

				//if (Elem->CanCheck)
				if (mShift == mouseLeft && (!Clicked || Elem->Click)) {
					Clicked = true;

					for (int k = 0; k < Tabs[id]->Elements.size(); k++) {
						if (!dynamic_cast<UITabElement*>(Tabs[id]->Elements[k]))
							continue;

						((UITabElement*)Tabs[id]->Elements[k])->Checked = false;
						Tabs[id]->Elements[k]->Enter = false;
					}

					Elem->Checked = true;
					//Elem->MouseUp (mx, my, mShift);
					if (Elem->OnClick != 0)
						Elem->OnClick(Elem);
				}
			} else Elem->Enter = false;

			Elem->Draw(tabx, taby);
			tabx += Elem->Width+10;
		}

		if (dynamic_cast<UITabSplit*>(Elem)) {
			tabx += Elem->Width+10;

			Split->SetPosition (glm::vec2 (tabx, WindowHeight-iHeights[1]-taby));
			Split->SetSize (glm::vec2 (iWidths[1], iHeights[1]));

			glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(Split->MVPMatrix[0][0]));
			glUniform2f (Shader->Locations->GetData("Size")  , Widths  [1], Heights [1]);
			glUniform2f (Shader->Locations->GetData("Offset"), OffsetsX[1], OffsetsY[1]);
			//
			Split->BaseRender();

			tabx += iWidths[1]+10;
		}
	}
}

void UITabs::DblClick (int x, int y, int Shift) {
	
}

void UITabs::MouseDown (int x, int y, int Shift) {
	mx = x; my = y; mShift = Shift;
	cx = x; cy = y;
}

void UITabs::MouseMove (int x, int y, int Shift) {
	mx = x; my = y;
}

void UITabs::MouseUp (int x, int y, int Shift) {
	Clicked = false; mShift = mouseNone;
}

//
void UITabs::KeyPressed (Uint16 key) {
	
}

void UITabs::TextEntered (Uint16 key) {
	
}

// Tab Element -------------->

void UITabElement::Init() {
	//if (Inited) return;
	//Inited = true;

	QuadElement = new BaseObject();
	QuadElement->Init();
	//
	QuadElement->Data    = GUIElementDat;
	QuadElement->mCamera = ScreenCamera;
	QuadElement->Shader  = Shader;
	//
	Width  = ToolIcons.SizeIcon;
	Height = ToolIcons.SizeIcon;
}

void UITabElement::Draw (int x, int y) {
	//
	Click = Checked;
	Left = x; Top = y;
	int io[2];

	if (!Enter && !Checked) glUniform1f (Shader->Locations->GetData("Alpha"), 0.65f);

	if (ShowClickIcon && Click) {
		io[0] = ClickIconOffset[0];
		io[1] = ClickIconOffset[1];
	} else {
		io[0] = IconOffset[0];
		io[1] = IconOffset[1];
	}
	//
	if (io[0] >= 0 && io[1] >= 0) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, Textures::TopToolIcons->Handle);
		glUniform1i (Shader->Locations->GetData("Texture"), 2);
		//
		glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(QuadElement->MVPMatrix[0][0]));
		glUniform2f (Shader->Locations->GetData("Size")  , ToolIcons.Width, ToolIcons.Height);
		glUniform2f (Shader->Locations->GetData("Offset"), ToolIcons.OffsetsX[io[0]][io[1]], ToolIcons.OffsetsY[io[0]][io[1]]);

		QuadElement->SetPosition (glm::vec2(Left+5, WindowHeight-Top-Height-5));
		QuadElement->SetSize (glm::vec2(ToolIcons.SizeIcon, ToolIcons.SizeIcon));
		QuadElement->BaseRender();

		glUniform1i (Shader->Locations->GetData("Texture"), ThemeTexID);
	}

	Shader->UnApplyShader();
	glBegin2D();
	//
	string Text = wstring2string(Caption);
	float bounds[6];
	
	ftglGetFontBBox(Theme->Font, Text.c_str(), Caption.size(), bounds);

	int twidth = bounds[3]-bounds[0];
	int tx = Left+5+((Width-twidth) >> 1);
	//glColor3f(TextColor[n], TextColor[n+1], TextColor[n+2]);
	
	if (!Enter && !Checked) glColor3f(110.0f/255.0f, 110.0f/255.0f, 110.0f/255.0f);
	else glColor3f(0, 0, 0);

	glActiveTexture(GL_TEXTURE0);
	glTranslatef(tx, WindowHeight-Top-Height-18, 0);
	ftglRenderFont(Theme->Font, Text.c_str(), RENDER_ALL);
	glEnd2D();
	Shader->ApplyShader();
	glUniform1f (Shader->Locations->GetData("Alpha"), 1.0f);
}

// <-------------- Tab Element

#endif
