#ifndef H_DICTS
#define H_DICTS 1

namespace Textures {
	Texture2D	*PanelTool;
	Texture2D	*Icons;
	Texture2D	*SplashScreen;
	Texture2D	*Preview;
	Texture2D	*PreviewFloat;
	Texture2D	*AtlasEdit;
	Texture2D	*TopToolIcons;
	Texture2D	*UISkin;
	Texture2D	*ImageGrid;
	Texture2D	*TestExistTex;
	Texture2D	*MsgIcons;

	inline void Init() {
		PanelTool	 = new Texture2D();
		Icons		 = new Texture2D();
		SplashScreen = new Texture2D();
		Preview		 = new Texture2D();
		PreviewFloat = new Texture2D();
		AtlasEdit	 = new Texture2D();
		TopToolIcons = new Texture2D();
		ImageGrid    = new Texture2D();
		TestExistTex = new Texture2D();
		MsgIcons	 = new Texture2D();
		//
		ImageGrid->WrapS = GL_REPEAT;
		ImageGrid->WrapT = GL_REPEAT;
	}

	inline void Update() {
		//
		PanelTool->Update();
		Icons->Update();
		SplashScreen->Update();
		Preview->Update();
		TopToolIcons->Update();
		ImageGrid->Update();
		MsgIcons->Update();
	}
	//
	inline void Free() {
		delete PanelTool;
		delete Icons;
		delete SplashScreen;
		delete Preview;
		delete AtlasEdit;
		delete TopToolIcons;
		delete ImageGrid;
		delete TestExistTex;
		delete MsgIcons;
	}

	inline Texture2D *GetByName (const string Name) {
		//
		if (Name == "toolpanel")	return PanelTool;
		if (Name == "icons")		return Icons;
		if (Name == "splashscreen")	return SplashScreen;
		if (Name == "preview")		return Preview;
		if (Name == "previewfloat")	return PreviewFloat;
		if (Name == "atlasedit")	return AtlasEdit;
		if (Name == "toptoolicons") return TopToolIcons;
		if (Name == "uiskin")		return UISkin;
		if (Name == "imagegrid")	return ImageGrid;
		if (Name == "msgicons")		return MsgIcons;
	}
}

#define IconsCount 11
#define ThemeTexID 3
//
BaseShader	*TexAtlasShader;
BaseShader	*TexAtlasAlphaShader;
BaseShader	*ColorShader;
BaseShader	*ColorLineShader;
BaseShader	*ColorPickerShader;
//
struct {
	float OffsetsX[IconsCount][IconsCount];
	float OffsetsY[IconsCount][IconsCount];
	float Width; float Height;
	float SizeIcon;
} Icons;

struct {
	float OffsetsX[IconsCount][IconsCount];
	float OffsetsY[IconsCount][IconsCount];
	float Width; float Height;
	float SizeIcon;
} ToolIcons;

#endif;
