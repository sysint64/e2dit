#ifndef H_EDITABLEOBJECT
#define H_EDITABLEOBJECT

//#define while(a) while (counterw(a)
//#define for (a) for (counterf(a))
namespace vertexcmp {
	inline bool vless(const glm::vec2 a, const glm::vec2 b) {
		return (a.x < b.x) || ((a.x == b.x) && (a.y < b.y));
	}

	inline bool vgreater(const glm::vec2 a, const glm::vec2 b) {
		return (a.x > b.x) || ((a.x == b.x) && (a.y > b.y));
	}

	inline bool hullcmp(const glm::vec2 a, const glm::vec2 b) {
		return (a.y > b.y) || ((a.y == b.y) && (a.x < b.x));
	}

	inline int LeftToRightCmp (glm::vec2 a, glm::vec2 b) {
		if (vless   (a, b)) return -1;
		if (vgreater(a, b)) return  1;

		return 0;
	}

	inline int RightToLeftCmp (glm::vec2 a, glm::vec2 b) {
		LeftToRightCmp(b, a);
	}
}

bool counterw(bool cond) {
	LOG_DEBUG("while\n");
	return cond;
}

bool counterf(bool cond) {
	LOG_DEBUG("for\n");
	return cond;
}

#define EPS numeric_limits<float>::epsilon()

namespace classify {
	enum {left, right, beyond, behind, between, origin, destination};
	enum {inside, outside};
	enum {touching, crossing, inessential};
	enum {collinear, parallel, skew, skewCross, skewNoCross};
}

inline int ClassifyPoint (const glm::vec2 &p2, const glm::vec2 &p0, const glm::vec2 &p1) {
	glm::vec2 a = p1-p0;
	glm::vec2 b = p2-p0;
	double sa = a.x*b.y-b.x*a.y;

	if (sa > 0.0f) return classify::left;
	if (sa < 0.0f) return classify::right;
	//if (a.x*b.x < 0.0f-numeric_limits<double>::epsilon() || a.y*b.y < 0.0f-numeric_limits<double>::epsilon()) return classify::behind;
	//if (glm::length(a) < glm::length(b)) return classify::beyond;
	//if (feq<double>(p0.x, p2.x) && feq<double>(p0.y, p2.y)) return classify::origin;
	//if (feq<double>(p1.x, p2.x) && feq<double>(p1.y, p2.y)) return classify::destination;

	return classify::left;
}

// Vertex

class Edge;
class Vertex {
public:
	Vertex	   *cw;
	Vertex	   *ccw;

	glm::vec2	Pos;
	glm::vec2	AbsPos;
	glm::vec2	TexCoord;
	int			Index;
	Edge	   *pEdge;

	Vertex (glm::vec2 p) : cw(this), ccw(this), Pos(p), pEdge(0) {}
	Vertex (glm::vec2 p, glm::vec2 t, int i) : cw(this), ccw(this), Pos(p), TexCoord(t), Index(i), pEdge(0) {}
	inline int Classify (const glm::vec2 &p0, const glm::vec2 &p1) {
		ClassifyPoint (Pos, p0, p1);
	}

	inline int Classify (Vertex *v1, Vertex *v2) {
		Classify (v1->Pos, v2->Pos);
	}
	Vertex *Insert(Vertex *v) {
		Vertex *c = cw;
		v->cw  = c;
		v->ccw = this;
		cw = v;
		c->ccw = v;

		return v;
	}

	Vertex *Remove() {
		ccw->cw = cw;
		cw->ccw = ccw;
		cw = ccw = this;
		return this;
	}
};

class PolygonT;
enum {elive, edead, efront};
class Edge {
private:
	bool FreeVertices;

public:
	Vertex	*v1, *v2;
	PolygonT *Tri1, *Tri2;
	int Type;
	
