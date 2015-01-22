//
#ifndef CPP_UIPRECOMPUTED
#define CPP_UIPRECOMPUTED

//----------------------------------------------------------------------------//
// UILabel
//----------------------------------------------------------------------------//

void UILabel::Init() {
	if (Inited) return;
	Inited = true;

	// Text
	TextColor[0] = Theme->Data["label"].Func["textcolor"][0].Num/255.0f;
	TextColor[1] = Theme->Data["label"].Func["textcolor"][1].Num/255.0f;
	TextColor[2] = Theme->Data["label"].Func["textcolor"][2].Num/255.0f;
	TextColor[0] = 0.0f;
	TextColor[1] = 0.0f;
	TextColor[2] = 0.0f;
	//
	TextOffset[0]  = floor(Theme->Data["label"].Func["textoffset"][0].Num);
	TextOffset[1]  = floor(Theme->Data["label"].Func["textoffset"][1].Num);
	//
	TextShadow = false;
	
	/*if (Theme->Data["label"].Func["shadow"][0].Num == 1.0f) {
		// Shadow
		ShadowColor[0] = Theme->Data["label"].Func["shadowcolor"][0].Num/255.0f;
		ShadowColor[1] = Theme->Data["label"].Func["shadowcolor"][1].Num/255.0f;
		ShadowColor[2] = Theme->Data["label"].Func["shadowcolor"][2].Num/255.0f;
		//
		ShadowOffset[0]  = floor(Theme->Data["label"].Func["shadowoffset"][0].Num);
		ShadowOffset[1]  = floor(Theme->Data["label"].Func["shadowoffset"][1].Num);
		//
		TextShadow = true;
	}*/
	//
	//TextAlign = alLeft;
}

//----------------------------------------------------------------------------//
// UIHr
//----------------------------------------------------------------------------//

void UIHr::Init() {
	if (Inited) return;
	Inited = true;

	//
	QuadElement = new BaseObject();
	QuadElement->Init();
	//
	QuadElement->Data    = GUIElementDat;
	QuadElement->mCamera = ScreenCamera;
	QuadElement->Shader  = Shader;

	OffsetX = Theme->Data["general"].Func["hr"][0].Num / Theme->Skin->Width;
	OffsetY = Theme->Data["general"].Func["hr"][1].Num / Theme->Skin->Height;
	tWidth  = Theme->Data["general"].Func["hr"][2].Num / Theme->Skin->Width;
	tHeight = Theme->Data["general"].Func["hr"][3].Num / Theme->Skin->Height;
	iWidth  = floor(Theme->Data["general"].Func["hr"][2].Num);
	iHeight = floor(Theme->Data["general"].Func["hr"][3].Num);
}

//----------------------------------------------------------------------------//
// UIImage
//----------------------------------------------------------------------------//

void UIImage::Init() {
	if (Inited) return;
	Inited = true;

	//
	QuadElement = new BaseObject();
	QuadElement->Init();
	//
	QuadElement->Data    = GUIElementDat;
	QuadElement->mCamera = ScreenCamera;
	QuadElement->Shader  = Shader;
	//
	if (Texture != 0) {
		OffsetX = (float) itOffsetX / (float) Texture->Width;
		OffsetY = (float) itOffsetY / (float) Texture->Height;
		tWidth  = (float) itWidth   / (float) Texture->Width;
		tHeight = (float) itHeight  / (float) Texture->Height;
	}
}

void UIImage::Update() {
	OffsetX = (float) itOffsetX / (float) Texture->Width;
	OffsetY = (float) itOffsetY / (float) Texture->Height;
	tWidth  = (float) itWidth   / (float) Texture->Width;
	tHeight = (float) itHeight  / (float) Texture->Height;
}

//----------------------------------------------------------------------------//
// UIColorPanel
//----------------------------------------------------------------------------//

void UIColorPanel::Init() {
	if (Inited) return;
	Inited = true;

	//
	QuadElement = new BaseObject();
	QuadElement->Init();
	//
	QuadElement->Data    = GUIElementDat;
	QuadElement->mCamera = ScreenCamera;
	QuadElement->Shader  = Shader;
	//
	tWidth  = (float) itWidth  / (float) Textures::ImageGrid->Width;
	tHeight = (float) itHeight / (float) Textures::ImageGrid->Height;
}


//----------------------------------------------------------------------------//
// UIButton
//----------------------------------------------------------------------------//

