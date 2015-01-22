//* Author : Kabilin Andrey, 2013-2014 *//

#ifndef CPP_COLOR_DIALOG
#define CPP_COLOR_DIALOG

//
glm::vec3 HSB2RGB (glm::vec3 HSB) {
	float H = HSB[0];
	float S = HSB[1];
	float B = HSB[2];

	float Hi = H/60.f;
	float Bmin = ((100.f-S)*B)/100.f;
	float a = (B-Bmin)*((int(H) % 60)/60.f);
	float Binc = Bmin+a;
	float Bdec = B-a;
	//
	B /= 100.f; Binc /= 100.f; Bmin /= 100.f; Bdec /= 100.f;
	//
	switch (int(Hi)) {
		case 0 : return glm::vec3 (B   , Binc, Bmin);
		case 1 : return glm::vec3 (Bdec, B   , Bmin);
		case 2 : return glm::vec3 (Bmin, B   , Binc);
		case 3 : return glm::vec3 (Bmin, Bdec, B   );
		case 4 : return glm::vec3 (Binc, Bmin, B   );
		case 5 : return glm::vec3 (B   , Bmin, Bdec);
	}

	return glm::vec3 (0, 0, 0);
}
//
glm::vec3 RGB2HSB (glm::vec3 RGB) {
	float R = RGB[0];
	float G = RGB[1];
	float B = RGB[2];
	//
	float rgb_max = max (R, G, B);
	float rgb_min = min (R, G, B);
	//
	float H, S;
	//
	if (feq<float>(rgb_max, rgb_min))		   H = 0.f;
	else if (feq<float>(rgb_max, R) && G >= B) H = 60.f * (G - B)/(rgb_max-rgb_min);
	else if (feq<float>(rgb_max, R) && G <  B) H = 60.f * (G - B)/(rgb_max-rgb_min) + 360.f;
	else if (feq<float>(rgb_max, G))		   H = 60.f * (B - R)/(rgb_max-rgb_min) + 120.f;
	else if (feq<float>(rgb_max, B))		   H = 60.f * (R - G)/(rgb_max-rgb_min) + 240.f;
	
	if (feq<float>(rgb_max, 0.f)) S = 0.f;
	else S = (1-rgb_min/rgb_max)*100.f;

	return glm::vec3 (H, S, rgb_max*100.f);
}
//
void ColorDialog::Init() {
	Dialog		= (UIDialog*) Data->Elements["colordialog"];
	ColorLine	=  (UIPanel*) Data->Elements["pcolorsline"];
	ColorPicker	=  (UIPanel*) Data->Elements["pcolorspanel"];
	Cursor		=  (UIImage*) Data->Elements["icolorcursor"];
	CursorLine	=  (UIImage*) Data->Elements["ilinecursor"];
	//
	CurColor = (UIColorPanel*) Data->Elements["pcurcolor"];
	NewColor = (UIColorPanel*) Data->Elements["pnewcolor"];
	//
	Data->Elements["bok"]      ->OnClick  = OnOKClick;
	Data->Elements["bcancel"]  ->OnClick  = OnCancelClick;
	Data->Elements["pcurcolor"]->OnClick = OnCurColorClick;
	//
	Data->Elements["bhsb_h"]->OnMouseDown = OnHSB_HClick;
	Data->Elements["bhsb_s"]->OnMouseDown = OnHSB_SClick;
	Data->Elements["bhsb_b"]->OnMouseDown = OnHSB_BClick;
	//
	Data->Elements["brgb_r"]->OnMouseDown = OnRGB_RClick;
	Data->Elements["brgb_g"]->OnMouseDown = OnRGB_GClick;
	Data->Elements["brgb_b"]->OnMouseDown = OnRGB_BClick;
	//
	// HSB
	eHSB_H  = (UIEdit*) Data->Elements["ehsb_h"];
	eHSB_S  = (UIEdit*) Data->Elements["ehsb_s"];
	eHSB_B  = (UIEdit*) Data->Elements["ehsb_b"];

	// RGBA
	eRGB_R  = (UIEdit*) Data->Elements["ergb_r"];
	eRGB_G  = (UIEdit*) Data->Elements["ergb_g"];
	eRGB_B  = (UIEdit*) Data->Elements["ergb_b"];
	//
	eAlpha  = (UIEdit*) Data->Elements["ealpha"];
	eHex	= (UIEdit*) Data->Elements["ehexcolor"];
	//
	eHSB_H ->OnChange = OnChangeHSB_H;
	eHSB_S ->OnChange = OnChangeHSB_S;
	eHSB_B ->OnChange = OnChangeHSB_B;
	//
	eRGB_R ->OnChange = OnChangeRGB_R;
	eRGB_G ->OnChange = OnChangeRGB_G;
	eRGB_B ->OnChange = OnChangeRGB_B;
	//
	eAlpha ->OnChange = OnChangeAlpha;
	eHex   ->OnChange = OnChangeHex;
	//
	ColorLine  ->Visible = false;
	ColorPicker->Visible = false;
	Cursor     ->Visible = false;
	//
	QuadElement = new BaseObject();
	QuadElement->Init();
	//
	QuadElement->Data    = GUIElementDat;
	QuadElement->mCamera = ScreenCamera;
	//
	ColorPallete = 0;
	ScrollX = 0; ScrollY = 0; ScrollZ = 0;
	MaxX = 100; MaxY = 100; MaxZ = 360;
	PosZ0 = CursorLine->Top-1;
	//
	cx = ColorPicker->Left-(Cursor->Width  >> 1);
	cy = ColorPicker->Top -(Cursor->Height >> 1);
	//
	PosX0 = cx; PosY0 = cy;
	Clicked = false;
	nLastColors = new List<glm::vec4*>();
	//
	for (int i = 0; i < 7; i++) {
		LastColors[i] = new UIColorPanel();
		UIColorPanel *BorderBlack = new UIColorPanel();
		UIColorPanel *BorderWhile = new UIColorPanel();

		((UIPanel*) Data->Elements["plast"])->AddElement(BorderBlack);
		((UIPanel*) Data->Elements["plast"])->AddElement(BorderWhile);
		((UIPanel*) Data->Elements["plast"])->AddElement(LastColors[i]);

		LastColors[i]->Theme       = ((UIPanel*) Data->Elements["plast"])->Theme;
		LastColors[i]->Shader      = ((UIPanel*) Data->Elements["plast"])->Shader;
		LastColors[i]->ColorShader = ((UIPanel*) Data->Elements["plast"])->ColorShader;
		LastColors[i]->Manager     = ((UIPanel*) Data->Elements["plast"])->Manager;
		LastColors[i]->Focused     = false;
		//
		glm::vec4 lc;
		lc[0] = Config->Data["lastcolors"].Func["color"+IntToStr(i+1)][0].Num;
		lc[1] = Config->Data["lastcolors"].Func["color"+IntToStr(i+1)][1].Num;
		lc[2] = Config->Data["lastcolors"].Func["color"+IntToStr(i+1)][2].Num;
		lc[3] = Config->Data["lastcolors"].Func["color"+IntToStr(i+1)][3].Num;
		//
		nLastColors->append(new glm::vec4(lc));
		//
		LastColors[i]->Color  = lc;
		LastColors[i]->Width  = 24;
		LastColors[i]->Height = 24;
		LastColors[i]->Left   = 2+32*i;
		LastColors[i]->Top    = 2;
		LastColors[i]->itWidth  = 24;
		LastColors[i]->itHeight = 24;
		//
		*BorderBlack = *LastColors[i];
		*BorderWhile = *LastColors[i];
		//
		BorderBlack->Color = glm::vec4 (0, 0, 0, 1);
		BorderWhile->Color = glm::vec4 (1, 1, 1, 1);
		//
		BorderBlack->Width = 28; BorderBlack->Height = 28;
		BorderWhile->Width = 26; BorderWhile->Height = 26;
		BorderBlack->Left -= 2;  BorderBlack->Top   -= 2;
		BorderWhile->Left -= 1;  BorderWhile->Top   -= 1;
		//
		LastColors[i]->OnClick = OnLastColorClick;
	}
	//SetCursorZ(0);
}