	Edge (Vertex *p1, Vertex *p2) : v1(p1), v2(p2), FreeVertices(false) {Type = elive; Tri1 = 0; Tri2 = 0;}
	Edge (glm::vec2 p1, glm::vec2 p2) {
		v1 = new Vertex(p1);
		v2 = new Vertex(p2);
		Type = elive;

		FreeVertices = true;
		Tri1 = 0;  Tri2 = 0;
	}

	~Edge() {
		if (FreeVertices) {
			delete v1;
			delete v2;
		}
	}

	inline glm::vec2 Point (const float t) { return v1->Pos+t*(v2->Pos-v1->Pos); }
	int Intersect (Edge e, float &t);

	inline int Dist2 (glm::vec2 p) {
		glm::vec2 p1 = v1->Pos;
		glm::vec2 p2 = v2->Pos;

		float a = p1.y - p2.y;
		float b = p2.x - p1.x;
		float c = p1.x*p2.y-p2.x*p1.y;

		return (a*p.x+b*p.y+c);
	}
	
	inline int Angle (glm::vec2 p) {
		glm::vec2 d1 = p-v1->Pos;
		glm::vec2 d2 = p-v2->Pos;

		d1 = glm::normalize (d1);
		d2 = glm::normalize (d2);

		return d1.x*d2.x+d1.y*d2.y; 
	}

	inline float Length() {
		glm::vec2 l = v2->Pos-v1->Pos;
		return glm::length(l);
	}

	inline bool HaveVertex (Vertex *v) {
		//return v1 == v || v2 == v;
		return  (feq<double>(v1->Pos.x, v->Pos.x, 0.5f) && feq<double>(v1->Pos.y, v->Pos.y, 0.5f)) ||
				(feq<double>(v2->Pos.x, v->Pos.x, 0.5f) && feq<double>(v2->Pos.y, v->Pos.y, 0.5f));
	}

	inline bool HaveVertex (glm::vec2 p) {
		//return v1 == v || v2 == v;
		return  (feq<double>(v1->Pos.x, p.x, 0.5f) && feq<double>(v1->Pos.y, p.y, 0.5f)) ||
				(feq<double>(v2->Pos.x, p.x, 0.5f) && feq<double>(v2->Pos.y, p.y, 0.5f));
	}

	inline int HaveVertices (Vertex *a, Vertex *b) {
		int cnt = 0;

		if (HaveVertex(a)) cnt++;
		if (HaveVertex(b)) cnt++;
		//if (v1 == a || v2 == a) cnt++;
		//if (v1 == b || v2 == b) cnt++;

		return cnt;
	}

	inline Vertex *GetCommonVertex (Edge *e) {
		if (v1 == e->v1 || v1 == e->v2) return v1;
		if (v2 == e->v1 || v2 == e->v2) return v2;

		return 0;
	}

	inline bool IsConnectingEdge (Edge *e1, Edge *e2) {
		Vertex *cp1 = GetCommonVertex (e1);
		Vertex *cp2 = GetCommonVertex (e2);

		return (cp1 != 0 && cp2 != 0 && cp1 != cp2);
	}

	inline float area (glm::vec2 a, glm::vec2 b, glm::vec2 c) {
		return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
	}
	
	inline bool intersect_1 (double a, double b, double c, double d) {
		if (a > b)  swap (a, b);
		if (c > d)  swap (c, d);
		return max(a,c) <= min(b,d);
	}