void UIButton::Init() {
	if (Inited) return;
	Inited = true;

	// Pre-computed
	// Leav //
	// Left
	OffsetsX[0] = Theme->Data[LeavParam].Func["left"][0].Num / Theme->Skin->Width;
	OffsetsY[0] = Theme->Data[LeavParam].Func["left"][1].Num / Theme->Skin->Height;
	Widths  [0] = Theme->Data[LeavParam].Func["left"][2].Num / Theme->Skin->Width;
	Heights [0] = Theme->Data[LeavParam].Func["left"][3].Num / Theme->Skin->Height;
	iWidths [0] = floor(Theme->Data[LeavParam].Func["left"][2].Num);
	iHeights[0] = floor(Theme->Data[LeavParam].Func["left"][3].Num);
	
	// Center
	OffsetsX[1] = Theme->Data[LeavParam].Func["center"][0].Num / Theme->Skin->Width;
	OffsetsY[1] = Theme->Data[LeavParam].Func["center"][1].Num / Theme->Skin->Height;
	Widths  [1] = Theme->Data[LeavParam].Func["center"][2].Num / Theme->Skin->Width;
	Heights [1] = Theme->Data[LeavParam].Func["center"][3].Num / Theme->Skin->Height;
	iWidths [1] = floor(Theme->Data[LeavParam].Func["center"][2].Num);
	iHeights[1] = floor(Theme->Data[LeavParam].Func["center"][3].Num);
	
	// Right
	OffsetsX[2] = Theme->Data[LeavParam].Func["right"][0].Num / Theme->Skin->Width;
	OffsetsY[2] = Theme->Data[LeavParam].Func["right"][1].Num / Theme->Skin->Height;
	Widths  [2] = Theme->Data[LeavParam].Func["right"][2].Num / Theme->Skin->Width;
	Heights [2] = Theme->Data[LeavParam].Func["right"][3].Num / Theme->Skin->Height;
	iWidths [2] = floor(Theme->Data[LeavParam].Func["right"][2].Num);
	iHeights[2] = floor(Theme->Data[LeavParam].Func["right"][3].Num);
	
	// Text
	TextColors[0]   = Theme->Data[LeavParam].Func["textcolor"][0].Num/255.0f;
	TextColors[1]   = Theme->Data[LeavParam].Func["textcolor"][1].Num/255.0f;
	TextColors[2]   = Theme->Data[LeavParam].Func["textcolor"][2].Num/255.0f;
	//
	TextOffsets[0]  = floor(Theme->Data[LeavParam].Func["textoffset"][0].Num);
	TextOffsets[1]  = floor(Theme->Data[LeavParam].Func["textoffset"][1].Num);

	if (Theme->Data[LeavParam].Func.count("iconoffset") != 0)
		IcoOffset = floor(Theme->Data[LeavParam].Func["iconoffset"][0].Num);
	
	/*if (Theme->Data[LeavParam].Func["shadow"][0].Num == 1.0f) {
		// Shadow
		ShadowColors[0] = Theme->Data[LeavParam].Func["shadowcolor"][0].Num/255.0f;
		ShadowColors[1] = Theme->Data[LeavParam].Func["shadowcolor"][1].Num/255.0f;
		ShadowColors[2] = Theme->Data[LeavParam].Func["shadowcolor"][2].Num/255.0f;
		//
		ShadowOffsets[0]  = floor(Theme->Data[LeavParam].Func["shadowoffset"][0].Num);
		ShadowOffsets[1]  = floor(Theme->Data[LeavParam].Func["shadowoffset"][1].Num);
		//
		TextShadows[0] = true;
	}*/
	//---
	
	// Enter //
	// Left
	OffsetsX[3] = Theme->Data[EnterParam].Func["left"][0].Num / Theme->Skin->Width;
	OffsetsY[3] = Theme->Data[EnterParam].Func["left"][1].Num / Theme->Skin->Height;
	Widths  [3] = Theme->Data[EnterParam].Func["left"][2].Num / Theme->Skin->Width;
	Heights [3] = Theme->Data[EnterParam].Func["left"][3].Num / Theme->Skin->Height;
	iWidths [3] = floor(Theme->Data[EnterParam].Func["left"][2].Num);
	iHeights[3] = floor(Theme->Data[EnterParam].Func["left"][3].Num);
	
	// Center
	OffsetsX[4] = Theme->Data[EnterParam].Func["center"][0].Num / Theme->Skin->Width;
	OffsetsY[4] = Theme->Data[EnterParam].Func["center"][1].Num / Theme->Skin->Height;
	Widths  [4] = Theme->Data[EnterParam].Func["center"][2].Num / Theme->Skin->Width;
	Heights [4] = Theme->Data[EnterParam].Func["center"][3].Num / Theme->Skin->Height;
	iWidths [4] = floor(Theme->Data[EnterParam].Func["center"][2].Num);
	iHeights[4] = floor(Theme->Data[EnterParam].Func["center"][3].Num);
	
	// Right
	OffsetsX[5] = Theme->Data[EnterParam].Func["right"][0].Num / Theme->Skin->Width;
	OffsetsY[5] = Theme->Data[EnterParam].Func["right"][1].Num / Theme->Skin->Height;
	Widths  [5] = Theme->Data[EnterParam].Func["right"][2].Num / Theme->Skin->Width;
	Heights [5] = Theme->Data[EnterParam].Func["right"][3].Num / Theme->Skin->Height;
	iWidths [5] = floor(Theme->Data[EnterParam].Func["right"][2].Num);
	iHeights[5] = floor(Theme->Data[EnterParam].Func["right"][3].Num);
	
	// Text
	TextColors[3]   = Theme->Data[EnterParam].Func["textcolor"][0].Num/255.0f;
	TextColors[4]   = Theme->Data[EnterParam].Func["textcolor"][1].Num/255.0f;
	TextColors[5]   = Theme->Data[EnterParam].Func["textcolor"][2].Num/255.0f;
	//
	TextOffsets[2]  = floor(Theme->Data[EnterParam].Func["textoffset"][0].Num);
	TextOffsets[3]  = floor(Theme->Data[EnterParam].Func["textoffset"][1].Num);
	
	/*if (Theme->Data[EnterParam].Func["shadow"][0].Num == 1.0f) {
		// Shadow
		ShadowColors[3] = Theme->Data[EnterParam].Func["shadowcolor"][0].Num/255.0f;
		ShadowColors[4] = Theme->Data[EnterParam].Func["shadowcolor"][1].Num/255.0f;
		ShadowColors[5] = Theme->Data[EnterParam].Func["shadowcolor"][2].Num/255.0f;
		//
		ShadowOffsets[2]  = floor(Theme->Data[EnterParam].Func["shadowoffset"][0].Num);
		ShadowOffsets[3]  = floor(Theme->Data[EnterParam].Func["shadowoffset"][1].Num);
		//
		TextShadows[1] = true;
	}*/
	//---
	
	// Click //
	// Left
	OffsetsX[6] = Theme->Data[ClickParam].Func["left"][0].Num / Theme->Skin->Width;
	OffsetsY[6] = Theme->Data[ClickParam].Func["left"][1].Num / Theme->Skin->Height;
	Widths  [6] = Theme->Data[ClickParam].Func["left"][2].Num / Theme->Skin->Width;
	Heights [6] = Theme->Data[ClickParam].Func["left"][3].Num / Theme->Skin->Height;
	iWidths [6] = floor(Theme->Data[ClickParam].Func["left"][2].Num);
	iHeights[6] = floor(Theme->Data[ClickParam].Func["left"][3].Num);
	
	// Center
	OffsetsX[7] = Theme->Data[ClickParam].Func["center"][0].Num / Theme->Skin->Width;
	OffsetsY[7] = Theme->Data[ClickParam].Func["center"][1].Num / Theme->Skin->Height;
	Widths  [7] = Theme->Data[ClickParam].Func["center"][2].Num / Theme->Skin->Width;
	Heights [7] = Theme->Data[ClickParam].Func["center"][3].Num / Theme->Skin->Height;
	iWidths [7] = floor(Theme->Data[ClickParam].Func["center"][2].Num);
	iHeights[7] = floor(Theme->Data[ClickParam].Func["center"][3].Num);
	
	// Right
	OffsetsX[8] = Theme->Data[ClickParam].Func["right"][0].Num / Theme->Skin->Width;
	OffsetsY[8] = Theme->Data[ClickParam].Func["right"][1].Num / Theme->Skin->Height;
	Widths  [8] = Theme->Data[ClickParam].Func["right"][2].Num / Theme->Skin->Width;
	Heights [8] = Theme->Data[ClickParam].Func["right"][3].Num / Theme->Skin->Height;
	iWidths [8] = floor(Theme->Data[ClickParam].Func["right"][2].Num);
	iHeights[8] = floor(Theme->Data[ClickParam].Func["right"][3].Num);
	
	// Text
	TextColors[6]   = Theme->Data[ClickParam].Func["textcolor"][0].Num/255.0f;
	TextColors[7]   = Theme->Data[ClickParam].Func["textcolor"][1].Num/255.0f;
	TextColors[8]   = Theme->Data[ClickParam].Func["textcolor"][2].Num/255.0f;
	//
	TextOffsets[4]  = floor(Theme->Data[ClickParam].Func["textoffset"][0].Num);
	TextOffsets[5]  = floor(Theme->Data[ClickParam].Func["textoffset"][1].Num);
	
	/*if (Theme->Data[ClickParam].Func["shadow"][0].Num == 1.0f) {
		// Shadow
		ShadowColors[6] = Theme->Data[ClickParam].Func["shadowcolor"][0].Num/255.0f;
		ShadowColors[7] = Theme->Data[ClickParam].Func["shadowcolor"][1].Num/255.0f;
		ShadowColors[8] = Theme->Data[ClickParam].Func["shadowcolor"][2].Num/255.0f;
		//
		ShadowOffsets[4]  = floor(Theme->Data[ClickParam].Func["shadowoffset"][0].Num);
		ShadowOffsets[5]  = floor(Theme->Data[ClickParam].Func["shadowoffset"][1].Num);
		//
		TextShadows[2] = true;
	}*/
	//---
	
	Height = iHeights[1];

	//
	LeftElement = new BaseObject();
	LeftElement->Init();
	//
	LeftElement->Data    = GUIElementDat;
	LeftElement->mCamera = ScreenCamera;
	LeftElement->Shader  = Shader;

	//
	RightElement = new BaseObject();
	RightElement->Init();
	//
	RightElement->Data    = GUIElementDat;
	RightElement->mCamera = ScreenCamera;
	RightElement->Shader  = Shader;

	//
	MiddleElement = new BaseObject();
	MiddleElement->Init();
	//
	MiddleElement->Data    = GUIElementDat;
	MiddleElement->mCamera = ScreenCamera;
	MiddleElement->Shader  = Shader;
	//
	IconElement = new BaseObject();
	IconElement->Init();
	//
	IconElement->Data    = GUIElementDat;
	IconElement->mCamera = ScreenCamera;
	IconElement->Shader  = Shader;
	//
	IconElement2 = new BaseObject();
	IconElement2->Init();
	//
	IconElement2->Data    = GUIElementDat;
	IconElement2->mCamera = ScreenCamera;
	IconElement2->Shader  = Shader;
	//
	if (DrawAlign == alLeft || DrawAlign == alRight)
		Width += iWidths[0];
}

//----------------------------------------------------------------------------//
// UICheckBox
//----------------------------------------------------------------------------//

