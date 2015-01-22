//* Author : Kabilin Andrey, 2013 *//

#ifndef H_MAPEDITOR_2D
#define H_MAPEDITOR_2D
//
struct PhysicsParams {

};

struct LabelData {
	wstring Name;
	bool NoDelete;
	glm::vec2 Point;
	glm::vec2 Dir;
};

BaseUniforms *GlobalVars;
class PointObject;
class BaseGameObject : public EditableObject {
public:
	wstring Name;
	BaseGameObject *Template;
	BaseUniforms *Vars;
	int Id, LayerId;
	string GameClass;
	bool NoSelect;
	//
	glm::vec2 TexOffset;
	glm::vec2 TexSize;

	float lastAng;
	glm::vec2 Size0;
	glm::vec2 Pos0;
	//
	bool Deleted;

	// Parameters
	PhysicsParams		PhysParams;
	vector<AnimData2D*>	AnimsList;
	vector<PolyData*>	PolysList;
	vector<LabelData>	LabelsList;
	vector<PointObject*> JoinedPoints;
	//
	BaseGameObject() {
		Vars = new BaseUniforms();
		Vars->Copy (GlobalVars);
		Deleted = false;
		NoSelect = false;
	}

	virtual void DrawTopology();
	virtual void UpdatePosVertices();
	virtual void UpdateListVertices();
	virtual void DrawVertices();
	virtual void DrawEdges (const bool Selected = false);
	//
	void DisplayMainTab();
	void DisplayVars();
	void DisplayMaterialVars();
	void DisplayPolygonsList();
	void DisplayLabelsList();
	void DisplayAnimsList();

	inline void DisplayParams() {
		DisplayMainTab();
		DisplayVars();
		//DisplayMaterialVars();
		DisplayPolygonsList();
		DisplayLabelsList();
		DisplayAnimsList();
	}
};

class PolygonObject : public BaseGameObject {
public:
	virtual void UpdatePosVertices();
	virtual void UpdateListVertices();

	virtual void DrawTopology() {}
	virtual void EmptyRender();
	virtual void DrawVertices();
	virtual void DrawEdges (const bool Selected = false);
	//
	inline  void AddVertex(glm::vec2 &pos) {
		Polygon->Insert(pos);
	}

	inline void DeleteVertex (const int index) {
		delete Polygon->Vertices[index]->Remove();
		Polygon->Vertices.erase(Polygon->Vertices.begin()+index);
	}
};

class PointObject : public BaseGameObject {
public:
	int VertexJoinId;
	BaseGameObject *JoinObj;
	glm::vec2 dPosition, dPositionLast;

	virtual void UpdatePosVertices();
	virtual void UpdateListVertices();
	virtual void UpdatePosJoined();
	//
	virtual void DrawTopology() {}
	virtual void DrawVertices() {}
	virtual void DrawEdges (const bool Selected = false) {}
	virtual void EmptyRender();
};

enum  LightType {ltStationar, ltDynamic, ltStatic};
class LightObject : public BaseGameObject {
public:
	float Radius;
	float Attenuation;
	glm::vec4 Color;
	LightType Type;
	bool CastShadow;

	virtual void UpdatePosVertices();
	virtual void UpdateListVertices();
	//
	virtual void DrawTopology() {}
	virtual void DrawVertices() {}
	virtual void DrawEdges (const bool Selected = false) {}
	virtual void EmptyRender();
};

class TilesObjects : public BaseGameObject {
public:
	struct { int x; int y; } typedef TileCoord;
	map <TileCoord*, BaseGameObject*> Grid;
	//
	int TileSize[2];
	int TileOffset[2];

	void InitGrid();
	void RecalcGrid();
};

//
class Layer {
public:
	string Name;
	vector <BaseGameObject*> Objects;
	bool Visible;
	bool Select;
	//
	Layer () { Visible = true; Select = true; }
	void Render ();
};
//
// Select Tool
#include "Groupes.h"
//
//
struct SpriteGroup {
	wstring Name;
	vector<SpriteGroupItem> Items;
};

