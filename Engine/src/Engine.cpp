//* Author : Kabilin Andrey, 2013 *//

#ifndef CPP_ENGINE
#define CPP_ENGINE

//
Engine::Engine() {
	//
	//
	Theme 				= new UITheme();
	ScreenCamera		= new Camera2D();
	GUIManager			= new UIManager();
	TexAtlasShader		= new BaseShader();
	TexAtlasAlphaShader	= new BaseShader();
	ColorShader			= new BaseShader();
	SpriteDat			= new SpriteData();
	GUIElementDat		= new GUIElementData();
	ColorLineShader		= new BaseShader();
	ColorPickerShader	= new BaseShader();
	//
	//ToolTabs = (UITabs*) MapEditor->Data->Elements["tooltabs"];
	//
	/*MapEditor->Colors[0] = 51/255.0f;
	MapEditor->Colors[1] = 51/255.0f;
	MapEditor->Colors[2] = 51/255.0f;*/

	//UIThread = sf::Thread(&UIManager::Draw, &GUIManager);
	//

}

void Engine::Init() {
	//
	InitCursor();
	Textures::Init();
	//
	Textures::SplashScreen->LoadFromFile ("Data/splashscreen.png");
	Textures::PreviewFloat->LoadFromFile ("Data/preview.png");
	Textures::TopToolIcons->LoadFromFile ("Data/TopPanelIcons.png");
	Textures::PanelTool   ->LoadFromFile ("Data/paneltool.png");
	Textures::ImageGrid   ->LoadFromFile ("Data/img_grid.png");
	Textures::Preview     ->LoadFromFile ("Data/preview.png");
	Textures::Icons       ->LoadFromFile ("Data/icons.png");
	Textures::MsgIcons    ->LoadFromFile ("Data/msg_icons.png");
	Textures::Update();
	//
	// Fill Icons Data
	Icons.SizeIcon = 18;
	Icons.Width  = Icons.SizeIcon / Textures::Icons->Width;
	Icons.Height = Icons.SizeIcon / Textures::Icons->Height;
	//
	for (int i = 0; i < IconsCount; i++) {
		//
		for (int j = 0; j < IconsCount; j++) {
			Icons.OffsetsX[i][j] = (float)(9+i*18+3*i) / Textures::Icons->Width;
			Icons.OffsetsY[i][j] = (float)(9+j*18+3*j) / Textures::Icons->Height;
		}
	}

	ToolIcons.SizeIcon = 48;
	ToolIcons.Width  = ToolIcons.SizeIcon / Textures::TopToolIcons->Width;
	ToolIcons.Height = ToolIcons.SizeIcon / Textures::TopToolIcons->Height;
	//
	for (int i = 0; i < 4; i++) {
		//
		for (int j = 0; j < 4; j++) {
			ToolIcons.OffsetsX[i][j] = (float)(1+i*ToolIcons.SizeIcon+1*i) / Textures::TopToolIcons->Width;
			ToolIcons.OffsetsY[i][j] = (float)(1+j*ToolIcons.SizeIcon+1*j) / Textures::TopToolIcons->Height;
		}
	}
	//
	Theme->LoadFromFile(("Themes/"+CurrentTheme+"/Controls.thm").c_str());
	//Theme->SaveToBytecode(("Themes/"+CurrentTheme+"/Controls.dat").c_str());
	//Theme->LoadFromBytecode(("Themes/"+CurrentTheme+"/Controls.dat").c_str());
	Theme->Skin->LoadFromFile(("Themes/"+CurrentTheme+"/Controls.png").c_str());
	Theme->Update();
	Textures::UISkin = Theme->Skin;
	//Theme->SaveToBytecode(("Themes/"+CurrentTheme+"/Controls.dat").c_str());

		//
	VertexSize[0]  = Theme->Data["general"].Func["vertex"][2].Num;
	VertexSize[1]  = Theme->Data["general"].Func["vertex"][3].Num;
	fVertexSize[0] = Theme->Data["general"].Func["vertex"][2].Num / Theme->Skin->Width;
	fVertexSize[1] = Theme->Data["general"].Func["vertex"][3].Num / Theme->Skin->Height;

	VertexOffset[0] = Theme->Data["general"].Func["vertex"][0].Num / Theme->Skin->Width;
	VertexOffset[1] = Theme->Data["general"].Func["vertex"][1].Num / Theme->Skin->Height;

	VertexOffset[2] = Theme->Data["general"].Func["newvertex"][0].Num / Theme->Skin->Width;
	VertexOffset[3] = Theme->Data["general"].Func["newvertex"][1].Num / Theme->Skin->Height;

	VertexSize[4]  = Theme->Data["general"].Func["selvertex"][2].Num;
	VertexSize[5]  = Theme->Data["general"].Func["selvertex"][3].Num;
	fVertexSize[4] = Theme->Data["general"].Func["selvertex"][2].Num / Theme->Skin->Width;
	fVertexSize[5] = Theme->Data["general"].Func["selvertex"][3].Num / Theme->Skin->Height;
	
	VertexOffset[4] = Theme->Data["general"].Func["selvertex"][0].Num / Theme->Skin->Width;
	VertexOffset[5] = Theme->Data["general"].Func["selvertex"][1].Num / Theme->Skin->Height;
	//
	TargetSize      [0] = Theme->Data["general"].Func["target"][2].Num;
	TargetSize      [1] = Theme->Data["general"].Func["target"][3].Num;
	TargetTexSize   [0] = Theme->Data["general"].Func["target"][2].Num / Theme->Skin->Width;
	TargetTexSize   [1] = Theme->Data["general"].Func["target"][3].Num / Theme->Skin->Height;
	TargetTexOffset [0] = Theme->Data["general"].Func["target"][0].Num / Theme->Skin->Width;
	TargetTexOffset [1] = Theme->Data["general"].Func["target"][1].Num / Theme->Skin->Height;

	// Create Font
	//
	/*if (Theme->Data["general"].Func["font"][2].Num == 1.0f) {
		Theme->Font = ftglCreatePixmapFont(Theme->Data["general"].Func["font"][0].Str.c_str());
	} else {
		Theme->Font = ftglCreateBitmapFont(Theme->Data["general"].Func["font"][0].Str.c_str());
	}*/
		//Theme->Font = ftglCreateBitmapFont(Theme->Data["general"].Func["font"][0].Str.c_str());
	Theme->Font = ftglCreateTextureFont(Theme->Data["general"].Func["font"][0].Str.c_str());
	ftglSetFontCharMap (Theme->Font, FT_ENCODING_UNICODE);

	int size = Theme->Data["general"].Func["font"][1].Num;
	ftglSetFontFaceSize(Theme->Font, size, size);
	ftglSetFontCharMap(Theme->Font, ft_encoding_unicode);

	// Init GUI
	GUIManager->ColorShader	= ColorShader;
	GUIManager->Shader		= TexAtlasShader;
	GUIManager->Theme		= Theme;
	//
	if (OGLMajor >= 4) TexAtlasShader->AddGLSLShader("glsl/GL4/TexAtlas.glsl");
	else if (OGLMajor >= 3) TexAtlasShader->AddGLSLShader("glsl/GL3/TexAtlas.glsl");
	else TexAtlasShader->AddGLSLShader("glsl/GL2/TexAtlas.glsl");
	//
	TexAtlasShader->EnableShader();
	TexAtlasShader->Locations->Insert ("MVP"    , glGetUniformLocation (TexAtlasShader->ProgramObject, "MVP"));
	//TexAtlasShader->Locations->Insert ("ViewMatrix", glGetUniformLocation (TexAtlasShader->ProgramObject, "ViewMatrix"));
	//TexAtlasShader->Locations->Insert ("ProjMatrix", glGetUniformLocation (TexAtlasShader->ProgramObject, "ProjMatrix"));
	TexAtlasShader->Locations->Insert ("Size"   , glGetUniformLocation (TexAtlasShader->ProgramObject, "Size"));
	TexAtlasShader->Locations->Insert ("Offset" , glGetUniformLocation (TexAtlasShader->ProgramObject, "Offset"));
	TexAtlasShader->Locations->Insert ("Texture", glGetUniformLocation (TexAtlasShader->ProgramObject, "Texture"));
	TexAtlasShader->Locations->Insert ("Alpha"  , glGetUniformLocation (TexAtlasShader->ProgramObject, "Alpha"));
	//
	if (OGLMajor >= 4) TexAtlasAlphaShader->AddGLSLShader("glsl/GL4/TexAtlasAlpha.glsl");
	else if (OGLMajor >= 3) TexAtlasAlphaShader->AddGLSLShader("glsl/GL3/TexAtlasAlpha.glsl");
	else TexAtlasAlphaShader->AddGLSLShader("glsl/GL2/TexAtlasAlpha.glsl");
	//
	TexAtlasAlphaShader->EnableShader();
	TexAtlasAlphaShader->Locations->Insert ("MVP"    , glGetUniformLocation (TexAtlasAlphaShader->ProgramObject, "MVP"));
	//TexAtlasAlphaShader->Locations->Insert ("ViewMatrix", glGetUniformLocation (TexAtlasAlphaShader->ProgramObject, "ViewMatrix"));
	//TexAtlasAlphaShader->Locations->Insert ("ProjMatrix", glGetUniformLocation (TexAtlasAlphaShader->ProgramObject, "ProjMatrix"));
	TexAtlasAlphaShader->Locations->Insert ("Size"   , glGetUniformLocation (TexAtlasAlphaShader->ProgramObject, "Size"));
	TexAtlasAlphaShader->Locations->Insert ("Offset" , glGetUniformLocation (TexAtlasAlphaShader->ProgramObject, "Offset"));
	TexAtlasAlphaShader->Locations->Insert ("Texture", glGetUniformLocation (TexAtlasAlphaShader->ProgramObject, "Texture"));
	TexAtlasAlphaShader->Locations->Insert ("aSize"  , glGetUniformLocation (TexAtlasAlphaShader->ProgramObject, "aSize"));
	TexAtlasAlphaShader->Locations->Insert ("aOffset", glGetUniformLocation (TexAtlasAlphaShader->ProgramObject, "aOffset"));
	TexAtlasAlphaShader->Locations->Insert ("aTex"   , glGetUniformLocation (TexAtlasAlphaShader->ProgramObject, "aTex"));
	TexAtlasAlphaShader->Locations->Insert ("Alpha"  , glGetUniformLocation (TexAtlasAlphaShader->ProgramObject, "Alpha"));
	//
	if (OGLMajor >= 4) ColorShader->AddGLSLShader("glsl/GL4/Color.glsl");
	else if (OGLMajor >= 3) ColorShader->AddGLSLShader("glsl/GL3/Color.glsl");
	else ColorShader->AddGLSLShader("glsl/GL2/Color.glsl");
	//
	ColorShader->EnableShader();
	//ColorShader->Locations->Insert ("ViewMatrix", glGetUniformLocation (ColorShader->ProgramObject, "ViewMatrix"));
	//ColorShader->Locations->Insert ("ProjMatrix", glGetUniformLocation (ColorShader->ProgramObject, "ProjMatrix"));
	ColorShader->Locations->Insert ("MVP"  , glGetUniformLocation (ColorShader->ProgramObject, "MVP"));
	ColorShader->Locations->Insert ("Color", glGetUniformLocation (ColorShader->ProgramObject, "Color"));
	//
	if (OGLMajor >= 4) ColorLineShader->AddGLSLShader("glsl/GL4/ColorLine.glsl");
	else if (OGLMajor >= 3) ColorLineShader->AddGLSLShader("glsl/GL3/ColorLine.glsl");
	else ColorLineShader->AddGLSLShader("glsl/GL2/ColorLine.glsl");
	//
	ColorLineShader->EnableShader();
	ColorLineShader->Locations->Insert ("MVP"    , glGetUniformLocation (ColorLineShader->ProgramObject, "MVP"));
	ColorLineShader->Locations->Insert ("Color"  , glGetUniformLocation (ColorLineShader->ProgramObject, "Color"));
	ColorLineShader->Locations->Insert ("Pallete", glGetUniformLocation (ColorLineShader->ProgramObject, "Pallete"));
	ColorLineShader->Locations->Insert ("RGB"    , glGetUniformLocation (ColorLineShader->ProgramObject, "RGB"));
	ColorLineShader->Locations->Insert ("HSB"    , glGetUniformLocation (ColorLineShader->ProgramObject, "HSB"));
	//
	if (OGLMajor >= 4) ColorPickerShader->AddGLSLShader("glsl/GL4/ColorPicker.glsl");
	else if (OGLMajor >= 3) ColorPickerShader->AddGLSLShader("glsl/GL3/ColorPicker.glsl");
	else ColorPickerShader->AddGLSLShader("glsl/GL2/ColorPicker.glsl");
	//
	ColorPickerShader->EnableShader();
	ColorPickerShader->Locations->Insert ("MVP"    , glGetUniformLocation (ColorPickerShader->ProgramObject, "MVP"));
	ColorPickerShader->Locations->Insert ("Color"  , glGetUniformLocation (ColorPickerShader->ProgramObject, "Color"));
	ColorPickerShader->Locations->Insert ("Pallete", glGetUniformLocation (ColorPickerShader->ProgramObject, "Pallete"));
	ColorPickerShader->Locations->Insert ("RGB"    , glGetUniformLocation (ColorPickerShader->ProgramObject, "RGB"));
	ColorPickerShader->Locations->Insert ("HSB"    , glGetUniformLocation (ColorPickerShader->ProgramObject, "HSB"));
	//
	//
	Config = new DataMap();
	Config->LoadFromFile("Config.thm");
	//
	MapEditor = new MapEditor2D();
	MapEditor->Manager = GUIManager;
	MapEditor->Data->Manager = GUIManager;
	MapEditor->Data->LoadFromFile(("Data/"+lang+"/MapEditor.thm").c_str());
	//Terminate(-1);
	//MapEditor->Data->LoadFromBytecode(("Data/"+lang+"/MapEditor.dat").c_str());
	//MapEditor->Data->SaveToBytecode(("Data/"+lang+"/MapEditor.dat").c_str());
	//exit(0);
	MapEditor->Init();
	//
	finder = new Finder();
	finder->Data->Manager = GUIManager;
	finder->Data->LoadFromFile(("Data/"+lang+"/Finder.thm").c_str());
	//finder->Data->LoadFromBytecode(("Data/"+lang+"/Finder.dat").c_str());
	//finder->Data->SaveToBytecode(("Data/"+lang+"/Finder.dat").c_str());
	finder->Init();
	//
	addSpriteForm = new AddSpriteForm();
	addSpriteForm->Data->Manager = GUIManager;
	addSpriteForm->Data->LoadFromFile(("Data/"+lang+"/AddSprite.thm").c_str());
	//addSpriteForm->Data->SaveToBytecode(("Data/"+lang+"/AddSprite.dat").c_str());
	addSpriteForm->Init();
	//
	colorDialog = new ColorDialog();
	colorDialog->Data->Manager = GUIManager;
	colorDialog->Data->LoadFromFile(("Data/"+lang+"/ColorDialog.thm").c_str());
	//colorDialog->Data->SaveToBytecode(("Data/"+lang+"/ColorDialog.dat").c_str());
	colorDialog->Init();
	//colorDialog->Open(glm::vec4(1.f,110.f/255.f,86.f/255.f,1));
	
	addUniformDialog = new AddUniformDialog();
	addUniformDialog->Data->Manager = GUIManager;
	addUniformDialog->Data->LoadFromFile(("Data/"+lang+"/AddUniformDialog.thm").c_str());
	addUniformDialog->Init();

	addVarDialog = new AddVarDialog();
	addVarDialog->Data->Manager = GUIManager;
	addVarDialog->Data->LoadFromFile(("Data/"+lang+"/AddVarDialog.thm").c_str());
	addVarDialog->Init();
	//
	setPolygonForm = new SetPolygonForm();
	setPolygonForm->Data->Manager = GUIManager;
	setPolygonForm->Data->LoadFromFile(("Data/"+lang+"/SetPolygon.thm").c_str());
	setPolygonForm->Init();
	//
	preferencesDialog = new PreferencesDialog();
	preferencesDialog->Data->Manager = GUIManager;
	preferencesDialog->Data->LoadFromFile(("Data/"+lang+"/UserPreferences.thm").c_str());
	preferencesDialog->Init();
	//
	animationsDialog = new AnimationsDialog();
	animationsDialog->Data->Manager = GUIManager;
	animationsDialog->Data->LoadFromFile(("Data/"+lang+"/AnimationsDialog.thm").c_str());
	animationsDialog->Init();
	//
	okDialog = new OKDialog();
	okDialog->Data->Manager = GUIManager;
	okDialog->Data->LoadFromFile(("Data/"+lang+"/OKDialog.thm").c_str());
	okDialog->Init();

	retopologyDialog = new RetopologyDialog();
	retopologyDialog->Data->Manager = GUIManager;
	retopologyDialog->Data->LoadFromFile(("Data/"+lang+"/RetopologyDialog.thm").c_str());
	retopologyDialog->Init();

	// Set Uniforms / Vars

	DebugCamera = MapEditor->Camera;

	//
	//GUIManager->AddUIElement (MapEditor);
	GUIManager->Init();
	SplashScreen = MapEditor->Data->Elements["splashscreen"];

	HotKeys::Init();
}