void UICheckBox::Init() {
	if (Inited) return;
	Inited = true;

	// Pre-computed
	
	// Leav //
	// Unchecked
	OffsetsX[0] = Theme->Data["checkboxleav"].Func["unchecked"][0].Num / Theme->Skin->Width;
	OffsetsY[0] = Theme->Data["checkboxleav"].Func["unchecked"][1].Num / Theme->Skin->Height;
	Widths  [0] = Theme->Data["checkboxleav"].Func["unchecked"][2].Num / Theme->Skin->Width;
	Heights [0] = Theme->Data["checkboxleav"].Func["unchecked"][3].Num / Theme->Skin->Height;
	iWidths [0] = floor(Theme->Data["checkboxleav"].Func["unchecked"][2].Num);
	iHeights[0] = floor(Theme->Data["checkboxleav"].Func["unchecked"][3].Num);
	
	// Checked
	OffsetsX[1] = Theme->Data["checkboxleav"].Func["checked"][0].Num / Theme->Skin->Width;
	OffsetsY[1] = Theme->Data["checkboxleav"].Func["checked"][1].Num / Theme->Skin->Height;
	Widths  [1] = Theme->Data["checkboxleav"].Func["checked"][2].Num / Theme->Skin->Width;
	Heights [1] = Theme->Data["checkboxleav"].Func["checked"][3].Num / Theme->Skin->Height;
	iWidths [1] = floor(Theme->Data["checkboxleav"].Func["checked"][2].Num);
	iHeights[1] = floor(Theme->Data["checkboxleav"].Func["checked"][3].Num);
	
	// Text
	TextColors[0]   = Theme->Data["checkboxleav"].Func["textcolor"][0].Num/255.0f;
	TextColors[1]   = Theme->Data["checkboxleav"].Func["textcolor"][1].Num/255.0f;
	TextColors[2]   = Theme->Data["checkboxleav"].Func["textcolor"][2].Num/255.0f;
	//
	TextOffsets[0]  = floor(Theme->Data["checkboxleav"].Func["textoffset"][0].Num);
	TextOffsets[1]  = floor(Theme->Data["checkboxleav"].Func["textoffset"][1].Num);
	
	/*if (Theme->Data["checkboxleav"].Func["shadow"][0].Num == 1.0f) {
		// Shadow
		ShadowColors[0] = Theme->Data["checkboxleav"].Func["shadowcolor"][0].Num/255.0f;
		ShadowColors[1] = Theme->Data["checkboxleav"].Func["shadowcolor"][1].Num/255.0f;
		ShadowColors[2] = Theme->Data["checkboxleav"].Func["shadowcolor"][2].Num/255.0f;
		//
		ShadowOffsets[0]  = floor(Theme->Data["checkboxleav"].Func["shadowoffset"][0].Num);
		ShadowOffsets[1]  = floor(Theme->Data["checkboxleav"].Func["shadowoffset"][1].Num);
		//
		TextShadows[0] = true;
	}*/
	//---
	
	// Enter //
	// Unchecked
	OffsetsX[2] = Theme->Data["checkboxenter"].Func["unchecked"][0].Num / Theme->Skin->Width;
	OffsetsY[2] = Theme->Data["checkboxenter"].Func["unchecked"][1].Num / Theme->Skin->Height;
	Widths  [2] = Theme->Data["checkboxenter"].Func["unchecked"][2].Num / Theme->Skin->Width;
	Heights [2] = Theme->Data["checkboxenter"].Func["unchecked"][3].Num / Theme->Skin->Height;
	iWidths [2] = floor(Theme->Data["checkboxenter"].Func["unchecked"][2].Num);
	iHeights[2] = floor(Theme->Data["checkboxenter"].Func["unchecked"][3].Num);
	
	// Checked
	OffsetsX[3] = Theme->Data["checkboxenter"].Func["checked"][0].Num / Theme->Skin->Width;
	OffsetsY[3] = Theme->Data["checkboxenter"].Func["checked"][1].Num / Theme->Skin->Height;
	Widths  [3] = Theme->Data["checkboxenter"].Func["checked"][2].Num / Theme->Skin->Width;
	Heights [3] = Theme->Data["checkboxenter"].Func["checked"][3].Num / Theme->Skin->Height;
	iWidths [3] = floor(Theme->Data["checkboxenter"].Func["checked"][2].Num);
	iHeights[3] = floor(Theme->Data["checkboxenter"].Func["checked"][3].Num);
	
	// Text
	TextColors[3]   = Theme->Data["checkboxenter"].Func["textcolor"][0].Num/255.0f;
	TextColors[4]   = Theme->Data["checkboxenter"].Func["textcolor"][1].Num/255.0f;
	TextColors[5]   = Theme->Data["checkboxenter"].Func["textcolor"][2].Num/255.0f;
	//
	TextOffsets[2]  = floor(Theme->Data["checkboxenter"].Func["textoffset"][0].Num);
	TextOffsets[3]  = floor(Theme->Data["checkboxenter"].Func["textoffset"][1].Num);
	
	/*if (Theme->Data["checkboxenter"].Func["shadow"][0].Num == 1.0f) {
		// Shadow
		ShadowColors[3] = Theme->Data["checkboxenter"].Func["shadowcolor"][0].Num/255.0f;
		ShadowColors[4] = Theme->Data["checkboxenter"].Func["shadowcolor"][1].Num/255.0f;
		ShadowColors[5] = Theme->Data["checkboxenter"].Func["shadowcolor"][2].Num/255.0f;
		//
		ShadowOffsets[2]  = floor(Theme->Data["checkboxenter"].Func["shadowoffset"][0].Num);
		ShadowOffsets[3]  = floor(Theme->Data["checkboxenter"].Func["shadowoffset"][1].Num);
		//
		TextShadows[1] = true;
	}*/
	//---
	
	Width  = iWidths [0];
	Height = iHeights[0];
	//
	BoxElement = new BaseObject();
	BoxElement->Init();
	//
	BoxElement->Data    = GUIElementDat;
	BoxElement->Texture = Theme->Skin;
	BoxElement->mCamera = ScreenCamera;

	float bounds[6];
	string Text = wstring2string(Caption);

	ftglGetFontBBox(Theme->Font, Text.c_str(), Caption.size(), bounds);
	int twidth = bounds[3]-bounds[0];
	Width = iWidths[0]+twidth+TextOffsets[0]*3;
}

//----------------------------------------------------------------------------//
// UIEdit
//----------------------------------------------------------------------------//