struct PolygonGroup {
	wstring Name;
	vector<PolygonGroupItem> Items;
};

struct PointGroup {
	wstring Name;
	vector<PointGroupItem> Items;
};

struct LightGroup {
	wstring Name;
	vector<LightGroupItem> Items;
};
//
#define tRect		0
#define tCircle		1
#define tLaso		2

// Add Tool
#define tNone		0
#define tSprite		1
#define tPolygon	2
#define tLight		3
#define tPoint		4
#define tRailpoly	5
#define tSound		6
#define tTiles		7
#define tMulti		8

// Transform Tool
#define tMove		0
#define tRotate		1
#define tScale		2
#define tFree		3 // Может не нужно

// Mode
#define tObject		0
#define tEdit		1
#define tAdd		2

//
class MapEditor2D {
private:
	int mShift;
	int mx, my;
	int dx, dy;
	int cx, cy;
	int dgx, dgy;
	float lastAng, tmpAng;
	//
	BaseGameObject  *SPCData;
	//
	int CurLayerId;
	glm::vec2		MouseWay;
	glm::vec2		SOSize0;
	glm::vec2		SOPos0;
	glm::vec2		dPoint;
	glm::vec4		eDebug;
	glm::vec4		eDebug2;
	glm::vec2		dSnapPos;
	bool ClickedOnSO;
	bool PosSnaped;
	//
public:
	BaseMaterial	*SpriteMaterial;
	BaseMaterial	*PolygonMaterial;
	BaseGameObject	*SpriteCursor;
	BaseObject		*VertexCursor;
	//
	glm::ivec2 RoomSize;

	bool GridSnap;
	bool CtrlSnap;
	bool SnapOffset;
	int  StepGrid;
	bool AddOnMapObj;
	bool SkipMouseUp;
	bool ShowLightRadius;
	float GridColor[3];
	float SelectColor[3];
	float CameraColor[3];
	float RoomBorderColor[3];
	bool LockPivot;
	//
	vector<BaseGameObject*> ObjTemplates;
	vector<Camera2D*> Cameras;
	
	UIData *VarsUIData;
	UIData *UniformsUIData;

	BaseMaterial	*CurMaterial;
	vector<BaseMaterial*> MaterialLibrary;
	//vector<BaseEffect*> EffectLibrary;
	//
	int CurrAddTool;
	int CurrTransformTool;
	int CurrSelectTool;
	int CurrMode;
	int CurSelMode;
	//
	BaseObject	*VertexObject;
	wstring CurSpriteGroup;
	int CurSpriteId;
	bool AddSpriteTool;
	GLuint	LastTex;
	BaseShader *LastShader;
	BaseGameObject *SelectedObject;
	vector<BaseGameObject*> SelectedObjects;
	vector<int> SelectedVertices;
	glm::vec2 SelectRect[2];
	glm::vec2 SelectRectWorld[2];
	glm::vec2 TransformPivot;
	glm::vec2 TransformPivotScreen;
	bool GroupedPivot;
	bool RotOnlyPos; // Rotate Group and no change Rotation on elements
	char ScaleArea;
	//
	wstring CurPolygonGroup;
	int CurPolygonId;
	//
	wstring CurPointGroup;
	int CurPointId;

	wstring CurLightGroup;
	int CurLightId;
	//
	UIListMenu	*SpritesList;
	UIListMenu	*MultimediaList;
	UIListMenu	*LayersList;
	UIListMenu	*PolygonsList;
	UIListMenu	*PointsList;
	UIListMenu	*LightsList;
	UIEdit		*LayerEdit;
	//
	// Groupes Menu
	UIListMenu	*PolygonGoupsList;
	UIListMenu	*PointGoupsList;
	UIListMenu	*LightGoupsList;
	UIListMenu	*SpriteGoupsList;
	//
	UIImage		*SelectTool;
	UIImage		*AddTool;
	UIImage		*TransformTool;
	//
	UIPanel		*SpriteTabs;
	UIPanel		*SurfaceTabs;
	UIPanel		*PolygonsPanel;
	UIPanel		*SpritesPanel;
	UIPanel		*DetailsPanel;
	//
	UIEdit		*eSizeW, *eSizeH, *eAngle;
	//
	UIGroupedElements *SnapElements;
	UIGroupedElements *ModeElements;
	//
	UICheckButton *SnapButton;
	UICheckButton *ObjectModeButton;
	UICheckButton *EditModeButton;
	UICheckButton *AddModeButton;
	//
	Camera2D *Camera;
	UIData *Data;
	UIManager *Manager;
	int Left, Top;
	int Width, Height;
	bool OnlyTranslate = false;
	//
	//vector <SpriteGroup>  SpritesGroups;
	//vector <PolygonGroup> PolygonsGroups;