	bool Intersect (glm::vec2 a, glm::vec2 b) {
		glm::vec2 c = v1->Pos;
		glm::vec2 d = v2->Pos;

		return intersect_1 (a.x, b.x, c.x, d.x)
			&& intersect_1 (a.y, b.y, c.y, d.y)
			&& area(a,b,c) * area(a,b,d) <= 0.0f+EPS
			&& area(c,d,a) * area(c,d,b) <= 0.0f+EPS;

		/*glm::vec2 p1 = v1->Pos;
		glm::vec2 p2 = v2->Pos;
		//
		float rTop = (A.y-p1.y)*(p2.x-p1.x)-(A.x-p1.x)*(p2.y-p1.y);
		float sTop = (A.y-p1.y)*(B.x-A.x)-(A.x-p1.x)*(B.y-A.y);
		float rBot = (B.x-A.x)*(p2.y-p1.y)-(B.y-A.y)*(p2.x-p1.x);
		float sBot = (B.x-A.x)*(p2.y-p1.y)-(B.y-A.y)*(p2.x-p1.x);
		//
		if (feq<float>(rBot, 0.0f) || feq<float>(sBot, 0.0f))
			return false;
		//
		float r = rTop/rBot;
		float s = sTop/sBot;

		if (r > EPS && r < 1.0f-EPS && s > EPS && s < 1.0f-EPS)
			return true;

		return false;*/


	}

	bool Intersect (Edge *e, float &t) {
		glm::vec2 a = v1->Pos;
		glm::vec2 b = v2->Pos;
		glm::vec2 c = e->v1->Pos;
		glm::vec2 d = e->v2->Pos;
		glm::vec2 n = glm::vec2((d-c).y, (c-d).x);

		float denom = glm::dot(n, b-a);

		//if (denom < EPS && denom >= 0.0f)
		if (feq<float>(denom, 0.0f))
		//if (denom == 0.0f)
			return false;
		/*if (denom == 0.0f) {
			int aclass = ClassifyPoint(e->v1->Pos, e->v1->Pos, e->v2->Pos);

			if (aclass == classify::left || aclass == classify::right)
				return false;
			return false;
		}*/

		float num = glm::dot(n, a-c);
		t = -num/denom;
		return true;
	}

	inline bool Intersect (Vertex *A, Vertex *B) {
		return Intersect (A->Pos, B->Pos);
	}

	inline glm::vec2 Divide () {
		glm::vec2 n = glm::normalize(v2->Pos-v1->Pos);
		return v1->Pos+n*(Length()/2);
	}

	inline bool Intersect (Edge *e) {
		return Intersect (e->v1->Pos, e->v2->Pos);
		float t;
		Intersect (e, t);
		glm::vec2 cpt = Point (t);

		if (ClassifyPoint(cpt, v1->Pos, v2->Pos) == classify::between)
			return true;

		return false;
	}
};
//
bool cmpfunc(Edge *e1, Edge *e2) {
	return e1->Length() < e2->Length();	
}
//
class EdgeList {
public:
	vector<Edge*> List;

	inline bool isEmpty() { return List.empty(); }
	inline void push_back (Edge *e) { List.push_back(e); }
	inline int  find (Edge *e) {
		for (int i = 0; i < List.size(); i++) {
			Edge *te = List[i];

			if (te->HaveVertices(e->v1, e->v2) == 2)
				return i;
		}

		return -1;
	}

	inline int size() { return List.size(); }

	inline Edge *pop_back() {
		Edge *e = List.back();
		List.pop_back();
		return e;
	}

	inline void InsertOrDelete (Vertex *v1, Vertex *v2) {
		Edge *e = new Edge(v1, v2);
		int i = find(e);

		if (i >= 0) List.erase(List.begin()+i);
		else  {
			if (e->Type != edead) {
				e->Type = edead;
				push_back(e);
			}
		}
	}

	inline bool haveIsecs (Edge *e) {
		//for (vector<Edge*>::const_iterator it = List.begin(); it != List.end(); it++) {
		//	Edge *te = *it;
		for (int i = 0; i < List.size(); i++) {
			Edge *te = List[i];
			float t;

			if (te->HaveVertices (e->v1, e->v2) == 0)
				if (te->Intersect(e)) return true;

			return false;
		}
	}

	inline void sort() {
		std::sort(List.begin(), List.end(), cmpfunc);
	}
};