void UIEdit::Init() {
	if (Inited) return;
	Inited = true;

	// Pre-computed
	
	// Leav //
	// Left
	OffsetsX[0] = Theme->Data["editleav"].Func["left"][0].Num / Theme->Skin->Width;
	OffsetsY[0] = Theme->Data["editleav"].Func["left"][1].Num / Theme->Skin->Height;
	Widths  [0] = Theme->Data["editleav"].Func["left"][2].Num / Theme->Skin->Width;
	Heights [0] = Theme->Data["editleav"].Func["left"][3].Num / Theme->Skin->Height;
	iWidths [0] = floor(Theme->Data["editleav"].Func["left"][2].Num);
	iHeights[0] = floor(Theme->Data["editleav"].Func["left"][3].Num);
	
	// Center
	OffsetsX[1] = Theme->Data["editleav"].Func["center"][0].Num / Theme->Skin->Width;
	OffsetsY[1] = Theme->Data["editleav"].Func["center"][1].Num / Theme->Skin->Height;
	Widths  [1] = Theme->Data["editleav"].Func["center"][2].Num / Theme->Skin->Width;
	Heights [1] = Theme->Data["editleav"].Func["center"][3].Num / Theme->Skin->Height;
	iWidths [1] = floor(Theme->Data["editleav"].Func["center"][2].Num);
	iHeights[1] = floor(Theme->Data["editleav"].Func["center"][3].Num);
	
	// Right
	OffsetsX[2] = Theme->Data["editleav"].Func["right"][0].Num / Theme->Skin->Width;
	OffsetsY[2] = Theme->Data["editleav"].Func["right"][1].Num / Theme->Skin->Height;
	Widths  [2] = Theme->Data["editleav"].Func["right"][2].Num / Theme->Skin->Width;
	Heights [2] = Theme->Data["editleav"].Func["right"][3].Num / Theme->Skin->Height;
	iWidths [2] = floor(Theme->Data["editleav"].Func["right"][2].Num);
	iHeights[2] = floor(Theme->Data["editleav"].Func["right"][3].Num);
	
	// Stick
	OffsetsX[12] = Theme->Data["editleav"].Func["stick"][0].Num / Theme->Skin->Width;
	OffsetsY[12] = Theme->Data["editleav"].Func["stick"][1].Num / Theme->Skin->Height;
	Widths  [12] = Theme->Data["editleav"].Func["stick"][2].Num / Theme->Skin->Width;
	Heights [12] = Theme->Data["editleav"].Func["stick"][3].Num / Theme->Skin->Height;
	iWidths [12] = floor(Theme->Data["editleav"].Func["stick"][2].Num);
	iHeights[12] = floor(Theme->Data["editleav"].Func["stick"][3].Num);
	
	// RectColor (RGBA)
	RectColor[0] = Theme->Data["editleav"].Func["selrectcolor"][0].Num/255.0f;
	RectColor[1] = Theme->Data["editleav"].Func["selrectcolor"][1].Num/255.0f;
	RectColor[2] = Theme->Data["editleav"].Func["selrectcolor"][2].Num/255.0f;
	RectColor[3] = Theme->Data["editleav"].Func["selrectcolor"][3].Num/100.0f;
	//
	RectOffset[0] = Theme->Data["editleav"].Func["selrectoffset"][0].Num;
	RectOffset[1] = Theme->Data["editleav"].Func["selrectoffset"][1].Num;
	
	// Text
	TextColors[0] = Theme->Data["editleav"].Func["textcolor"][0].Num/255.0f;
	TextColors[1] = Theme->Data["editleav"].Func["textcolor"][1].Num/255.0f;
	TextColors[2] = Theme->Data["editleav"].Func["textcolor"][2].Num/255.0f;
	//
	TextOffsets[0] = floor(Theme->Data["editleav"].Func["textoffset"][0].Num);
	TextOffsets[1] = floor(Theme->Data["editleav"].Func["textoffset"][1].Num);
	
	/*if (Theme->Data["editleav"].Func["shadow"][0].Num == 1.0f) {
		// Shadow
		ShadowColors[0] = Theme->Data["editleav"].Func["shadowcolor"][0].Num/255.0f;
		ShadowColors[1] = Theme->Data["editleav"].Func["shadowcolor"][1].Num/255.0f;
		ShadowColors[2] = Theme->Data["editleav"].Func["shadowcolor"][2].Num/255.0f;
		//
		ShadowOffsets[0]  = floor(Theme->Data["editleav"].Func["shadowoffset"][0].Num);
		ShadowOffsets[1]  = floor(Theme->Data["editleav"].Func["shadowoffset"][1].Num);
		//
		TextShadows[0] = true;
	}*/
	//---
	
	// Enter //
	// Left
	OffsetsX[3] = Theme->Data["editenter"].Func["left"][0].Num / Theme->Skin->Width;
	OffsetsY[3] = Theme->Data["editenter"].Func["left"][1].Num / Theme->Skin->Height;
	Widths  [3] = Theme->Data["editenter"].Func["left"][2].Num / Theme->Skin->Width;
	Heights [3] = Theme->Data["editenter"].Func["left"][3].Num / Theme->Skin->Height;
	iWidths [3] = floor(Theme->Data["editenter"].Func["left"][2].Num);
	iHeights[3] = floor(Theme->Data["editenter"].Func["left"][3].Num);
	
	// Center
	OffsetsX[4] = Theme->Data["editenter"].Func["center"][0].Num / Theme->Skin->Width;
	OffsetsY[4] = Theme->Data["editenter"].Func["center"][1].Num / Theme->Skin->Height;
	Widths  [4] = Theme->Data["editenter"].Func["center"][2].Num / Theme->Skin->Width;
	Heights [4] = Theme->Data["editenter"].Func["center"][3].Num / Theme->Skin->Height;
	iWidths [4] = floor(Theme->Data["editenter"].Func["center"][2].Num);
	iHeights[4] = floor(Theme->Data["editenter"].Func["center"][3].Num);
	
	// Right
	OffsetsX[5] = Theme->Data["editenter"].Func["right"][0].Num / Theme->Skin->Width;
	OffsetsY[5] = Theme->Data["editenter"].Func["right"][1].Num / Theme->Skin->Height;
	Widths  [5] = Theme->Data["editenter"].Func["right"][2].Num / Theme->Skin->Width;
	Heights [5] = Theme->Data["editenter"].Func["right"][3].Num / Theme->Skin->Height;
	iWidths [5] = floor(Theme->Data["editenter"].Func["right"][2].Num);
	iHeights[5] = floor(Theme->Data["editenter"].Func["right"][3].Num);
	
	// Text
	TextColors[3]   = Theme->Data["editenter"].Func["textcolor"][0].Num/255.0f;
	TextColors[4]   = Theme->Data["editenter"].Func["textcolor"][1].Num/255.0f;
	TextColors[5]   = Theme->Data["editenter"].Func["textcolor"][2].Num/255.0f;
	//
	TextOffsets[2]  = floor(Theme->Data["editenter"].Func["textoffset"][0].Num);
	TextOffsets[3]  = floor(Theme->Data["editenter"].Func["textoffset"][1].Num);
	
	/*if (Theme->Data["editenter"].Func["shadow"][0].Num == 1.0f) {
		// Shadow
		ShadowColors[3] = Theme->Data["editenter"].Func["shadowcolor"][0].Num/255.0f;
		ShadowColors[4] = Theme->Data["editenter"].Func["shadowcolor"][1].Num/255.0f;
		ShadowColors[5] = Theme->Data["editenter"].Func["shadowcolor"][2].Num/255.0f;
		//
		ShadowOffsets[2]  = floor(Theme->Data["editenter"].Func["shadowoffset"][0].Num);
		ShadowOffsets[3]  = floor(Theme->Data["editenter"].Func["shadowoffset"][1].Num);
		//
		TextShadows[1] = true;
	}*/
	//---
	
	// Click //
	// Left
	OffsetsX[6] = Theme->Data["editclick"].Func["left"][0].Num / Theme->Skin->Width;
	OffsetsY[6] = Theme->Data["editclick"].Func["left"][1].Num / Theme->Skin->Height;
	Widths  [6] = Theme->Data["editclick"].Func["left"][2].Num / Theme->Skin->Width;
	Heights [6] = Theme->Data["editclick"].Func["left"][3].Num / Theme->Skin->Height;
	iWidths [6] = floor(Theme->Data["editclick"].Func["left"][2].Num);
	iHeights[6] = floor(Theme->Data["editclick"].Func["left"][3].Num);
	
	// Center
	OffsetsX[7] = Theme->Data["editclick"].Func["center"][0].Num / Theme->Skin->Width;
	OffsetsY[7] = Theme->Data["editclick"].Func["center"][1].Num / Theme->Skin->Height;
	Widths  [7] = Theme->Data["editclick"].Func["center"][2].Num / Theme->Skin->Width;
	Heights [7] = Theme->Data["editclick"].Func["center"][3].Num / Theme->Skin->Height;
	iWidths [7] = floor(Theme->Data["editclick"].Func["center"][2].Num);
	iHeights[7] = floor(Theme->Data["editclick"].Func["center"][3].Num);
	
	// Right
	OffsetsX[8] = Theme->Data["editclick"].Func["right"][0].Num / Theme->Skin->Width;
	OffsetsY[8] = Theme->Data["editclick"].Func["right"][1].Num / Theme->Skin->Height;
	Widths  [8] = Theme->Data["editclick"].Func["right"][2].Num / Theme->Skin->Width;
	Heights [8] = Theme->Data["editclick"].Func["right"][3].Num / Theme->Skin->Height;
	iWidths [8] = floor(Theme->Data["editclick"].Func["right"][2].Num);
	iHeights[8] = floor(Theme->Data["editclick"].Func["right"][3].Num);
	
	// Text
	TextColors[6]   = Theme->Data["editclick"].Func["textcolor"][0].Num/255.0f;
	TextColors[7]   = Theme->Data["editclick"].Func["textcolor"][1].Num/255.0f;
	TextColors[8]   = Theme->Data["editclick"].Func["textcolor"][2].Num/255.0f;
	//
	TextOffsets[4]  = floor(Theme->Data["editclick"].Func["textoffset"][0].Num);
	TextOffsets[5]  = floor(Theme->Data["editclick"].Func["textoffset"][1].Num);
	
	/*if (Theme->Data["editclick"].Func["shadow"][0].Num == 1.0f) {
		// Shadow
		ShadowColors[6] = Theme->Data["editclick"].Func["shadowcolor"][0].Num/255.0f;
		ShadowColors[7] = Theme->Data["editclick"].Func["shadowcolor"][1].Num/255.0f;
		ShadowColors[8] = Theme->Data["editclick"].Func["shadowcolor"][2].Num/255.0f;
		//
		ShadowOffsets[4]  = floor(Theme->Data["editclick"].Func["shadowoffset"][0].Num);
		ShadowOffsets[5]  = floor(Theme->Data["editclick"].Func["shadowoffset"][1].Num);
		//
		TextShadows[2] = true;
	}*/
	//---
	
	// Focus //
	// Left
	OffsetsX[9]  = Theme->Data["editfocus"].Func["left"][0].Num / Theme->Skin->Width;
	OffsetsY[9]  = Theme->Data["editfocus"].Func["left"][1].Num / Theme->Skin->Height;
	Widths  [9]  = Theme->Data["editfocus"].Func["left"][2].Num / Theme->Skin->Width;
	Heights [9]  = Theme->Data["editfocus"].Func["left"][3].Num / Theme->Skin->Height;
	iWidths [9]  = floor(Theme->Data["editfocus"].Func["left"][2].Num);
	iHeights[9]  = floor(Theme->Data["editfocus"].Func["left"][3].Num);
	
	// Center
	OffsetsX[10] = Theme->Data["editfocus"].Func["center"][0].Num / Theme->Skin->Width;
	OffsetsY[10] = Theme->Data["editfocus"].Func["center"][1].Num / Theme->Skin->Height;
	Widths  [10] = Theme->Data["editfocus"].Func["center"][2].Num / Theme->Skin->Width;
	Heights [10] = Theme->Data["editfocus"].Func["center"][3].Num / Theme->Skin->Height;
	iWidths [10] = floor(Theme->Data["editfocus"].Func["center"][2].Num);
	iHeights[10] = floor(Theme->Data["editfocus"].Func["center"][3].Num);
	
	// Right
	OffsetsX[11] = Theme->Data["editfocus"].Func["right"][0].Num / Theme->Skin->Width;
	OffsetsY[11] = Theme->Data["editfocus"].Func["right"][1].Num / Theme->Skin->Height;
	Widths  [11] = Theme->Data["editfocus"].Func["right"][2].Num / Theme->Skin->Width;
	Heights [11] = Theme->Data["editfocus"].Func["right"][3].Num / Theme->Skin->Height;
	iWidths [11] = floor(Theme->Data["editfocus"].Func["right"][2].Num);
	iHeights[11] = floor(Theme->Data["editfocus"].Func["right"][3].Num);
	//---

	// Left Arrow Leav //
	OffsetsX[13] = Theme->Data["editleav"].Func["arrowleft"][0].Num / Theme->Skin->Width;
	OffsetsY[13] = Theme->Data["editleav"].Func["arrowleft"][1].Num / Theme->Skin->Height;
	Widths  [13] = Theme->Data["editleav"].Func["arrowleft"][2].Num / Theme->Skin->Width;
	Heights [13] = Theme->Data["editleav"].Func["arrowleft"][3].Num / Theme->Skin->Height;
	iWidths [13] = floor(Theme->Data["editleav"].Func["arrowleft"][2].Num);
	iHeights[13] = floor(Theme->Data["editleav"].Func["arrowleft"][3].Num);
	//---

	// Left Arrow Enter //
	OffsetsX[14] = Theme->Data["editenter"].Func["arrowleft"][0].Num / Theme->Skin->Width;
	OffsetsY[14] = Theme->Data["editenter"].Func["arrowleft"][1].Num / Theme->Skin->Height;
	Widths  [14] = Theme->Data["editenter"].Func["arrowleft"][2].Num / Theme->Skin->Width;
	Heights [14] = Theme->Data["editenter"].Func["arrowleft"][3].Num / Theme->Skin->Height;
	iWidths [14] = floor(Theme->Data["editenter"].Func["arrowleft"][2].Num);
	iHeights[14] = floor(Theme->Data["editenter"].Func["arrowleft"][3].Num);
	//---

	// Right Arrow Leav //
	OffsetsX[15] = Theme->Data["editleav"].Func["arrowright"][0].Num / Theme->Skin->Width;
	OffsetsY[15] = Theme->Data["editleav"].Func["arrowright"][1].Num / Theme->Skin->Height;
	Widths  [15] = Theme->Data["editleav"].Func["arrowright"][2].Num / Theme->Skin->Width;
	Heights [15] = Theme->Data["editleav"].Func["arrowright"][3].Num / Theme->Skin->Height;
	iWidths [15] = floor(Theme->Data["editleav"].Func["arrowright"][2].Num);
	iHeights[15] = floor(Theme->Data["editleav"].Func["arrowright"][3].Num);
	//---

	// Right Arrow Enter //
	OffsetsX[16] = Theme->Data["editenter"].Func["arrowright"][0].Num / Theme->Skin->Width;
	OffsetsY[16] = Theme->Data["editenter"].Func["arrowright"][1].Num / Theme->Skin->Height;
	Widths  [16] = Theme->Data["editenter"].Func["arrowright"][2].Num / Theme->Skin->Width;
	Heights [16] = Theme->Data["editenter"].Func["arrowright"][3].Num / Theme->Skin->Height;
	iWidths [16] = floor(Theme->Data["editenter"].Func["arrowright"][2].Num);
	iHeights[16] = floor(Theme->Data["editenter"].Func["arrowright"][3].Num);
	//---
	
	// Stick pos
	sy = Top+((iHeights[1]-iHeights[12]) >> 1);
	//sx = iWidths[9];
	
	// Text pos
	//ty = ScreenHeight-Top-(iHeights[1] >> 1)-4;
	//tx = Left+iWidths[12];
	
	tx = Left+iWidths[12];
	toffset = 0;
	if (DrawAlign == alCenter | DrawAlign == alLeft) tx += iWidths[12];
	ty = WindowHeight-Top-(iHeights[1] >> 1)-4;
	sy = Top+((iHeights[1]-iHeights[12]) >> 1);
	//
	//
	LeftElement = new BaseObject();
	LeftElement->Init();
	//
	LeftElement->Data    = GUIElementDat;
	LeftElement->mCamera = ScreenCamera;
	LeftElement->Shader  = Shader;

	//
	RightElement = new BaseObject();
	RightElement->Init();
	//
	RightElement->Data    = GUIElementDat;
	RightElement->mCamera = ScreenCamera;
	RightElement->Shader  = Shader;

	//
	MiddleElement = new BaseObject();
	MiddleElement->Init();
	//
	MiddleElement->Data    = GUIElementDat;
	MiddleElement->mCamera = ScreenCamera;
	MiddleElement->Shader  = Shader;
	//
	SelectRectElement = new BaseObject();
	SelectRectElement->Init();
	//
	SelectRectElement->Data    = GUIElementDat;
	SelectRectElement->mCamera = ScreenCamera;
	SelectRectElement->Shader  = Shader;
	//
	StickElement = new BaseObject();
	StickElement->Init();
	//
	StickElement->Data    = GUIElementDat;
	StickElement->mCamera = ScreenCamera;
	StickElement->Shader  = Shader;
	//
	LeftArrow = new BaseObject();
	LeftArrow->Init();
	//
	LeftArrow->Data    = GUIElementDat;
	LeftArrow->mCamera = ScreenCamera;
	LeftArrow->Shader  = Shader;
	//
	RightArrow = new BaseObject();
	RightArrow->Init();
	//
	RightArrow->Data    = GUIElementDat;
	RightArrow->mCamera = ScreenCamera;
	RightArrow->Shader  = Shader;
}