void ColorDialog::Open (glm::vec4 Color, VoidEvent Callback) {
	Dialog->Left = (WindowWidth  >> 1) - (Dialog->Width  >> 1);
	Dialog->Top  = (WindowHeight >> 1) - (Dialog->Height >> 1);
	Dialog->Open();
	
	ColorPallete = 0;
	UI_UncheckRGB();
	CurColor->Color = Color;
	NewColor->Color = Color;
	ColorLineClick   = false; Clicked = false;
	ColorPickerClick = false;
	//
	RGB[0] = Color[0]; RGB[1] = Color[1]; RGB[2] = Color[2];
	HSB = RGB2HSB (RGB);
	//
	MaxZ = 360; MaxX = 100; MaxY = 100;
	Alpha = Color[3];
	//
	SetCursorZ  (HSB[0]);
	SetCursorXY (HSB[1], HSB[2]);
	//
	UpdateHSBnRGB_XY();
	UpdateHSBnRGB_Z();
	//colorDialog->SetCursorsPos();
	//colorDialog->UpdateEdits();
	//
	LastHex = eHex->wText;
	this->Callback = Callback;
}

void ColorDialog::Render() {
	if (!Dialog->isOpened)
		return;

	ColorLineShader->ApplyShader();
	
	QuadElement->SetPosition (glm::vec2(Dialog->Left+ColorLine->Left, WindowHeight-ColorLine->Height-ColorLine->Top-Dialog->Top));
	QuadElement->SetSize (glm::vec2(ColorLine->Width, ColorLine->Height));
	QuadElement->UpdateModelMatrix();

	glUniformMatrix4fv (ColorLineShader->Locations->GetData("MVP"), 1, GL_FALSE, &(QuadElement->MVPMatrix[0][0]));
	glUniform1i        (ColorLineShader->Locations->GetData("Pallete"), ColorPallete);
	glUniform3fv       (ColorLineShader->Locations->GetData("HSB"), 1, &HSB2[0]);
	glUniform3fv       (ColorLineShader->Locations->GetData("RGB"), 1, &RGB[0]);

	QuadElement->EmptyRender();
	ColorLineShader->UnApplyShader();
	//
	ColorPickerShader->ApplyShader();
	
	QuadElement->SetPosition (glm::vec2(Dialog->Left+ColorPicker->Left, WindowHeight-ColorPicker->Height-ColorPicker->Top-Dialog->Top));
	QuadElement->SetSize (glm::vec2(ColorPicker->Width, ColorPicker->Height));
	QuadElement->UpdateModelMatrix();

	glUniformMatrix4fv (ColorPickerShader->Locations->GetData("MVP"), 1, GL_FALSE, &(QuadElement->MVPMatrix[0][0]));
	glUniform1i        (ColorPickerShader->Locations->GetData("Pallete"), ColorPallete);
	glUniform3fv       (ColorPickerShader->Locations->GetData("HSB"), 1, &HSB2[0]);
	glUniform3fv       (ColorPickerShader->Locations->GetData("RGB"), 1, &RGB[0]);

	QuadElement->EmptyRender();
	ColorPickerShader->UnApplyShader();
	//
	bool CursorInPicker = PointInRect (MouseX, MouseY, Dialog->Left+ColorPicker->Left, Dialog->Top+ColorPicker->Top,
									   ColorPicker->Width, ColorPicker->Height);

	Cursor->Shader->ApplyShader();

	//
	if (ColorLineClick) {
		CursorLine->Top = clamp (MouseY-Dialog->Top, ColorLine->Top, ColorLine->Top+ColorLine->Height);
		CursorLine->Top -= (CursorLine->Height >> 1)+1;
		//
		ScrollZ = MaxZ-((CursorLine->Top-PosZ0)*MaxZ)/(ColorLine->Top+ColorLine->Height-CursorLine->Height*2-2);
		UpdateHSBnRGB_Z();
	}
	//
	if (ColorPickerClick) {
		cx = clamp (MouseX-Dialog->Left-(Cursor->Width  >> 1), ColorPicker->Left-(Cursor->Width  >> 1), ColorPicker->Left+ColorPicker->Width -(Cursor->Width  >> 1));
		cy = clamp (MouseY-Dialog->Top -(Cursor->Height >> 1), ColorPicker->Top -(Cursor->Height >> 1), ColorPicker->Top +ColorPicker->Height-(Cursor->Height >> 1));
		//
		ScrollX = ((cx-PosX0)*MaxX)/(ColorPicker->Left+ColorPicker->Width -(Cursor->Width  >> 1)-3);
		ScrollY = MaxY-((cy-PosY0)*MaxY)/(ColorPicker->Height-(Cursor->Height >> 1)+7);

		UpdateHSBnRGB_XY();
	}

	glEnable  (GL_SCISSOR_TEST);
	glScissor (Dialog->Left+ColorPicker->Left, WindowHeight-ColorPicker->Height-Dialog->Top-ColorPicker->Top, ColorPicker->Width, ColorPicker->Height);
	Cursor->Draw (Dialog->Left+cx, Dialog->Top+cy);
	glDisable (GL_SCISSOR_TEST);

	//if ((CursorInPicker || ColorPickerClick) && !ColorLineClick) {
	if (CursorInPicker && (!Clicked || ColorPickerClick)) {
		HideCursor = true; InPicker = true;
		Cursor->Draw (MouseX-(Cursor->Width >> 1), MouseY-(Cursor->Height >> 1));
	} else if (InPicker) {
		InPicker = false;
		HideCursor = false;
	}
	//
	Cursor->Shader->UnApplyShader();
}