Vertex *max (Vertex *a, Vertex *b, Vertex *c) {
	Vertex *m = b;

	if (a->Pos.x > m->Pos.x || a->Pos.y > m->Pos.y) m = a;
	if (c->Pos.x > m->Pos.x || c->Pos.y > m->Pos.y) m = c;

	return m;
}

Vertex *min (Vertex *a, Vertex *b, Vertex *c) {
	Vertex *m = b;

	if (a->Pos.x < m->Pos.x || a->Pos.y < m->Pos.y) m = a;
	if (c->Pos.x < m->Pos.x || c->Pos.y < m->Pos.y) m = c;

	return m;
}

//
class PolygonT {
protected:
	Vertex *StartV;
	bool Started;
	int Size;

public:
	bool NeedDel;
	vector<Vertex*> Vertices;
	Vertex *Iter;
	EdgeList *Edges;

	PolygonT() : StartV(0), Iter(StartV), Started(false), Size(0), NeedDel(false) { Edges = new EdgeList(); }
	PolygonT(Edge *e1, Edge *e2, Edge *e3) : Started(false), Size(0), NeedDel(false) {
		StartV = 0;
		Edges = new EdgeList();

		Insert (e1->v1);
		Insert (e1->v2);

		if (e2->HaveVertex(e3->v1)) Insert(e3->v1);
		if (e2->HaveVertex(e3->v2)) Insert(e3->v2);

		Edges->push_back(e1);
		Edges->push_back(e2);
		Edges->push_back(e3);
	}

	PolygonT(Vertex *p1, Vertex *p2, Vertex *p3) : Started(false), Size(0) {
		Edges = new EdgeList();

		Insert (p1);
		Insert (p2);
		Insert (p3);

		Edges->push_back(new Edge(p1, p2));
		Edges->push_back(new Edge(p2, p3));
		Edges->push_back(new Edge(p3, p1));

		StartV = 0;
	}

	PolygonT (glm::vec2 &p1, glm::vec2 &p2, glm::vec2 &p3) : Started(false), Size(0), NeedDel(false) {
		StartV = 0; Iter = 0;
		Edges = new EdgeList();

		Insert (new Vertex(p1));
		Insert (new Vertex(p2));
		Insert (new Vertex(p3));

		Edges->push_back(new Edge(p1, p2));
		Edges->push_back(new Edge(p2, p3));
		Edges->push_back(new Edge(p3, p1));
	}

	~PolygonT() {
		Start();

		if (Iter) {
			Vertex *w = Iter->cw;

			while (Iter != w) {
				delete w->Remove();
				w = Iter->cw;
			}
		}

		delete Iter;
		delete Edges;
	}

	bool HaveEdge(Edge *e) {
		return Edges->find(e) >= 0;
	}

	inline int IndexOf (Vertex *v) {
		for (int i = 0; i < Vertices.size(); i++)
			if (Vertices[i] == v) return i;

		return -1;
	}

	inline bool End() {
		if (Started) {
			Started = false;
			return StartV == 0 || Iter->cw == StartV;
		}

		return StartV == 0 || Iter == StartV;
	}

	inline void Start() {
		Iter = StartV;
		Started = true;
	}

	inline void Next() {
		Iter = Iter->cw;
	}

	inline Vertex *Insert (glm::vec2 p, glm::vec2 t, int i) {
		if (StartV == 0) {
			StartV = new Vertex(p, t, i);
			Iter = StartV;
		} else Iter = Iter->Insert(new Vertex(p, t, i));
		
		Vertices.push_back(Iter);

		//Vertices.insert(Vertices.begin()+Iter->Index, Iter);
		return Iter;
	}

	inline Vertex *Insert (glm::vec2 p) {
		return Insert(p, glm::vec2(1), -1);
	}