void UIListMenu::Init() {
	for (int i = 0; i < Items.size(); i++) {
		//
		if (Items[i] == 0) continue;
		Items[i]->TextAlign = alLeft;
		//
		Items[i]->Shader  = Shader;
		Items[i]->Theme   = Theme;
		Items[i]->Manager = Manager;
		//
		Items[i]->Init();
		Height += Items[i]->Height;
	}
	
	if (Inited) return;
	Inited = true;

	//
	// Pre computed
	
	//* Top *//
	// Left
	OffsetsX[0] = Theme->Data["listmenutop"].Func["left"][0].Num / Theme->Skin->Width;
	OffsetsY[0] = Theme->Data["listmenutop"].Func["left"][1].Num / Theme->Skin->Height;
	Widths  [0] = Theme->Data["listmenutop"].Func["left"][2].Num / Theme->Skin->Width;
	Heights [0] = Theme->Data["listmenutop"].Func["left"][3].Num / Theme->Skin->Height;
	iWidths [0] = floor(Theme->Data["listmenutop"].Func["left"][2].Num);
	iHeights[0] = floor(Theme->Data["listmenutop"].Func["left"][3].Num);
	
	// Center
	OffsetsX[1] = Theme->Data["listmenutop"].Func["center"][0].Num / Theme->Skin->Width;
	OffsetsY[1] = Theme->Data["listmenutop"].Func["center"][1].Num / Theme->Skin->Height;
	Widths  [1] = Theme->Data["listmenutop"].Func["center"][2].Num / Theme->Skin->Width;
	Heights [1] = Theme->Data["listmenutop"].Func["center"][3].Num / Theme->Skin->Height;
	iWidths [1] = floor(Theme->Data["listmenutop"].Func["center"][2].Num);
	iHeights[1] = floor(Theme->Data["listmenutop"].Func["center"][3].Num);
	
	// Right
	OffsetsX[2] = Theme->Data["listmenutop"].Func["right"][0].Num / Theme->Skin->Width;
	OffsetsY[2] = Theme->Data["listmenutop"].Func["right"][1].Num / Theme->Skin->Height;
	Widths  [2] = Theme->Data["listmenutop"].Func["right"][2].Num / Theme->Skin->Width;
	Heights [2] = Theme->Data["listmenutop"].Func["right"][3].Num / Theme->Skin->Height;
	iWidths [2] = floor(Theme->Data["listmenutop"].Func["right"][2].Num);
	iHeights[2] = floor(Theme->Data["listmenutop"].Func["right"][3].Num);
	// ---
	
	//* Center *///
	// Left
	OffsetsX[3] = Theme->Data["listmenucenter"].Func["left"][0].Num / Theme->Skin->Width;
	OffsetsY[3] = Theme->Data["listmenucenter"].Func["left"][1].Num / Theme->Skin->Height;
	Widths  [3] = Theme->Data["listmenucenter"].Func["left"][2].Num / Theme->Skin->Width;
	Heights [3] = Theme->Data["listmenucenter"].Func["left"][3].Num / Theme->Skin->Height;
	iWidths [3] = floor(Theme->Data["listmenucenter"].Func["left"][2].Num);
	iHeights[3] = floor(Theme->Data["listmenucenter"].Func["left"][3].Num);
	
	// Center
	OffsetsX[4] = Theme->Data["listmenucenter"].Func["center"][0].Num / Theme->Skin->Width;
	OffsetsY[4] = Theme->Data["listmenucenter"].Func["center"][1].Num / Theme->Skin->Height;
	Widths  [4] = Theme->Data["listmenucenter"].Func["center"][2].Num / Theme->Skin->Width;
	Heights [4] = Theme->Data["listmenucenter"].Func["center"][3].Num / Theme->Skin->Height;
	iWidths [4] = floor(Theme->Data["listmenucenter"].Func["center"][2].Num);
	iHeights[4] = floor(Theme->Data["listmenucenter"].Func["center"][3].Num);
	
	// Right
	OffsetsX[5] = Theme->Data["listmenucenter"].Func["right"][0].Num / Theme->Skin->Width;
	OffsetsY[5] = Theme->Data["listmenucenter"].Func["right"][1].Num / Theme->Skin->Height;
	Widths  [5] = Theme->Data["listmenucenter"].Func["right"][2].Num / Theme->Skin->Width;
	Heights [5] = Theme->Data["listmenucenter"].Func["right"][3].Num / Theme->Skin->Height;
	iWidths [5] = floor(Theme->Data["listmenucenter"].Func["right"][2].Num);
	iHeights[5] = floor(Theme->Data["listmenucenter"].Func["right"][3].Num);
	// ---
	
	//* Bottom *///
	// Left
	OffsetsX[6] = Theme->Data["listmenubottom"].Func["left"][0].Num / Theme->Skin->Width;
	OffsetsY[6] = Theme->Data["listmenubottom"].Func["left"][1].Num / Theme->Skin->Height;
	Widths  [6] = Theme->Data["listmenubottom"].Func["left"][2].Num / Theme->Skin->Width;
	Heights [6] = Theme->Data["listmenubottom"].Func["left"][3].Num / Theme->Skin->Height;
	iWidths [6] = floor(Theme->Data["listmenubottom"].Func["left"][2].Num);
	iHeights[6] = floor(Theme->Data["listmenubottom"].Func["left"][3].Num);
	
	// Center
	OffsetsX[7] = Theme->Data["listmenubottom"].Func["center"][0].Num / Theme->Skin->Width;
	OffsetsY[7] = Theme->Data["listmenubottom"].Func["center"][1].Num / Theme->Skin->Height;
	Widths  [7] = Theme->Data["listmenubottom"].Func["center"][2].Num / Theme->Skin->Width;
	Heights [7] = Theme->Data["listmenubottom"].Func["center"][3].Num / Theme->Skin->Height;
	iWidths [7] = floor(Theme->Data["listmenubottom"].Func["center"][2].Num);
	iHeights[7] = floor(Theme->Data["listmenubottom"].Func["center"][3].Num);
	
	// Right
	OffsetsX[8] = Theme->Data["listmenubottom"].Func["right"][0].Num / Theme->Skin->Width;
	OffsetsY[8] = Theme->Data["listmenubottom"].Func["right"][1].Num / Theme->Skin->Height;
	Widths  [8] = Theme->Data["listmenubottom"].Func["right"][2].Num / Theme->Skin->Width;
	Heights [8] = Theme->Data["listmenubottom"].Func["right"][3].Num / Theme->Skin->Height;
	iWidths [8] = floor(Theme->Data["listmenubottom"].Func["right"][2].Num);
	iHeights[8] = floor(Theme->Data["listmenubottom"].Func["right"][3].Num);
	// ---
	
	//* Mark Leav *///
	OffsetsX[9] = Theme->Data["listitemleav"].Func["mark"][0].Num / Theme->Skin->Width;
	OffsetsY[9] = Theme->Data["listitemleav"].Func["mark"][1].Num / Theme->Skin->Height;
	Widths  [9] = Theme->Data["listitemleav"].Func["mark"][2].Num / Theme->Skin->Width;
	Heights [9] = Theme->Data["listitemleav"].Func["mark"][3].Num / Theme->Skin->Height;
	iWidths [9] = floor(Theme->Data["listitemleav"].Func["mark"][2].Num);
	iHeights[9] = floor(Theme->Data["listitemleav"].Func["mark"][3].Num);
	// ---
	
	//* Mark Leav *///
	OffsetsX[9] = Theme->Data["listitemleav"].Func["mark"][0].Num / Theme->Skin->Width;
	OffsetsY[9] = Theme->Data["listitemleav"].Func["mark"][1].Num / Theme->Skin->Height;
	Widths  [9] = Theme->Data["listitemleav"].Func["mark"][2].Num / Theme->Skin->Width;
	Heights [9] = Theme->Data["listitemleav"].Func["mark"][3].Num / Theme->Skin->Height;
	iWidths [9] = floor(Theme->Data["listitemleav"].Func["mark"][2].Num);
	iHeights[9] = floor(Theme->Data["listitemleav"].Func["mark"][3].Num);
	// ---
	
	//* Mark Enter *///
	OffsetsX[10] = Theme->Data["listitementer"].Func["mark"][0].Num / Theme->Skin->Width;
	OffsetsY[10] = Theme->Data["listitementer"].Func["mark"][1].Num / Theme->Skin->Height;
	Widths  [10] = Theme->Data["listitementer"].Func["mark"][2].Num / Theme->Skin->Width;
	Heights [10] = Theme->Data["listitementer"].Func["mark"][3].Num / Theme->Skin->Height;
	iWidths [10] = floor(Theme->Data["listitementer"].Func["mark"][2].Num);
	iHeights[10] = floor(Theme->Data["listitementer"].Func["mark"][3].Num);

	//* Split *///
	OffsetsX[11] = Theme->Data["general"].Func["menusplit"][0].Num / Theme->Skin->Width;
	OffsetsY[11] = Theme->Data["general"].Func["menusplit"][1].Num / Theme->Skin->Height;
	Widths  [11] = Theme->Data["general"].Func["menusplit"][2].Num / Theme->Skin->Width;
	Heights [11] = Theme->Data["general"].Func["menusplit"][3].Num / Theme->Skin->Height;
	iWidths [11] = floor(Theme->Data["general"].Func["menusplit"][2].Num);
	iHeights[11] = floor(Theme->Data["general"].Func["menusplit"][3].Num);
	// ---
	
	Height = 0;
	//
	for (int i = 0; i < 11; i++) {
		//
		DrawElements[i] = new BaseObject();
		DrawElements[i]->Init();
		//
		DrawElements[i]->Data    = GUIElementDat;
		DrawElements[i]->mCamera = ScreenCamera;
		DrawElements[i]->Shader	 = Shader;
	}
	//
}