void ColorDialog::MouseDown (int x, int y, int shift) {
	Clicked = true;
	ColorPickerClick = PointInRect (x, y, Dialog->Left+ColorPicker->Left, Dialog->Top+ColorPicker->Top,
									ColorPicker->Width, ColorPicker->Height) && shift == mouseLeft;

	ColorLineClick   = PointInRect (x, y, Dialog->Left+ColorLine->Left, Dialog->Top+ColorLine->Top,
									ColorLine->Width, ColorLine->Height) && shift == mouseLeft;
}

void ColorDialog::MouseUp (int x, int y, int shift) {
	ColorLineClick   = false;
	ColorPickerClick = false;
	Clicked = false;
}

void ColorDialog::UpdateHSBnRGB_Z() {
	switch (ColorPallete) {
		// HSB
		case 0 : HSB[0] = ScrollZ; break;
		case 1 : HSB[1] = ScrollZ; break;
		case 2 : HSB[2] = ScrollZ; break;
		//
		// RGB
		case 3 : RGB[0] = ScrollZ/255.f; break;
		case 4 : RGB[1] = ScrollZ/255.f; break;
		case 5 : RGB[2] = ScrollZ/255.f; break;
	}
	//
	if (ColorPallete >= 3) HSB = RGB2HSB (RGB);

	HSB2 = HSB;
	if (HSB2[0] >= 360.f-numeric_limits<float>::epsilon()) HSB2[0] = 0;
	if (ColorPallete >= 0 && ColorPallete <= 2) RGB = HSB2RGB (HSB2);

	NewColor->Color = glm::vec4 (RGB, Alpha);
	UpdateEdits();
}
//
void ColorDialog::UpdateHSBnRGB_XY() {
	switch (ColorPallete) {
		// HSB
		case 0 : HSB[1] = ScrollX; HSB[2] = ScrollY; break;
		case 1 : HSB[0] = ScrollX; HSB[2] = ScrollY; break;
		case 2 : HSB[0] = ScrollX; HSB[1] = ScrollY; break;
		//
		// RGB
		case 3 : RGB[1] = ScrollY/255.f; RGB[2] = ScrollX/255.f; break;
		case 4 : RGB[0] = ScrollY/255.f; RGB[2] = ScrollX/255.f; break;
		case 5 : RGB[0] = ScrollX/255.f; RGB[1] = ScrollY/255.f; break;
	}
	//
	if (ColorPallete >= 3) HSB = RGB2HSB (RGB);

	HSB2 = HSB;
	if (HSB2[0] >= 360.f-numeric_limits<float>::epsilon()) HSB2[0] = 0;
	if (ColorPallete >= 0 && ColorPallete <= 2) RGB = HSB2RGB (HSB2);

	NewColor->Color = glm::vec4 (RGB, Alpha);
	UpdateEdits();
}
//
void ColorDialog::SetCursorZ (const int Sz) {
	ScrollZ = Sz;
	CursorLine->Top = ((ColorLine->Top+ColorLine->Height-CursorLine->Height*2-2)*(MaxZ-ScrollZ))/MaxZ+PosZ0;
}