	map <wstring, SpriteGroup>  SpritesGroups;
	map <wstring, PolygonGroup> PolygonsGroups;
	map <wstring, PointGroup>   PointsGroupes;
	map <wstring, LightGroup>   LightsGroupes;

	SpriteGroupesT *SpriteGroupes;
	BaseGroupes<PolygonGroupItem> *PolygonGroupes;
	BaseGroupes<PointGroupItem>   *PointGroupes;
	BaseGroupes<LightGroupItem>   *LightGroupes;
	//
	vector <Layer*> Layers;
	//
	void Render();
	void Init();
	//
	void DblClick   (int x, int y, int Shift);
	void MouseDown  (int x, int y, int Shift);

	void MouseMove  (int x, int y, int Shift);
	void MouseWheel (int delta, int x, int y);
	void MouseUp    (int x, int y, int Shift);
	//
	void KeyDown    (int key);
	void KeyPressed (Uint16 key);
	//
	void Step();

	// Update Groupes
	void UpdateSpriteGroup();
	void UpdatePolygonGroup();
	void UpdateTransformPivot();
	//
	MapEditor2D() {
		Data = new UIData();
		Camera = new Camera2D();
		Camera->Position = glm::vec2 (-60.0f, -60.0f);
		Camera->Zoom = 1.f;
		mShift = mouseNone; dgx = 0; dgy = 0;
		AddSpriteTool = false; GridSnap = false;
		StepGrid = 32; SnapOffset = true;
		CurLayerId = 0; CtrlSnap = false;
		AddOnMapObj = false; SelectedObject = 0;
		CurPolygonId = 0; GroupedPivot = true;
		RotOnlyPos = false; LockPivot = false;
		//
		// Tool
		CurrSelectTool		= tRect;
		CurrAddTool 		= tSprite;
		CurrTransformTool	= tMove;
		CurrMode			= tObject;
		CurSelMode			= tNone;
		//
		LastTex = 0;
		LastShader = 0;
		SkipMouseUp = false;
		ClickedOnSO = false;
		PosSnaped   = false;
		ShowLightRadius = true;

		//
		Camera2D *MainGameCamera = new Camera2D();
		MainGameCamera->Size = glm::ivec2(800, 600);
		MainGameCamera->Name = L"MainCamera";
		Cameras.push_back(MainGameCamera);
		RoomSize = glm::ivec2(1024, 1024);
	}
	//
	~MapEditor2D() {
		delete Data;
		//delete SpriteGroupes;
		delete PolygonGroupes;
		delete SPCData;

		for (int i = 0; i < ObjTemplates.size(); i++)
			delete ObjTemplates[i];

	}

	inline void SetModeButtonsE (bool e1, bool e2) {
		EditModeButton  ->Checked = false;
		AddModeButton   ->Checked = false;
		ObjectModeButton->Checked = true;
		//
		EditModeButton->Enabled = e1;
		AddModeButton ->Enabled = e2;
	}