// UITabs
void UITabs::Init() {
	if (Inited) return;
	Inited = true;

	//* Background *///
	OffsetsX[0] = Theme->Data["tabs"].Func["background"][0].Num / Theme->Skin->Width;
	OffsetsY[0] = Theme->Data["tabs"].Func["background"][1].Num / Theme->Skin->Height;
	Widths  [0] = Theme->Data["tabs"].Func["background"][2].Num / Theme->Skin->Width;
	Heights [0] = Theme->Data["tabs"].Func["background"][3].Num / Theme->Skin->Height;
	iWidths [0] = floor(Theme->Data["tabs"].Func["background"][2].Num);
	iHeights[0] = floor(Theme->Data["tabs"].Func["background"][3].Num);
	// ---

	//* Split *///
	OffsetsX[1] = Theme->Data["tabs"].Func["split"][0].Num / Theme->Skin->Width;
	OffsetsY[1] = Theme->Data["tabs"].Func["split"][1].Num / Theme->Skin->Height;
	Widths  [1] = Theme->Data["tabs"].Func["split"][2].Num / Theme->Skin->Width;
	Heights [1] = Theme->Data["tabs"].Func["split"][3].Num / Theme->Skin->Height;
	iWidths [1] = floor(Theme->Data["tabs"].Func["split"][2].Num);
	iHeights[1] = floor(Theme->Data["tabs"].Func["split"][3].Num);
	// ---

	shHeight  = floor(Theme->Data["tabs"].Func["shadowheight"][0].Num);
	TabOffset = floor(Theme->Data["tabs"].Func["taboffset"][0].Num);

	Height = iHeights[0]-1-shHeight;

	Panel = new BaseObject();
	Panel->Init();
	//
	Panel->Data    = GUIElementDat;
	Panel->mCamera = ScreenCamera;
	Panel->Shader  = ColorShader;

	Split = new BaseObject();
	Split->Init();
	//
	Split->Data    = GUIElementDat;
	Split->mCamera = ScreenCamera;
	Split->Shader  = ColorShader;

	for (int i = 0; i < Tabs.size(); i++) {
		if (Tabs[i] == 0)
			continue;

		Tabs[i]->Shader = Shader;
		Tabs[i]->Theme  = Theme;
		Tabs[i]->Manager = Manager;
		Tabs[i]->Init();

		for (int j = 0; j < Tabs[i]->Elements.size(); j++) {
			Tabs[i]->Elements[j]->Shader = Shader;
			Tabs[i]->Elements[j]->Theme  = Theme;
			Tabs[i]->Elements[j]->Manager = Manager;
			Tabs[i]->Elements[j]->Init();			
		}
	}
}