void ColorDialog::SetCursorXY (const int Sx, const int Sy) {
	ScrollX = Sx; ScrollY = Sy;
	//
	cx = ((ColorPicker->Left+ColorPicker->Width -(Cursor->Width  >> 1)-2)*ScrollX)/MaxX+PosX0;
	cy = ((ColorPicker->Height-(Cursor->Height >> 1)+7)*(MaxY-ScrollY))/MaxY+PosY0;
}

//
void ColorDialog::SetCursorsPos() {
	switch (ColorPallete) {
		case 0 : SetCursorZ (HSB[0]); SetCursorXY (HSB[1], HSB[2]); break;
		case 1 : SetCursorZ (HSB[1]); SetCursorXY (HSB[0], HSB[2]); break;
		case 2 : SetCursorZ (HSB[2]); SetCursorXY (HSB[0], HSB[1]); break;
		//
		case 3 : SetCursorZ (RGB[0]*255.f); SetCursorXY (RGB[2]*255.f, RGB[1]*255.f); break;
		case 4 : SetCursorZ (RGB[1]*255.f); SetCursorXY (RGB[2]*255.f, RGB[0]*255.f); break;
		case 5 : SetCursorZ (RGB[2]*255.f); SetCursorXY (RGB[0]*255.f, RGB[1]*255.f); break;
	}
}
// Events
void ColorDialog::OnOKClick (UIElement *Sender) {
	colorDialog->nLastColors->first();

	if (!feq<float>(colorDialog->NewColor->Color[0], (*(colorDialog->nLastColors->val()))[0], 0.05f) ||
		!feq<float>(colorDialog->NewColor->Color[1], (*(colorDialog->nLastColors->val()))[1], 0.05f) ||
		!feq<float>(colorDialog->NewColor->Color[2], (*(colorDialog->nLastColors->val()))[2], 0.05f) ||
		!feq<float>(colorDialog->NewColor->Color[3], (*(colorDialog->nLastColors->val()))[3], 0.05f))
	{
		colorDialog->nLastColors->prev();
		colorDialog->nLastColors->insert(new glm::vec4(colorDialog->NewColor->Color));
		colorDialog->nLastColors->last();
		colorDialog->nLastColors->remove();
		colorDialog->nLastColors->first();
	}
	//
	for (int i = 0; i < 7; i++) {
		colorDialog->LastColors[i]->Color = *(colorDialog->nLastColors->val());
		colorDialog->nLastColors->next();

		//
		Config->Data["lastcolors"].Func["color"+IntToStr(i+1)][0].Num = colorDialog->LastColors[i]->Color[0];
		Config->Data["lastcolors"].Func["color"+IntToStr(i+1)][1].Num = colorDialog->LastColors[i]->Color[1];
		Config->Data["lastcolors"].Func["color"+IntToStr(i+1)][2].Num = colorDialog->LastColors[i]->Color[2];
		Config->Data["lastcolors"].Func["color"+IntToStr(i+1)][3].Num = colorDialog->LastColors[i]->Color[3];
	}

	colorDialog->Dialog->Close();

	if (colorDialog->Callback != 0)
		colorDialog->Callback();
}

