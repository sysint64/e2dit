#ifndef CPP_EDITABLEOBJECT
#define CPP_EDITABLEOBJECT
//

namespace vertexcmp {
	inline bool vless(const glm::vec2 a, const glm::vec2 b) {
		return (a.x < b.x) || ((a.x == b.x) && (a.y < b.y));
	}

	inline bool vgreater(const glm::vec2 a, const glm::vec2 b) {
		return (a.x > b.x) || ((a.x == b.x) && (a.y > b.y));
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

inline int Orientation (const glm::vec2 &p0, const glm::vec2 &p1, const glm::vec2 &p2) {
	glm::vec2 a = p1-p0;
	glm::vec2 b = p2-p0;

	float sa = a.x*b.y-b.x*a.y;

	if (sa > 0.0f) return  1;
	if (sa < 0.0f) return -1;
	
	return 0;
}

inline int ClassifyPoint (const glm::vec2 &p2, const glm::vec2 &p0, const glm::vec2 &p1) {
	glm::vec2 a = p1-p0;
	glm::vec2 b = p2-p0;
	float sa = a.x*b.y-b.x*a.y;

	if (sa > 0.0f) return classify::left;
	if (sa < 0.0f) return classify::right;
	if (a.x*b.x < 0.0f || a.y*b.y < 0.0f) return classify::behind;
	if (a.length() < b.length()) return classify::beyond;
	if (p0 == p2) return classify::origin;
	if (p1 == p2) return classify::destination;

	return classify::between;
}

int EdgeType (glm::vec2 &a, Edge &e) {
	glm::vec2 v = e.org;
	glm::vec2 w = e.dest;
	//
	switch (ClassifyPoint(a, e.org, e.dest)) {
		case classify::left :
			return (v.y < a.y && a.y <= w.x) ? classify::crossing : classify::inessential;
		case classify::right :
			return (w.y < a.y && a.y <= w.y) ? classify::crossing : classify::inessential;

		case classify::between :
		case classify::origin :
		case classify::destination :
			return classify::touching;
		default :
			return classify::inessential;
	}
}

int PointInPolygon(glm::vec2 a, PolygonT &p) {
	int parity = 0;

	for (int i = 0; i < p.size; i++, p.Advance(traverse::closwise)) {
		Edge e = p.edge();

		switch (EdgeType(a, e)) {
			case classify::touching : return classify::inside;
			case classify::crossing : parity = 1-parity;
		}
	}

	return parity ? classify::inside : classify::outside;
}

inline float DotProduct(glm::vec2 p, glm::vec2 q) {
	return p.x*q.x+p.y*q.y;
}

Vertex *LeastVertex(PolygonT &p, int (*cmp)(glm::vec2, glm::vec2)) {
	Vertex *bestV = p.v();
	p.Advance(traverse::closwise);

	for (int i = 1; i < p.size; p.Advance(traverse::closwise), i++) {
		if ((*cmp)(p.v()->Point, bestV->Point) < 0)
			bestV = p.v();
	}

	p.SetV(bestV);
	return bestV;
}

float PointDistance(glm::vec2 p, Edge &e) {
	Edge ab = e;
	ab.Flip().Rotate();
	glm::vec2 n(ab.dest-ab.org);

	n = (1.0f / n.length())*n;
	Edge f(p, p+n);

	float t;
	f.Intersect(e, t);

	return t;
}

bool Adjacent (Vertex *v, Vertex *w) {
	return w == v->cw || w == v->ccw;
}

int AdvancePtr (Vertex *&vu, Vertex *&vl, Vertex *&v) {
	Vertex *vun = vu->cw;
	Vertex *vln = vl->ccw;
	//
	if (vertexcmp::vless(vun->Point, vln->Point)) {
		v = vu =vun;
		return classify::upper;
	} else {
		v = vl = vln;
		return classify::lower;
	}
}

// Vertex ---------------------------------------->

Vertex *Vertex::Insert(Vertex *v) {
	Vertex *c = cw;
	v->cw  = c;
	v->ccw = this;
	cw = v;
	c->ccw = v;

	return c;
}

Vertex *Vertex::Remove() {
	ccw->cw = cw;
	cw->ccw = ccw;
	cw = ccw = this;
	return this;
}

void Vertex::Splice(Vertex *b) {
	Vertex *a  = this;
	Vertex *an = a->cw;
	Vertex *bn = b->cw;
	
	a->cw   = bn; b->cw   = an;
	an->ccw = b;  bn->ccw = a;
}

Vertex* Vertex::Split(Vertex *b) {
	Vertex *bp = b->ccw->Insert(new Vertex(b->Point));
	Insert(new Vertex(Point));
	Splice(bp);
	return bp;
}

// <---------------------------------------- Vertex


// PolygonT -------------------------------------->

PolygonT::PolygonT (PolygonT &p) {
	size = p.size;

	if (size == 0) {
		_v = 0;
	} else {
		_v = new Vertex (p.v()->Point);
		
		for (int i = 1; i < size; i++) {
			p.Advance(traverse::closwise);
			_v = _v->Insert(new Vertex(p.v()->Point));
		}

		p.Advance(traverse::closwise);
		_v = _v->cw;
	}
}

PolygonT::~PolygonT() {
	if (_v) {
		Vertex *w = _v->cw;

		while (_v != w) {
			delete w->Remove();
			w = _v->cw;
		}
	}

	delete _v;
}

Vertex *PolygonT::Insert (const glm::vec2 p) {
	if (size++ == 0) _v = new Vertex(p);
	else _v = _v->Insert(new Vertex(p));

	return _v;
}

void PolygonT::Remove() {
	Vertex *vv = _v;
	_v = (--size == 0) ? 0 : _v->ccw;
	delete vv->Remove();
}

PolygonT *PolygonT::Split (Vertex *b) {
	Vertex *bp = _v->Split(b);
	Resize();
	return new PolygonT(bp);
}

Edge PolygonT::edge() {
	return Edge(_v->Point, _v->cw->Point);
}

// <-------------------------------------- PolygonT


// Edge ------------------------------------------>

Edge &Edge::Rotate() {
	glm::vec2 m = 0.5f*(org+dest);
	glm::vec2 v = dest-org;
	glm::vec2 n(v.y, -v.x);

	org  = m-0.5f*n;
	dest = m+0.5f*n;

	return *this;
}

int Edge::Intersect (Edge e, float &t) {
	glm::vec2 a = org;
	glm::vec2 b = dest;
	glm::vec2 c = e.org;
	glm::vec2 d = e.dest;
	glm::vec2 n = glm::vec2((d-c).y, (c-d).x);

	float denom = DotProduct(n, b-a);

	if (denom == 0.0f) {
		int aclass = ClassifyPoint(e.org, e.org, e.dest);

		if (aclass == classify::left || aclass == classify::right)
			return classify::parallel;
		else return classify::collinear;
	}

	float num = DotProduct(n, a-c);
	t = -num/denom;
	return classify::skew;
}

int Edge::Cross (Edge &e, float &t) {
	float s;
	int crossType = e.Intersect(*this, s);

	if (crossType == classify::collinear || crossType == classify::parallel)
		return crossType;

	if (s < 0.0f || s > 1.0f)
		return classify::skewNoCross;

	Intersect(e, t);

	if (t >= 0.0f && t <= 1.0f) return classify::skewCross;
	else return classify::skewNoCross;
}

// <------------------------------------------ Edge


// Editable Object ------------------------------->

void EditableObject::CopyFromDataRender() {
	Polygons.clear();
	Polygon = new PolygonT();
	Polygons.push_back(Polygon);

	for (int i = 0; i < Data->VertexBuffer.size(); i++) {
		Polygon->Insert(Data->VertexBuffer[i]*Size);
	}

	Polygon = new PolygonT();
	Polygons.push_back(Polygon);

	Polygon->Insert(glm::vec2(0, 0));
	Polygon->Insert(glm::vec2(10, 40));
	Polygon->Insert(glm::vec2(50, 34));
	Polygon->Insert(glm::vec2(34, 4));
	Polygon->Insert(glm::vec2(10, -5));
}

void EditableObject::DebugRender(glm::vec2 pos) {
	glPointSize( 6.0 );
	glColor3f(1.0f, 0, 0);
	glBegin2D();
		for (int i = 0; i < Polygons.size(); i++) {
			PolygonT s = *Polygons[i];
			glBegin(GL_LINE_LOOP);

				for (int i = 0; i < s.size; s.Advance(traverse::closwise), i++) {
					glVertex2f(pos.x+s.Point().x, pos.y+s.Point().y);
					glBegin(GL_POINTS);
						glColor3f(0, 1.0f, 0);
						glVertex2f(pos.x+s.Point().x, pos.y+s.Point().y);
					glEnd();
					glColor3f(1.0f, 0, 0);
				}

			glEnd();

			glColor3f(0, 0, 1.0f);
			glBegin(GL_LINES);
				glVertex2f(pos.x-100, pos.y+10);
				glVertex2f(pos.x+100, pos.y+10);
			glEnd();
		}

		// Draw Triangles
		glColor3f(0, 0, 1.0f);

	glEnd2D();
}

void EditableObject::Cut(Edge e) {
	PolygonT s = *Polygon;
	PolygonT *p = new PolygonT();
	glm::vec2 crossingPt;
	//
	for (int i = 0; i < s.size; s.Advance(traverse::closwise), i++) {
		glm::vec2 org = s.Point();
		glm::vec2 dest = s.cw()->Point;

		int orgIsInside  = (ClassifyPoint(org , e.org, e.dest) != classify::left);
		int destIsInside = (ClassifyPoint(dest, e.org, e.dest) != classify::left);

		if (orgIsInside != destIsInside) {
			float t;
			e.Intersect(s.edge(), t);
			crossingPt = e.Point(t);
		}

		if (orgIsInside && destIsInside) {
			p->Insert(dest);
		} else if (orgIsInside && !destIsInside) {
			if (org != crossingPt)
				p->Insert(crossingPt);
		} else if (!orgIsInside && !destIsInside) {

		} else {
			p->Insert(crossingPt);
			if (dest != crossingPt)
				p->Insert(dest);
		}
	}

	delete Polygon;
	Polygon = p;
}

void EditableObject::TriangulateFanPoly(PolygonT &p, vector<PolygonT*> *tris) {
	Vertex *w = p.v()->cw->cw;
	int size = p.size;

	for (int i = 3; i < size; i++) {
		tris->push_back(p.Split(w));
		w = w->cw;
	}

	tris->push_back(&p);
}

void EditableObject::Triangulate() {
	PolygonT p = *Polygon;

	Stack<Vertex*> s;
	vector<PolygonT*> *tris = new vector<PolygonT*>();
	Polygons.clear();

	Vertex *v, *vu, *vl;
	LeastVertex(p, vertexcmp::RightToLeftCmp);

	v = vu = vl = p.v();
	s.push(v);
	int chain = AdvancePtr(vl, vu, v);
	s.push(v);

	//
	//while (1) {
		chain = AdvancePtr(vl, vu, v);

		/*if (Adjacent(v, s.top()) && !Adjacent(v, s.bottom())) { LOG_DEBUG("Case A\n");
			int side = (chain == classify::upper) ? classify::left : classify::right;
			Vertex *a = s.top();
			Vertex *b = s.nextToTop();

			while (s.size() > 1 && ClassifyPoint(b->Point, v->Point, a->Point) == side) {
				if (chain == classify::upper) {
					p.SetV(b);
					tris->push_back(p.Split(v));
				} else {
					p.SetV(v);
					tris->push_back(p.Split(b));
				}

				s.pop();
				a = b;
				b = s.nextToTop();
			}

			s.push(v);
			break;
		} else if (!Adjacent(v, s.top())) { LOG_DEBUG("Case B\n");
			PolygonT *q;
			Vertex *t = s.pop();
			//
			if (chain == classify::upper) {
				p.SetV(t);
				q = p.Split(v);
			} else {
				p.SetV(v);
				q = p.Split(t);
				q->Advance(traverse::closwise);
			}

			TriangulateFanPoly(*q, tris);
			
			while (!s.empty())
				s.pop();

			s.push(t);
			s.push(v);
		} else {*/
		//	LOG_DEBUG("Case C\n");
			p.SetV(v);
			TriangulateFanPoly(p, tris);
			//break;
		//}
	//}

	Polygons = *tris;
}

// <------------------------------- Editable Object

#endif