// UIPanel
void UIPanel::Init() {
	if (Inited) return;
	Inited = true;
	//
	for (int i = 0; i < Elements.size(); i++) {
		if (Elements[i] == 0) continue;
		Elements[i]->Shader = Shader;
		Elements[i]->Theme  = Theme;
		Elements[i]->Manager = Manager;
		Elements[i]->Init();
	}
	//
	QuadElement = new BaseObject();
	QuadElement->Init();
	//
	QuadElement->Data    = GUIElementDat;
	QuadElement->mCamera = ScreenCamera;
	QuadElement->Shader  = ColorShader;
	//
	for (int i = 0; i < 6; i++) {
		//
		ScrollBg[i] = new BaseObject();
		ScrollBg[i]->Init();
		//
		ScrollBg[i]->Data    = GUIElementDat;
		ScrollBg[i]->mCamera = ScreenCamera;
		ScrollBg[i]->Shader  = Shader;
	}
	//
	Arrow = new BaseObject();
	Arrow->Init();
	//
	Arrow->Data    = GUIElementDat;
	Arrow->mCamera = ScreenCamera;
	Arrow->Shader  = Shader;
	//
	ScrollBg[3]->SetRotate(pi/2);
	ScrollBg[4]->SetRotate(pi/2);
	ScrollBg[5]->SetRotate(pi/2);
	//
	for (int i = 0; i < 6; i++) {
		//
		ScrollBtn[i] = new BaseObject();
		ScrollBtn[i]->Init();
		//
		ScrollBtn[i]->Data    = GUIElementDat;
		ScrollBtn[i]->mCamera = ScreenCamera;
		ScrollBtn[i]->Shader  = Shader;
	}
	//
	// Text
	TextColor[0]   = Theme->Data["panel"].Func["textcolor"][0].Num/255.0f;
	TextColor[1]   = Theme->Data["panel"].Func["textcolor"][1].Num/255.0f;
	TextColor[2]   = Theme->Data["panel"].Func["textcolor"][2].Num/255.0f;
	TextColor[0] = 0.0f;
	TextColor[1] = 0.0f;
	TextColor[2] = 0.0f;
	//
	TextOffset[0]  = floor(Theme->Data["panel"].Func["textoffset"][0].Num);
	TextOffset[1]  = floor(Theme->Data["panel"].Func["textoffset"][1].Num);
	//
	TextShadow = false;
	
	/*if (Theme->Data["panel"].Func["shadow"][0].Num == 1.0f) {
		// Shadow
		ShadowColor[0] = Theme->Data["panel"].Func["shadowcolor"][0].Num/255.0f;
		ShadowColor[1] = Theme->Data["panel"].Func["shadowcolor"][1].Num/255.0f;
		ShadowColor[2] = Theme->Data["panel"].Func["shadowcolor"][2].Num/255.0f;
		//
		ShadowOffset[0]  = floor(Theme->Data["panel"].Func["shadowoffset"][0].Num);
		ShadowOffset[1]  = floor(Theme->Data["panel"].Func["shadowoffset"][1].Num);
		//
		TextShadow = true;
	}*/
	//
	TextAlign = alLeft;
	//
	Split = new BaseObject();
	Split->Init();
	//
	Split->Data    = GUIElementDat;
	Split->mCamera = ScreenCamera;
	Split->Shader  = Shader;
	//
	ScrollBtn[3]->SetRotate(pi/2);
	ScrollBtn[4]->SetRotate(pi/2);
	ScrollBtn[5]->SetRotate(pi/2);
	//
	Colors[0] = Theme->Data["panel"].Func["color"][0].Num / 255;
	Colors[1] = Theme->Data["panel"].Func["color"][1].Num / 255;
	Colors[2] = Theme->Data["panel"].Func["color"][2].Num / 255;
	Colors[3] = Theme->Data["panel"].Func["color"][3].Num;
	//
	ColorsIn[0] = Theme->Data["panel"].Func["colorin"][0].Num / 255;
	ColorsIn[1] = Theme->Data["panel"].Func["colorin"][1].Num / 255;
	ColorsIn[2] = Theme->Data["panel"].Func["colorin"][2].Num / 255;
	ColorsIn[3] = Theme->Data["panel"].Func["colorin"][3].Num;
	//
	ActionColor[0] = Theme->Data["panel"].Func["actioncolor"][0].Num / 255;
	ActionColor[1] = Theme->Data["panel"].Func["actioncolor"][1].Num / 255;
	ActionColor[2] = Theme->Data["panel"].Func["actioncolor"][2].Num / 255;
	ActionColor[3] = Theme->Data["panel"].Func["actioncolor"][3].Num;
	//
	// ScrollHorizontal //
	// Left
	OffsetsX[0] = Theme->Data["scrollhorizontal"].Func["left"][0].Num / Theme->Skin->Width;
	OffsetsY[0] = Theme->Data["scrollhorizontal"].Func["left"][1].Num / Theme->Skin->Height;
	Widths  [0] = Theme->Data["scrollhorizontal"].Func["left"][2].Num / Theme->Skin->Width;
	Heights [0] = Theme->Data["scrollhorizontal"].Func["left"][3].Num / Theme->Skin->Height;
	iWidths [0] = floor(Theme->Data["scrollhorizontal"].Func["left"][2].Num);
	iHeights[0] = floor(Theme->Data["scrollhorizontal"].Func["left"][3].Num);
	
	// Center
	OffsetsX[1] = Theme->Data["scrollhorizontal"].Func["center"][0].Num / Theme->Skin->Width;
	OffsetsY[1] = Theme->Data["scrollhorizontal"].Func["center"][1].Num / Theme->Skin->Height;
	Widths  [1] = Theme->Data["scrollhorizontal"].Func["center"][2].Num / Theme->Skin->Width;
	Heights [1] = Theme->Data["scrollhorizontal"].Func["center"][3].Num / Theme->Skin->Height;
	iWidths [1] = floor(Theme->Data["scrollhorizontal"].Func["center"][2].Num);
	iHeights[1] = floor(Theme->Data["scrollhorizontal"].Func["center"][3].Num);
	
	// Right
	OffsetsX[2] = Theme->Data["scrollhorizontal"].Func["right"][0].Num / Theme->Skin->Width;
	OffsetsY[2] = Theme->Data["scrollhorizontal"].Func["right"][1].Num / Theme->Skin->Height;
	Widths  [2] = Theme->Data["scrollhorizontal"].Func["right"][2].Num / Theme->Skin->Width;
	Heights [2] = Theme->Data["scrollhorizontal"].Func["right"][3].Num / Theme->Skin->Height;
	iWidths [2] = floor(Theme->Data["scrollhorizontal"].Func["right"][2].Num);
	iHeights[2] = floor(Theme->Data["scrollhorizontal"].Func["right"][3].Num);

	// ScrollVertical //
	// Left
	OffsetsX[3] = Theme->Data["scrollvertical"].Func["left"][0].Num / Theme->Skin->Width;
	OffsetsY[3] = Theme->Data["scrollvertical"].Func["left"][1].Num / Theme->Skin->Height;
	Widths  [3] = Theme->Data["scrollvertical"].Func["left"][2].Num / Theme->Skin->Width;
	Heights [3] = Theme->Data["scrollvertical"].Func["left"][3].Num / Theme->Skin->Height;
	iWidths [3] = floor(Theme->Data["scrollvertical"].Func["left"][2].Num);
	iHeights[3] = floor(Theme->Data["scrollvertical"].Func["left"][3].Num);
	
	// Center
	OffsetsX[4] = Theme->Data["scrollvertical"].Func["center"][0].Num / Theme->Skin->Width;
	OffsetsY[4] = Theme->Data["scrollvertical"].Func["center"][1].Num / Theme->Skin->Height;
	Widths  [4] = Theme->Data["scrollvertical"].Func["center"][2].Num / Theme->Skin->Width;
	Heights [4] = Theme->Data["scrollvertical"].Func["center"][3].Num / Theme->Skin->Height;
	iWidths [4] = floor(Theme->Data["scrollvertical"].Func["center"][2].Num);
	iHeights[4] = floor(Theme->Data["scrollvertical"].Func["center"][3].Num);
	
	// Right
	OffsetsX[5] = Theme->Data["scrollvertical"].Func["right"][0].Num / Theme->Skin->Width;
	OffsetsY[5] = Theme->Data["scrollvertical"].Func["right"][1].Num / Theme->Skin->Height;
	Widths  [5] = Theme->Data["scrollvertical"].Func["right"][2].Num / Theme->Skin->Width;
	Heights [5] = Theme->Data["scrollvertical"].Func["right"][3].Num / Theme->Skin->Height;
	iWidths [5] = floor(Theme->Data["scrollvertical"].Func["right"][2].Num);
	iHeights[5] = floor(Theme->Data["scrollvertical"].Func["right"][3].Num);

	// Scroll Button
	// -------------

	// Leave
	// Left
	OffsetsX[6] = Theme->Data["scroolbuttonleav"].Func["left"][0].Num / Theme->Skin->Width;
	OffsetsY[6] = Theme->Data["scroolbuttonleav"].Func["left"][1].Num / Theme->Skin->Height;
	Widths  [6] = Theme->Data["scroolbuttonleav"].Func["left"][2].Num / Theme->Skin->Width;
	Heights [6] = Theme->Data["scroolbuttonleav"].Func["left"][3].Num / Theme->Skin->Height;
	iWidths [6] = floor(Theme->Data["scroolbuttonleav"].Func["left"][2].Num);
	iHeights[6] = floor(Theme->Data["scroolbuttonleav"].Func["left"][3].Num);
	
	// Center
	OffsetsX[7] = Theme->Data["scroolbuttonleav"].Func["center"][0].Num / Theme->Skin->Width;
	OffsetsY[7] = Theme->Data["scroolbuttonleav"].Func["center"][1].Num / Theme->Skin->Height;
	Widths  [7] = Theme->Data["scroolbuttonleav"].Func["center"][2].Num / Theme->Skin->Width;
	Heights [7] = Theme->Data["scroolbuttonleav"].Func["center"][3].Num / Theme->Skin->Height;
	iWidths [7] = floor(Theme->Data["scroolbuttonleav"].Func["center"][2].Num);
	iHeights[7] = floor(Theme->Data["scroolbuttonleav"].Func["center"][3].Num);
	
	// Right
	OffsetsX[8] = Theme->Data["scroolbuttonleav"].Func["right"][0].Num / Theme->Skin->Width;
	OffsetsY[8] = Theme->Data["scroolbuttonleav"].Func["right"][1].Num / Theme->Skin->Height;
	Widths  [8] = Theme->Data["scroolbuttonleav"].Func["right"][2].Num / Theme->Skin->Width;
	Heights [8] = Theme->Data["scroolbuttonleav"].Func["right"][3].Num / Theme->Skin->Height;
	iWidths [8] = floor(Theme->Data["scroolbuttonleav"].Func["right"][2].Num);
	iHeights[8] = floor(Theme->Data["scroolbuttonleav"].Func["right"][3].Num);

	// Enter
	// Left
	OffsetsX[9] = Theme->Data["scroolbuttonenter"].Func["left"][0].Num / Theme->Skin->Width;
	OffsetsY[9] = Theme->Data["scroolbuttonenter"].Func["left"][1].Num / Theme->Skin->Height;
	Widths  [9] = Theme->Data["scroolbuttonenter"].Func["left"][2].Num / Theme->Skin->Width;
	Heights [9] = Theme->Data["scroolbuttonenter"].Func["left"][3].Num / Theme->Skin->Height;
	iWidths [9] = floor(Theme->Data["scroolbuttonenter"].Func["left"][2].Num);
	iHeights[9] = floor(Theme->Data["scroolbuttonenter"].Func["left"][3].Num);
	
	// Center
	OffsetsX[10] = Theme->Data["scroolbuttonenter"].Func["center"][0].Num / Theme->Skin->Width;
	OffsetsY[10] = Theme->Data["scroolbuttonenter"].Func["center"][1].Num / Theme->Skin->Height;
	Widths  [10] = Theme->Data["scroolbuttonenter"].Func["center"][2].Num / Theme->Skin->Width;
	Heights [10] = Theme->Data["scroolbuttonenter"].Func["center"][3].Num / Theme->Skin->Height;
	iWidths [10] = floor(Theme->Data["scroolbuttonenter"].Func["center"][2].Num);
	iHeights[10] = floor(Theme->Data["scroolbuttonenter"].Func["center"][3].Num);
	
	// Right
	OffsetsX[11] = Theme->Data["scroolbuttonenter"].Func["right"][0].Num / Theme->Skin->Width;
	OffsetsY[11] = Theme->Data["scroolbuttonenter"].Func["right"][1].Num / Theme->Skin->Height;
	Widths  [11] = Theme->Data["scroolbuttonenter"].Func["right"][2].Num / Theme->Skin->Width;
	Heights [11] = Theme->Data["scroolbuttonenter"].Func["right"][3].Num / Theme->Skin->Height;
	iWidths [11] = floor(Theme->Data["scroolbuttonenter"].Func["right"][2].Num);
	iHeights[11] = floor(Theme->Data["scroolbuttonenter"].Func["right"][3].Num);
	//
	// Split
	OffsetsX[12] = Theme->Data["panel"].Func["split"][0].Num / Theme->Skin->Width;
	OffsetsY[12] = Theme->Data["panel"].Func["split"][1].Num / Theme->Skin->Height;
	Widths  [12] = Theme->Data["panel"].Func["split"][2].Num / Theme->Skin->Width;
	Heights [12] = Theme->Data["panel"].Func["split"][3].Num / Theme->Skin->Height;
	iWidths [12] = floor(Theme->Data["panel"].Func["split"][2].Num);
	iHeights[12] = floor(Theme->Data["panel"].Func["split"][3].Num);
	//
	// SplitIn
	OffsetsX[13] = Theme->Data["panel"].Func["splitin"][0].Num / Theme->Skin->Width;
	OffsetsY[13] = Theme->Data["panel"].Func["splitin"][1].Num / Theme->Skin->Height;
	Widths  [13] = Theme->Data["panel"].Func["splitin"][2].Num / Theme->Skin->Width;
	Heights [13] = Theme->Data["panel"].Func["splitin"][3].Num / Theme->Skin->Height;
	iWidths [13] = floor(Theme->Data["panel"].Func["splitin"][2].Num);
	iHeights[13] = floor(Theme->Data["panel"].Func["splitin"][3].Num);
	//
	// ArrowOpen
	OffsetsX[14] = Theme->Data["panel"].Func["arrowopen"][0].Num / Theme->Skin->Width;
	OffsetsY[14] = Theme->Data["panel"].Func["arrowopen"][1].Num / Theme->Skin->Height;
	Widths  [14] = Theme->Data["panel"].Func["arrowopen"][2].Num / Theme->Skin->Width;
	Heights [14] = Theme->Data["panel"].Func["arrowopen"][3].Num / Theme->Skin->Height;
	iWidths [14] = floor(Theme->Data["panel"].Func["arrowopen"][2].Num);
	iHeights[14] = floor(Theme->Data["panel"].Func["arrowopen"][3].Num);
	//
	// ArrowClose
	OffsetsX[15] = Theme->Data["panel"].Func["arrowclose"][0].Num / Theme->Skin->Width;
	OffsetsY[15] = Theme->Data["panel"].Func["arrowclose"][1].Num / Theme->Skin->Height;
	Widths  [15] = Theme->Data["panel"].Func["arrowclose"][2].Num / Theme->Skin->Width;
	Heights [15] = Theme->Data["panel"].Func["arrowclose"][3].Num / Theme->Skin->Height;
	iWidths [15] = floor(Theme->Data["panel"].Func["arrowclose"][2].Num);
	iHeights[15] = floor(Theme->Data["panel"].Func["arrowclose"][3].Num);
	//
	hbMin = iWidths[6]+iWidths[8];
	hbMax = Width-iHeights[4];
	//
	vbMin = iWidths[6]+iWidths[8];
	vbMax = Height-iHeights[4];

	hbSize = hbMax;
	vbSize = vbMax; mShift = mouseNone;
	hbOffset = 0; hsOffset = 0;
	vbOffset = 0; vsOffset = 0;
	SHClick = false;
	VHClick = false;
	SplitClick = false;
}