void ColorDialog::OnCancelClick (UIElement *Sender) {
	colorDialog->NewColor->Color = colorDialog->CurColor->Color;
	colorDialog->Dialog->Close();
}

void ColorDialog::OnCurColorClick (UIElement *Sender) {
	colorDialog->NewColor->Color = colorDialog->CurColor->Color;
	
	colorDialog->RGB[0] = colorDialog->NewColor->Color[0];
	colorDialog->RGB[1] = colorDialog->NewColor->Color[1];
	colorDialog->RGB[2] = colorDialog->NewColor->Color[2];
	colorDialog->Alpha  = colorDialog->NewColor->Color[3];

	colorDialog->HSB  = RGB2HSB (colorDialog->RGB);
	colorDialog->HSB2 = colorDialog->HSB;
	//
	colorDialog->SetCursorsPos();
	colorDialog->UpdateEdits();
}

void ColorDialog::OnLastColorClick (UIElement *Sender) {
	colorDialog->NewColor->Color = ((UIColorPanel*)Sender)->Color;

	colorDialog->RGB[0] = colorDialog->NewColor->Color[0];
	colorDialog->RGB[1] = colorDialog->NewColor->Color[1];
	colorDialog->RGB[2] = colorDialog->NewColor->Color[2];
	colorDialog->Alpha  = colorDialog->NewColor->Color[3];

	colorDialog->HSB  = RGB2HSB (colorDialog->RGB);
	colorDialog->HSB2 = colorDialog->HSB;
	//
	colorDialog->SetCursorsPos();
	colorDialog->UpdateEdits();
}