//
Engine::~Engine() {
	//
	delete Theme;
	delete ScreenCamera;
	//delete GUIManager;
	delete TexAtlasShader;
	delete GUIElementDat;
	delete SpriteDat;
	delete ColorShader;
	delete TexAtlasAlphaShader;
	delete finder;
	delete MapEditor;
	delete addSpriteForm;
	delete addUniformDialog;
	delete colorDialog;
	delete preferencesDialog;
	//
	//Shaders.Free();
	Textures::Free();
}

//
void Engine::Draw() {
	//
	ScreenCamera->Update();

	//
	MapEditor->Render();
	//DebugDraw();
	GUIManager->Draw();
	
	// Dialogs
	addSpriteForm ->Render();
	colorDialog   ->Render();
	setPolygonForm->Render();

	ScreenCamera->NeedUpdate = false;

	/*GUIManager->Shader->ApplyShader();
	for (int i = 0; i < GUIManager->DrawStack.size(); i++) {
		GUIManager->DrawStack[i]->Draw(GUIManager->DrawStack[i]->Left, GUIManager->DrawStack[i]->Top);
	}

	GUIManager->DrawStack.clear();
	GUIManager->Shader->UnApplyShader();*/
	//UIThread.launch();
	//
	/*if (SplashScreen->Visible) {
		SplashScreen->Left = (WindowWidth  >> 1) - (SplashScreen->Width  >> 1);
		SplashScreen->Top  = (WindowHeight >> 1) - (SplashScreen->Height >> 1);
	}*/
}

//
void Engine::Step() {
	MapEditor->Step();
	finder->Step();
	addSpriteForm->Step();
	HotKeys::Step();
}

inline void ShowError (wstring msg) {
	okDialog->Open(msg, iError);
	LOG_ERROR("%s\n", wstring2string (msg).c_str());
}

inline void ShowWarning (wstring msg) {
	okDialog->Open(msg, iWarning);
}

#include "Events.cpp"
#endif