//
// UIDialog
void UIDialog::Init() {
	UIPanel::Init();
	//
	for (int i = 0; i < 9; i++) {
		FormElements[i] = new BaseObject();
		FormElements[i]->Init();
		//
		FormElements[i]->Data    = GUIElementDat;
		FormElements[i]->mCamera = ScreenCamera;
		FormElements[i]->Shader  = Shader;
	}
	//
	string params[9];
	params[0] = "topleft";    params[1] = "topcenter";    params[2] = "topright";
	params[3] = "centerleft"; params[4] = "centercenter"; params[5] = "centerright";
	params[6] = "bottomleft"; params[7] = "bottomcenter"; params[8] = "bottomright";
	//
	for (int i = 0; i < 9; i++) {
		OffsetsX[i] = Theme->Data["dialog"].Func[params[i]][0].Num / Theme->Skin->Width;
		OffsetsY[i] = Theme->Data["dialog"].Func[params[i]][1].Num / Theme->Skin->Height;
		Widths  [i] = Theme->Data["dialog"].Func[params[i]][2].Num / Theme->Skin->Width;
		Heights [i] = Theme->Data["dialog"].Func[params[i]][3].Num / Theme->Skin->Height;
		iWidths [i] = floor(Theme->Data["dialog"].Func[params[i]][2].Num);
		iHeights[i] = floor(Theme->Data["dialog"].Func[params[i]][3].Num);
	}

	// Text
	// Text
	TextColor[0]   = Theme->Data["dialog"].Func["textcolor"][0].Num / 255.0f;
	TextColor[1]   = Theme->Data["dialog"].Func["textcolor"][1].Num / 255.0f;
	TextColor[2]   = Theme->Data["dialog"].Func["textcolor"][2].Num / 255.0f;
	//TextColor[0] = 0.0f;
	//TextColor[1] = 0.0f;
	//TextColor[2] = 0.0f;
	//
	TextOffset[0]  = floor(Theme->Data["dialog"].Func["textoffset"][0].Num);
	TextOffset[1]  = floor(Theme->Data["dialog"].Func["textoffset"][1].Num);
	//
	TextShadow = false;
	
	/*if (Theme->Data["dialog"].Func["shadow"][0].Num == 1.0f) {
		// Shadow
		ShadowColor[0] = Theme->Data["dialog"].Func["shadowcolor"][0].Num / 255.0f;
		ShadowColor[1] = Theme->Data["dialog"].Func["shadowcolor"][1].Num / 255.0f;
		ShadowColor[2] = Theme->Data["dialog"].Func["shadowcolor"][2].Num / 255.0f;
		//
		ShadowOffset[0]  = floor(Theme->Data["dialog"].Func["shadowoffset"][0].Num);
		ShadowOffset[1]  = floor(Theme->Data["dialog"].Func["shadowoffset"][1].Num);
		//
		TextShadow = true;
	}*/
	//
	TextAlign = alLeft;
}

#endif