//
void ColorDialog::OnHSB_HClick (UIElement *Sender, int x, int y, int shift) {
	if (!Sender->Checked || colorDialog->ColorPallete == 0)
		return;

	colorDialog->ColorPallete = 0;
	colorDialog->UI_UncheckRGB();
	//
	colorDialog->MaxZ = 360;
	colorDialog->MaxX = 100;
	colorDialog->MaxY = 100;

	colorDialog->SetCursorZ  (colorDialog->HSB[0]);
	colorDialog->SetCursorXY (colorDialog->HSB[1], colorDialog->HSB[2]);
}

void ColorDialog::OnHSB_SClick (UIElement *Sender, int x, int y, int shift) {
	if (!Sender->Checked || colorDialog->ColorPallete == 1)
		return;

	colorDialog->ColorPallete = 1;
	colorDialog->UI_UncheckRGB();
	//
	colorDialog->MaxZ = 100;
	colorDialog->MaxX = 360;
	colorDialog->MaxY = 100;

	colorDialog->SetCursorZ  (colorDialog->HSB[1]);
	colorDialog->SetCursorXY (colorDialog->HSB[0], colorDialog->HSB[2]);
}

void ColorDialog::OnHSB_BClick (UIElement *Sender, int x, int y, int shift) {
	if (!Sender->Checked || colorDialog->ColorPallete == 2)
		return;

	colorDialog->ColorPallete = 2;
	colorDialog->UI_UncheckRGB();
	//
	colorDialog->MaxZ = 100;
	colorDialog->MaxX = 360;
	colorDialog->MaxY = 100;

	colorDialog->SetCursorZ  (colorDialog->HSB[2]);
	colorDialog->SetCursorXY (colorDialog->HSB[0], colorDialog->HSB[1]);
}

void ColorDialog::OnRGB_RClick (UIElement *Sender, int x, int y, int shift) {
	if (!Sender->Checked || colorDialog->ColorPallete == 3)
		return;

	colorDialog->ColorPallete = 3;
	colorDialog->UI_UncheckHSB();
	//
	colorDialog->MaxZ = 255;
	colorDialog->MaxX = 255;
	colorDialog->MaxY = 255;

	colorDialog->SetCursorZ  (colorDialog->RGB[0]*255.f);
	colorDialog->SetCursorXY (colorDialog->RGB[2]*255.f, colorDialog->RGB[1]*255.f);
}

void ColorDialog::OnRGB_GClick (UIElement *Sender, int x, int y, int shift) {
	if (!Sender->Checked || colorDialog->ColorPallete == 4)
		return;

	colorDialog->ColorPallete = 4;
	colorDialog->UI_UncheckHSB();
	//
	colorDialog->MaxZ = 255;
	colorDialog->MaxX = 255;
	colorDialog->MaxY = 255;

	colorDialog->SetCursorZ  (colorDialog->RGB[1]*255.f);
	colorDialog->SetCursorXY (colorDialog->RGB[2]*255.f, colorDialog->RGB[0]*255.f);
}

void ColorDialog::OnRGB_BClick (UIElement *Sender, int x, int y, int shift) {
	if (!Sender->Checked || colorDialog->ColorPallete == 5)
		return;

	colorDialog->ColorPallete = 5;
	colorDialog->UI_UncheckHSB();
	//
	colorDialog->MaxZ = 255;
	colorDialog->MaxX = 255;
	colorDialog->MaxY = 255;

	colorDialog->SetCursorZ  (colorDialog->RGB[2]*255.f);
	colorDialog->SetCursorXY (colorDialog->RGB[0]*255.f, colorDialog->RGB[1]*255.f);
}

// Params ---------------------------------------------------------->
// HSB
void ColorDialog::OnChangeHSB_H (UIElement *Sender) {
	colorDialog->HSB[0] = WStrToInt (colorDialog->eHSB_H->wText);
	colorDialog->RGB    = HSB2RGB   (colorDialog->HSB);
	colorDialog->HSB2   = colorDialog->HSB;
	//
	if (colorDialog->HSB2[0] >= 360.f-numeric_limits<float>::epsilon())
		colorDialog->HSB2[0]  = 0;
	//
	colorDialog->SetCursorsPos();
	colorDialog->UpdateEdits();
	colorDialog->NewColor->Color = glm::vec4 (colorDialog->RGB, colorDialog->Alpha);
}

