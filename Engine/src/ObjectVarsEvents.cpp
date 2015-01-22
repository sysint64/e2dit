#ifndef CPP_OBJECT_VARS_EVENT
#define CPP_OBJECT_VARS_EVENT
//
inline void ProportionsUI (Texture2D *Texture, UIElement *Element, int wneed, int hneed) {
	//int size = min (wneed, hneed);
	int size;
	
	if (Element->Height >= hneed) size = Element->Width;
	else size = wneed;

	if (Element->Width >= wneed) size = wneed;

	if (Texture->Width > size || Texture->Height > size) {
		float ratio = (float) Texture->Height / (float) Texture->Width;

		if (ratio == 1.0f) {
			Element->Width  = size;
			Element->Height = size;
		} else if (ratio < 1.0f) {
			Element->Width  = size;
			Element->Height = size*ratio;
		} else {
			Element->Height = size;
			Element->Width  = size/ratio;
		}
	} else {
		Element->Width  = Texture->Width;
		Element->Height = Texture->Height;
	}
}


// Main Tab

void BaseGameObject::DisplayMainTab() {
	((UIEdit*) MapEditor->Data->Elements["eclass"])->wText = StrToWStr (GameClass);

	if (dynamic_cast<LightObject*>(this)) {

		LightObject *Light = (LightObject*) this;

		((UIColorPanel*) MapEditor->Data->Elements["clightcolor"])->Color = Light->Color;
		
		MapEditor->Data->Elements["bltdynamic"]  ->Checked = Light->Type == ltDynamic;
		MapEditor->Data->Elements["bltstatic"]   ->Checked = Light->Type == ltStatic;
		MapEditor->Data->Elements["bltstationar"]->Checked = Light->Type == ltStationar;
		MapEditor->Data->Elements["cbcastshadow"]->Checked = Light->CastShadow;

		((UIEdit*) MapEditor->Data->Elements["elightradius"])->wText = FloatToWStr (Light->Radius);
		((UIEdit*) MapEditor->Data->Elements["eattenuation"])->wText = FloatToWStr (Light->Attenuation);

	}

}