	inline void UpdateModeButtons() {
		EditModeButton  ->Checked = false;
		AddModeButton   ->Checked = false;
		ObjectModeButton->Checked = true;

		switch (CurSelMode) {
			case tNone     : EditModeButton->Enabled = false; AddModeButton->Enabled = false; break;
			case tSprite   : EditModeButton->Enabled = true;  AddModeButton->Enabled = true;  break;
			case tPolygon  : EditModeButton->Enabled = true;  AddModeButton->Enabled = true;  break;
			case tLight    : EditModeButton->Enabled = true;  AddModeButton->Enabled = false; break;
			case tPoint    : EditModeButton->Enabled = false; AddModeButton->Enabled = false; break;
			//case tRailpoly : EditModeButton->Enabled = true;  AddModeButton->Enabled = true;  break;
			case tSound    : EditModeButton->Enabled = false; AddModeButton->Enabled = false; break;
			case tTiles    : EditModeButton->Enabled = false; AddModeButton->Enabled = true;  break;
		}
	}

	//
	void SetVisibileElements (bool Light = false, bool Point = false) {

		Data->Elements["llightradius"]->Visible = Light; Data->Elements["elightradius"]->Visible = Light;
		Data->Elements["lattenuation"]->Visible = Light; Data->Elements["eattenuation"]->Visible = Light;
		Data->Elements["cbcastshadow"]->Visible = Light; Data->Elements["llightcolor" ]->Visible = Light;
		Data->Elements["clightcolor" ]->Visible = Light; Data->Elements["llighttype"  ]->Visible = Light;
		Data->Elements["glighttype"  ]->Visible = Light; Data->Elements["bltdynamic"  ]->Visible = Light;
		Data->Elements["bltstationar"]->Visible = Light; Data->Elements["bltstatic"   ]->Visible = Light;

		Data->Elements["gsize" ]->Visible = !Light && !Point; Data->Elements["lsize" ]->Visible = !Light && !Point;
		Data->Elements["langle"]->Visible = !Light && !Point; Data->Elements["eangle"]->Visible = !Light && !Point;

		if (Light) {
			Data->Elements["pparamstab"]->Height = 335;
		} else if (Point) {
			Data->Elements["pparamstab"]->Height = 185;
		} else {
			Data->Elements["pparamstab"]->Height = 205;
		}

	}

	// UI Events
	//
	// Parameters Panel
	// Polygon
	static void EditPPolygonDbl     (UIElement *Sender, int x, int y, int Shift);
	//
	// Mode, Toolbar
	static void SetSpriteObject    (UIElement *Sender);
	static void SetPolygonObject   (UIElement *Sender);
	static void SetLightObject     (UIElement *Sender);
	static void SetPointObject     (UIElement *Sender);
	static void SetRailpolyObject  (UIElement *Sender);
	static void SetSoundObject     (UIElement *Sender);
	static void SetTilesObject     (UIElement *Sender);
	//
	static void SetObjectMode      (UIElement *Sender);
	static void SetEditMode        (UIElement *Sender);
	static void SetAddMode         (UIElement *Sender);

	//
	// Tool Panel
	static void SetToolSelRect     (UIElement *Sender);
	static void SetToolSelCircle   (UIElement *Sender);
	static void SetToolSelLaso     (UIElement *Sender);
	//
	static void SetToolAddSprite   (UIElement *Sender);
	static void SetToolAddSurface  (UIElement *Sender);
	static void SetToolAddLight    (UIElement *Sender);
	static void SetToolAddCoord    (UIElement *Sender);
	//
	static void SetToolMove        (UIElement *Sender);
	static void SetToolRotate      (UIElement *Sender);
	static void SetToolScale       (UIElement *Sender);
	//
	// Main Params
	static void ChangeClassName	   (UIElement *Sender);
	static void SetLightColor      (UIElement *Sender);
	static void SetLightColorCallback();

	static void SetDynamicLightType    (UIElement *Sender);
	static void SetStaticLightType     (UIElement *Sender);
	static void SetStationarLightType  (UIElement *Sender);
	static void SetLightCastShadowType (UIElement *Sender);

	// Sprites
	static void AddSpriteFile      (UIElement *Sender);
	static void SelectSprite       (UIElement *Sender, int x, int y, int Shift);
	static void AddSpriteOnMap     (UIElement *Sender, int x, int y, int Shift);
	static void AddSpriteGroup     (UIElement *Sender);
	//
	static void SetSnap            (UIElement *Sender);