void ColorDialog::OnChangeHSB_S (UIElement *Sender) {
	colorDialog->HSB[1] = WStrToInt (colorDialog->eHSB_S->wText);
	colorDialog->RGB    = HSB2RGB   (colorDialog->HSB);
	colorDialog->HSB2   = colorDialog->HSB;
	//
	colorDialog->SetCursorsPos();
	colorDialog->UpdateEdits();
	colorDialog->NewColor->Color = glm::vec4 (colorDialog->RGB, colorDialog->Alpha);
}

void ColorDialog::OnChangeHSB_B (UIElement *Sender) {
	colorDialog->HSB[2] = WStrToInt (colorDialog->eHSB_B->wText);
	colorDialog->RGB    = HSB2RGB   (colorDialog->HSB);
	colorDialog->HSB2   = colorDialog->HSB;
	//
	colorDialog->SetCursorsPos();
	colorDialog->UpdateEdits();
	colorDialog->NewColor->Color = glm::vec4 (colorDialog->RGB, colorDialog->Alpha);
}

// RGBA
void ColorDialog::OnChangeRGB_R (UIElement *Sender) {
	colorDialog->RGB[0] = WStrToInt (colorDialog->eRGB_R->wText)/255.f;
	colorDialog->HSB    = RGB2HSB   (colorDialog->RGB);
	colorDialog->HSB2   = colorDialog->HSB;
	//
	colorDialog->SetCursorsPos();
	colorDialog->UpdateEdits();
	colorDialog->NewColor->Color = glm::vec4 (colorDialog->RGB, colorDialog->Alpha);
}

void ColorDialog::OnChangeRGB_G (UIElement *Sender) {
	colorDialog->RGB[1] = WStrToInt (colorDialog->eRGB_G->wText)/255.f;
	colorDialog->HSB    = RGB2HSB   (colorDialog->RGB);
	colorDialog->HSB2   = colorDialog->HSB;
	//
	colorDialog->SetCursorsPos();
	colorDialog->UpdateEdits();
	colorDialog->NewColor->Color = glm::vec4 (colorDialog->RGB, colorDialog->Alpha);
}

void ColorDialog::OnChangeRGB_B (UIElement *Sender) {
	colorDialog->RGB[2] = WStrToInt (colorDialog->eRGB_B->wText)/255.f;
	colorDialog->HSB    = RGB2HSB   (colorDialog->RGB);
	colorDialog->HSB2   = colorDialog->HSB;
	//
	colorDialog->SetCursorsPos();
	colorDialog->UpdateEdits();
	colorDialog->NewColor->Color = glm::vec4 (colorDialog->RGB, colorDialog->Alpha);
}

void ColorDialog::OnChangeAlpha (UIElement *Sender) {
	colorDialog->Alpha = WStrToInt(colorDialog->eAlpha->wText)/100.f;
	colorDialog->NewColor->Color = glm::vec4 (colorDialog->RGB, colorDialog->Alpha);
}

void ColorDialog::OnChangeHex (UIElement *Sender) {
	bool change = false;

	if (colorDialog->eHex->wText.size() == 3) {
		for (int i = 0; i < 3; i++) {
			wchar_t wc = colorDialog->eHex->wText[i];
			wstring hex;
			hex += wc; hex += wc;
			colorDialog->RGB[i] = HexToInt (hex)/255.f;
		}

		change = true;
	}

	if (colorDialog->eHex->wText.size() == 6) {
		for (int i = 0; i < 6; i += 2) {
			wchar_t wc1 = colorDialog->eHex->wText[i];
			wchar_t wc2 = colorDialog->eHex->wText[i+1];
			wstring hex;
			hex += wc2; hex += wc1;
			colorDialog->RGB[i/2] = HexToInt (hex)/255.f;
		}

		change = true;
	}

	if (change) {
		colorDialog->HSB  = RGB2HSB (colorDialog->RGB);
		colorDialog->HSB2 = colorDialog->HSB;
		//
		colorDialog->SetCursorsPos();
		colorDialog->UpdateEdits();
		colorDialog->NewColor->Color = glm::vec4 (colorDialog->RGB, colorDialog->Alpha);
	}
	//
	if (!change)
		colorDialog->eHex->wText = colorDialog->LastHex;
}

// <---------------------------------------------------------- Params

#endif
