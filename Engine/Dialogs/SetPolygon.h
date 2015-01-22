//* Author : Kabilin Andrey, 2013-2014 *//

#ifndef H_SET_POLYGON
#define H_SET_POLYGON

//
class BaseGameObject;

#define pdtCircle	0
#define pdtBox		1
#define pdtPolygon	2

class PolyData : public PolygonT {
public:
	wstring Name;
	int Type;
	bool NoDelete;
	//vector<glm::vec2> Vertices;
	float CircleRadius;
	glm::vec2 Pos;
	glm::vec2 BoxSize;

	void Copy (PolyData *P) {
		Name = P->Name; Type = P->Type;
		Type = P->Type; NoDelete = P->NoDelete;
		CircleRadius = P->CircleRadius;
		Pos = P->Pos; BoxSize = P->BoxSize;
		//
		//for (int i = 0; i < P->Vertices.size(); i++) {
		//	Insert (new Vertex(P->Vertices[i]->Pos));
		//	Edges->push_back(new Edge(P->Vertices[i]->Pos, P->Vertices[i]->cw->Pos));
		//}
		Vertex *s = P->Vertices[0];
		for (int i = 0; i < P->Vertices.size(); i++) {
			Insert (new Vertex(s->Pos));
			s = s->cw;
		}
	}
};

class SetPolygonForm {
private:
	UIDialog  *Dialog;
	Texture2D *Texture;
	PolyData  *Poly;
	int		   x0, y0;
	bool	   isMouseDown;
	glm::vec2  TmpPos0;
	glm::vec2  TmpSize0;
	BaseGameObject *Object, *SrcObj;
	float FinalAng, LastAng;
	bool cClick;
	bool MayClick;
	float DeltaRadius;
	float MouseDist;
	Vertex *InsVertex;
	glm::vec2 cptf;
	bool AddedVertex;
	bool MayMoveVertex;
	bool MoveVertex;
	int SelVertexId;
	int AddedXScroll;
	int AddedYScroll;
	float LastVX, LastVY;
	float VPMaxX, VPMaxY;
	float VPMinX, VPMinY;
	//
public:
	UIData  *Data;
	UIPanel	*BrowsePanel;
	UIColorPanel *GridImage;

	int		 PolyId;
	bool	 Snap;
	int		 SnapStep;
	int		 Zoom;
	float	 ZoomK;
	Recti	 Select;

	//
	SetPolygonForm()  {
		Data = new UIData();
		Zoom = 100; InsVertex = 0;
		ZoomK = 1.f;
		AddedVertex = false; MayMoveVertex = false;
		MoveVertex  = false;
		AddedXScroll = 0; AddedYScroll = 0;
	}

	~SetPolygonForm() { delete Data; }
	//
	// Events
	static void OnCancelClick (UIElement *Sender);
	static void OnOKClick     (UIElement *Sender);
	//
	static void OnBoxTypeClick     (UIElement *Sender);
	static void OnCircleTypeClick  (UIElement *Sender);
	static void OnPolygonTypeClick (UIElement *Sender);
	//
	static void OnChangeZoom     (UIElement *Sender);
	static void OnSetDefaulrZoom (UIElement *Sender);
	//
	void Init();
	void Step();
	void Render();
	void Open (BaseGameObject *Obj, int aPolyId);
	//
	void KeyPressed (Uint16 key);
	void MouseDown  (int x, int y, int shift);
	void MouseUp    (int x, int y, int shift);
	//
	inline void UpdateScroll(Vertex* CV = 0);
	//inline void UpdateScroll(Vertex* CV = 0);
};

SetPolygonForm *setPolygonForm;

#endif