	// Polygons
	static void AddPolygon         (UIElement *Sender);
	static void EditPolygonName    (UIElement *Sender, Uint16 key);
	static void EditPolygonNameDbl (UIElement *Sender, int x, int y, int Shift);
	static void SelectPolygon      (UIElement *Sender, int x, int y, int Shift);
	static void ChangePolygonName  (UIElement *Sender);

	// Points
	BaseGameObject *AddPoint (const wstring Name);
	static void AddPointClick      (UIElement *Sender);
	static void EditPointName      (UIElement *Sender, Uint16 key);
	static void EditPointNameDbl   (UIElement *Sender, int x, int y, int Shift);
	static void SelectPoint        (UIElement *Sender, int x, int y, int Shift);
	static void ChangePointName    (UIElement *Sender);

	// Lights
	BaseGameObject *AddLight (const wstring Name);
	static void AddLightClick      (UIElement *Sender);
	static void EditLightName      (UIElement *Sender, Uint16 key);
	static void EditLightNameDbl   (UIElement *Sender, int x, int y, int Shift);
	static void SelectLight        (UIElement *Sender, int x, int y, int Shift);
	static void ChangeLightName    (UIElement *Sender);

	// Tiles
	static void AddTileMap         (UIElement *Sender);
	static void EditTileName       (UIElement *Sender, Uint16 key);
	static void EditTileNameDbl    (UIElement *Sender, int x, int y, int Shift);
	static void SelectTile         (UIElement *Sender, int x, int y, int Shift);
	static void ChangeTileName     (UIElement *Sender);

	// Group
	static void AddPolygonGroup    (UIElement *Sender);
	static void SetPolygonGroup    (UIElement *Sender);
	static void ChangePolygonGroupName (UIElement *Sender);
	static void DeletePolygonGroup (UIElement *Sender);

	static void AddPointGroup      (UIElement *Sender);
	static void SetPointGroup      (UIElement *Sender);
	static void ChangePointGroupName (UIElement *Sender);
	static void DeletePointGroup   (UIElement *Sender);

	static void AddLightGroup      (UIElement *Sender);
	static void SetLightGroup      (UIElement *Sender);
	static void ChangeLightGroupName (UIElement *Sender);
	static void DeleteLightGroup   (UIElement *Sender);

	// Layers
	static void SelectLayer        (UIElement *Sender, int x, int y, int Shift);
	static void EditLayerDbl       (UIElement *Sender, int x, int y, int Shift);
	static void EditLayer          (UIElement *Sender, Uint16 key);
	static void ChangeLayerName    (UIElement *Sender);
	static void AddLayer           (UIElement *Sender);
	static void DeleteLayer        (UIElement *Sender);
	static void UpLayer            (UIElement *Sender);
	static void DownLayer          (UIElement *Sender);

	// Materials Tab
	static void AddUniform         (UIElement *Sender);
	static void AddVar             (UIElement *Sender);

	// Main Menu
	// - File
	static void ShowPreferences    (UIElement *Sender);
	static void Quit			   (UIElement *Sender);

	// - Modify
	static void Retopology		  (UIElement *Sender);
	static void Boolean			  (UIElement *Sender);
	static void CenterPivot		  (UIElement *Sender);
	static void JoinPointToVertex (UIElement *Sender);
	//
	void SelectObject (int x, int y);
	void SelectVertices();

	// Save
	void SaveToTextFile (const char *fn);
	inline void SaveToBinFile  (const char *fn);

	// Write Data
	void WritePolygonObjectData (PolygonObject  *Obj, FILE *F);
	void WritePointObjectData   (PointObject    *Obj, FILE *F);
	void WriteLightObjectData   (LightObject	*Obj, FILE *F);
	void WriteSpriteObjectData  (BaseGameObject *Obj, FILE *F);
	//
	void WriteJoinedPoints (BaseGameObject *Obj, FILE *F);
	void WriteMaterialData (BaseGameObject *Obj, FILE *F);
	void WritePolygonsData (BaseGameObject *Obj, FILE *F);
	void WriteLabelsData   (BaseGameObject *Obj, FILE *F);
};
//
MapEditor2D *MapEditor;
#endif