void BaseGameObject::DisplayVars() {

	// Clear Panel
	
	UIPanel *VarsTabPanel = ((UIPanel*) MapEditor->Data->Elements["pvarstab"]);
	UIPanel *Parent = 0;

	for (int i = 0; i < VarsTabPanel->Elements.size(); i++)
		delete VarsTabPanel->Elements[i];
	
	VarsTabPanel->Elements.clear();
	//

	for (BaseUniforms::UniformsMap::iterator it = Vars->Uniforms.begin(); it != Vars->Uniforms.end(); it++) {
		Uniform *U = &it->second;

		switch (U->Type) {
			case uFloat	: break;
			case uInt	: break;
			case uBool	: break;

			//case uVec2f : Parent = (UIPanel*) MapEditor->VarsUIData->Elements["vec2f"]; break;
			case uVec2i : break;
			case uVec3f : break;

			case uVec3i : break;
			case uMat4f : Parent = (UIPanel*) MapEditor->VarsUIData->Elements["mat4f"]; break;
			//case uTex1D : break;
			case uTex2D : Parent = (UIPanel*) MapEditor->VarsUIData->Elements["tex2d"]; break;
		} // switch

		if (Parent == 0) continue;

		UIPanel *UniformPanel = new UIPanel();

		UniformPanel->Theme = Parent->Theme;
		UniformPanel->Shader = Parent->Shader;
		UniformPanel->ColorShader = Parent->ColorShader;
		UniformPanel->Focused = false;
		UniformPanel->Manager = Parent->Manager;
		UniformPanel->Align = alTop;
		UniformPanel->CanHide = true;
		UniformPanel->Caption = U->Name;
		UniformPanel->MinSize = Parent->MinSize;
		UniformPanel->MaxSize = Parent->MaxSize;

		UniformPanel->Init();

		UIElement *Element = 0;
		UniformPanel->Elements.clear();
		//
		for (int i = 0; i < Parent->Elements.size(); i++) {
			if (dynamic_cast<UIButton*>(Parent->Elements[i])) {
				Element = new UIButton();
				*((UIButton*) Element) = *((UIButton*) Parent->Elements[i]);
			}

			if (dynamic_cast<UIEdit*>(Parent->Elements[i])) {
				if (Element != 0) delete Element;
				Element = new UIEdit();
				*((UIEdit*) Element) = *((UIEdit*) Parent->Elements[i]);
			}

			if (dynamic_cast<UIColorPanel*>(Parent->Elements[i])) {
				if (Element != 0) delete Element;
				Element = new UIColorPanel();
				*((UIColorPanel*) Element) = *((UIColorPanel*) Parent->Elements[i]);
			}

			if (dynamic_cast<UIImage*>(Parent->Elements[i])) {
				if (Element != 0) delete Element;
				Element = new UIImage();
				*((UIImage*) Element) = *((UIImage*) Parent->Elements[i]);
			}

			if (dynamic_cast<UICheckBox*>(Parent->Elements[i])) {
				if (Element != 0) delete Element;
				Element = new UICheckBox();
				*((UICheckBox*) Element) = *((UICheckBox*) Parent->Elements[i]);
			}

			if (dynamic_cast<UIGroupedElements*>(Parent->Elements[i])) {
				UIGroupedElements *Grouped = new UIGroupedElements();

				Grouped->Theme = Parent->Theme;
				Grouped->Shader = Parent->Shader;
				Grouped->ColorShader = Parent->ColorShader;
				Grouped->Focused = false;
				Grouped->Manager = Parent->Manager;
				Grouped->Align = ((UIGroupedElements*) Parent->Elements[i])->Align;
				Grouped->Left  = ((UIGroupedElements*) Parent->Elements[i])->Left;
				Grouped->Top   = ((UIGroupedElements*) Parent->Elements[i])->Top;

				Grouped->Parent = UniformPanel;
				Grouped->Init();

				Element = 0;

				for (int j = 0; j < ((UIGroupedElements*) Parent->Elements[i])->Elements.size(); j++) {
					UIElement *Element = 0;
					UIElement *SrcElement = ((UIGroupedElements*) Parent->Elements[i])->Elements[j];

					if (dynamic_cast<UIButton*>(SrcElement)) {
						Element = new UIButton();
						*((UIButton*) Element) = *((UIButton*) SrcElement);
					}

					if (dynamic_cast<UIEdit*>(SrcElement)) {
						Element = new UIEdit();
						*((UIEdit*) Element) = *((UIEdit*) SrcElement);
					}

					if (dynamic_cast<UICheckBox*>(SrcElement)) {
						Element = new UICheckBox();
						*((UICheckBox*) Element) = *((UICheckBox*) SrcElement);
					}

					if (Element != 0) {
						if (U->Type == uTex2D) SetTex2DValues (Vars->GetTex2D(it->first.c_str()), Element);

						Element->Inited = false;
						Grouped->AddElement(Element);
					}
				}

				Element = Grouped;
			}

			if (Element != 0) {
				if (U->Type == uTex2D) SetTex2DValues (Vars->GetTex2D(it->first.c_str()), Element);

				Element->Inited = false;
				Element->Init();
				UniformPanel->AddElement(Element);
			}

			Element = 0;
		} // for

		VarsTabPanel->AddElement(UniformPanel);
		Parent = 0;
	}
}

// Texture 2D Set Values ----------------------------------------------------------------------->