	inline Vertex *Insert (Vertex *v) {
		/*Vertex *nv = new Vertex(v->Pos, v->TexCoord, v->Index);
		nv->pEdge = v->pEdge;

		if (StartV == 0) {
			StartV = nv;
			Iter = StartV;
		} else Iter = Iter->Insert(nv);

		Vertices.push_back(Iter);
		return Iter;*/

		Vertex *nv = new Vertex(v->Pos, v->TexCoord, v->Index);
		nv->pEdge = v->pEdge;

		if (StartV == 0) {
			StartV = nv;
			Iter = StartV;
		} else Iter = Iter->Insert(nv);

		Vertices.push_back(Iter);
		return Iter;
	}

	inline void Clear() {
		StartV = 0;
		Vertices.clear();
	}
};
//
int PointInPolygon(glm::vec2 a, PolygonT *p) {	
	//
	Edge *l1 = new Edge(a, glm::vec2 ( a.x, 1000));
	Edge *l2 = new Edge(a, glm::vec2 ( a.x,-1000));
	Edge *l3 = new Edge(a, glm::vec2 ( 1000, a.y));
	//Edge *l4 = new Edge(a, glm::vec2 (-1000, a.y));

	int parity = 0;

	for (int i = 0; i < p->Vertices.size(); i++) {
		Vertex* v = p->Vertices[i];
		Edge *e = new Edge (v, v->cw);

		if (l1->Intersect(e)) parity = 1-parity;
		delete e;
	}

	delete l1;
	return parity;
}
//
class EditableObject : public BaseObject {
private:
	vector<PolygonT*> Tris;
	vector<PolygonT*> Parts;
	glm::vec2 testpt;
	vector<glm::vec2> dpts;
	vector<PolygonT*> Circles;
	vector<glm::vec2> FrontPoints;
	vector<Edge*> dEdges;

	PolygonT *Clip (Edge *e, int orient = classify::left, bool cut = false);
	Vertex *WiderAngleToEdge (Edge *e, vector<Vertex*> *pts, EdgeList *cont);
	inline bool isecWContours(Edge *e, Vertex *pt, EdgeList *cont) {
		if (cont->haveIsecs(new Edge(pt, e->v1))) return true;
		if (cont->haveIsecs(new Edge(pt, e->v2))) return true;

		return false;
	}
	Vertex *Mate (Edge *e, vector<Vertex*> *pts);
	void AddDebugTri(PolygonT *Tri);
	vector<PolygonT*> &SplitTri (int eidx, PolygonT *Tri, glm::vec2 &CrossPt);
	bool TrySplitTri (PolygonT *t);
	vector<Edge*> Border;

protected:
	PolygonT *Polygon;

public:
	PolygonT *SelectPoly;
	vector<glm::vec4> VertexAbsPos;
	BaseMaterial	 *Material;

	 EditableObject() { Polygon = new PolygonT(); SelectPoly = new PolygonT(); Material = new BaseMaterial(); }
	~EditableObject() { delete Polygon; }

	void Triangulate (); // -/-/-/-/- // ???
	void Triangulate2(); // +/-/-/+/+ // Greed Triangulate , Slow!
	void Triangulate3(); // -/-/-/-/- // Delone
	void Triangulate4(); // -/-/-/-/- // Delone
	void Triangulate5(); // +/+/+/-/- // Delone Truangulate, Fast! // FIX DELONE COND FOR BIG ANGLES && CROSSES WITH BORDER!

	void Split (Edge *e); // +
	void Cut (Edge *e); // +
	void Divide (int HorizN, int VertN); // +
	void Partition (Edge *e); // +
	void DebugRender (glm::vec2 pos); // +
	void CopyFromDataRender(); // +
	void CopyFromDataPolygon (PolygonT *P); // +
	void BuildDataRender(); // +
	//
	virtual void DrawTopology() {}
	virtual void DrawVertices() {}
	virtual void DrawEdges (const bool Selected = false) {}
	virtual void UpdatePosVertices() {}
	virtual void UpdateListVertices() {}
};

#endif