inline void SetTex2DValues (Texture2D *Val, UIElement *Element) {
	//
	if (Element->Name == "etexpath")  {     ((UIEdit*) Element)->wText   = StrToWStr(Val->FileName);  Element->OnUnFocused = SetTex2DPath;    }
	if (Element->Name == "cbwraps")   { ((UICheckBox*) Element)->Checked = Val->WrapS  == GL_REPEAT;  Element->OnClick     = SetTex2DWrapS;   }
	if (Element->Name == "cbwrapt")   { ((UICheckBox*) Element)->Checked = Val->WrapT  == GL_REPEAT;  Element->OnClick     = SetTex2DWrapT;   }
	if (Element->Name == "cblinear")  { ((UICheckBox*) Element)->Checked = Val->Filter == GL_LINEAR;  Element->OnClick     = SetTex2DFilter;  }
	if (Element->Name == "cbmipmaps") { ((UICheckBox*) Element)->Checked = Val->BuildMipmaps;         Element->OnClick     = SetTex2DMipmaps; }
	if (Element->Name == "cbprema")   { ((UICheckBox*) Element)->Checked = Val->PremutableAlpha;      Element->OnClick     = SetTex2DPremA;   }
	//
	if (Element->Name == "btexpath")    Element->OnClick    = SetTex2DPathFinder;
	if (Element->Name == "transparent") Element->OnProgress = PrevBgVarProgress;
	if (Element->Name == "texture") {
		//
		Element->OnProgress = PrevTexVarProgress;
		((UIImage*) Element)->Texture = Val;
		((UIImage*) Element)->itOffsetX = 1.f;
		((UIImage*) Element)->itOffsetY = 1.f;

		((UIImage*) Element)->itWidth  = ((UIImage*) Element)->Texture->Width;
		((UIImage*) Element)->itHeight = ((UIImage*) Element)->Texture->Height;

		((UIImage*) Element)->Update();
	}

	Element->CustomData = Val;
}

// Elements Events

void PrevBgVarProgress (UIElement *Sender) {
	Sender->Width = Sender->Parent->Width-Sender->Parent->Padding[0]*2-2;
	((UIColorPanel*)Sender)->Update();
}

void PrevTexVarProgress (UIElement *Sender) {
	int w = Sender->Parent->Width-Sender->Parent->Padding[0]*2-2;

	ProportionsUI (((UIImage*) Sender)->Texture, Sender, w, 150);

	Sender->Top   = 10+75 - (Sender->Height >> 1);
	Sender->Left  = 10+(w  >> 1) - (Sender->Width  >> 1);
}

//

void SetTex2DWrapS (UIElement *Sender) {
	((UICheckBox*) Sender)->Checked ? ((Texture2D*) Sender->CustomData)->WrapS = GL_REPEAT :
									  ((Texture2D*) Sender->CustomData)->WrapS = GL_CLAMP;

	((Texture2D*) Sender->CustomData)->Update();
}

void SetTex2DWrapT (UIElement *Sender) {
	((UICheckBox*) Sender)->Checked ? ((Texture2D*) Sender->CustomData)->WrapT = GL_REPEAT :
									  ((Texture2D*) Sender->CustomData)->WrapT = GL_CLAMP;

	((Texture2D*) Sender->CustomData)->Update();
}

void SetTex2DFilter (UIElement *Sender) {
	((UICheckBox*) Sender)->Checked ? ((Texture2D*) Sender->CustomData)->Filter = GL_LINEAR :
									  ((Texture2D*) Sender->CustomData)->Filter = GL_NEAREST;

	((Texture2D*) Sender->CustomData)->Update();
}

void SetTex2DMipmaps (UIElement *Sender) {
	((Texture2D*) Sender->CustomData)->BuildMipmaps = ((UICheckBox*) Sender)->Checked;
	((Texture2D*) Sender->CustomData)->Update();
}

void SetTex2DPremA (UIElement *Sender) {
	((Texture2D*) Sender->CustomData)->PremutableAlpha = ((UICheckBox*) Sender)->Checked;
	((Texture2D*) Sender->CustomData)->Update();
}

void SetTex2DPath (UIElement *Sender) {

}

void UpdateVarTex() {
	if (Textures::TestExistTex->LoadFromFile (finder->FileName.c_str())) {
		CurVarTexture->LoadFromFile (finder->FileName.c_str());
		CurVarTexture->Update();
	} else ShowError (L"I Cant load texture :("); // FIXME

	CurVarTexture = 0;
}

void SetTex2DPathFinder (UIElement *Sender) {
	CurVarTexture = (Texture2D*) Sender->CustomData;
	finder->Open (ftOpenPicture, UpdateVarTex);
}

// <----------------------------------------------------------------------- Texture 2D Set Values


#endif
