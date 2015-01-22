#ifndef CPP_EDITABLEOBJECT
#define CPP_EDITABLEOBJECT

int EdgeType (glm::vec2 a, Edge *e) {
	glm::vec2 v = e->v1->Pos;
	glm::vec2 w = e->v2->Pos;
	//
	switch (ClassifyPoint(a, e->v1->Pos, e->v2->Pos)) {
		case classify::left :
			return (v.y < a.y && a.y <= w.x) ? classify::crossing : classify::inessential;
		case classify::right :
			return (w.y < a.y && a.y <= v.y) ? classify::crossing : classify::inessential;

		case classify::between :
		case classify::origin :
		case classify::destination :
			return classify::touching;
		default :
			return classify::inessential;
	}
}

bool PointInConvexPolygon (Vertex *v, PolygonT *p) {
	glm::vec2 s = v->Pos;

	if (p->Vertices.size() == 1) return (v == p->Vertices[0]);
	if (p->Vertices.size() == 2) {
		int c = v->Classify(p->Vertices[0]->Pos, p->Vertices[1]->Pos);
		return c == classify::between || c == classify::origin || c == classify::destination;
	}

	for (int i = 0; i < p->Vertices.size(); i++) {
		if (v->Classify(p->Vertices[i], p->Vertices[i]->cw) == classify::left) {
			return false;
		}
	}

	return true;
}

inline int PointOrientation (const glm::vec2 p0, const glm::vec2 p1, const glm::vec2 p2) {
	glm::vec2 a = p1-p0;
	glm::vec2 b = p2-p0;

	float sa = a.x*b.y-b.x*a.y;

	if (sa > 0.0f) return  1;
	if (sa < 0.0f) return -1;
	
	return 0;
}

double polarAngle (glm::vec2 p) {
	if (feq<double>(p.x, 0.0f) && feq<double>(p.y, 0.0f)) return -1.0f;
	if (feq<double>(p.x, 0.0f)) return p.y >= 0.0f ? 90.f : 270.f;

	double theta = atan(p.y/p.x);
	theta *= 360.f/(2*pi);

	if (p.x > 0.0f-EPS) return p.y >= 0.0f-EPS ? theta : 360.f+theta;
	else return 180.f+theta;
}

double signedAngle (glm::vec2 a, Edge* e) {
	glm::vec2 v = e->v1->Pos-a;
	glm::vec2 w = e->v2->Pos-a;

	double va = polarAngle(v);
	double wa = polarAngle(w);

	if (feq<double>(va, 0.0f) || feq<double>(wa, 0.0f)) return 180.f;

	double x = wa-va;

	if (feq<double>(x, 180.0f) || feq<double>(x, -180.0f)) return 180.0f;
	else if (x < -180.0f) return x+360.0f;
	else if (x >  180.0f) return x-360.0f;
	else return x;
}

bool IsPerpendicular (glm::vec2 pt1, glm::vec2 pt2, glm::vec2 pt3) {
	float yDelta_a = pt2.y - pt1.y;
	float xDelta_a = pt2.x - pt1.x;
	float yDelta_b = pt3.y - pt2.y;
	float xDelta_b = pt3.x - pt2.x;
	//
	if (fabs(xDelta_a) <= EPS && fabs(yDelta_b) <= EPS) return false;
	if (fabs(yDelta_a) <= EPS)      return true;
	else if (fabs(yDelta_b) <= EPS) return true;
	else if (fabs(xDelta_a) <= EPS) return true;
	else if (fabs(xDelta_b) <= EPS) return true;
	else                            return false;
}

float CalcCircle (glm::vec2 pt1, glm::vec2 pt2, glm::vec2 pt3, glm::vec2 *m_Center) {
	float yDelta_a = pt2.y - pt1.y;
	float xDelta_a = pt2.x - pt1.x;
	float yDelta_b = pt3.y - pt2.y;
	float xDelta_b = pt3.x - pt2.x;
	float m_dRadius = -1.0f;
	//
	if (fabs(xDelta_a) <= EPS && fabs(yDelta_b) <= EPS) {
		m_Center->x = 0.5*(pt2.x + pt3.x);
		m_Center->y = 0.5*(pt1.y + pt2.y);
		m_dRadius = glm::length(pt1-*m_Center);

		return m_dRadius;
	}
	
	float aSlope = yDelta_a/xDelta_a;
	float bSlope = yDelta_b/xDelta_b;

	if (fabs(aSlope-bSlope) <= EPS)	return -1;

	// calc center
	m_Center->x =  (aSlope*bSlope*(pt1.y - pt3.y) + bSlope*(pt1.x + pt2.x) - aSlope*(pt2.x+pt3.x) )/(2* (bSlope-aSlope));
	m_Center->y = -(m_Center->x - (pt1.x+pt2.x/2))/aSlope + (pt1.y+pt2.y)/2;

	//m_dRadius= length(&m_Center,pt1);		// calc. radius
	m_dRadius = glm::length(pt1-*m_Center);
	return m_dRadius;
}

float Circle (glm::vec2 pt1, glm::vec2 pt2, glm::vec2 pt3, glm::vec2 *m_Center) {
	if      (!IsPerpendicular(pt1, pt2, pt3) ) return CalcCircle(pt1, pt2, pt3, m_Center);	
	else if (!IsPerpendicular(pt1, pt3, pt2) ) return CalcCircle(pt1, pt3, pt2, m_Center);	
	else if (!IsPerpendicular(pt2, pt1, pt3) ) return CalcCircle(pt2, pt1, pt3, m_Center);	
	else if (!IsPerpendicular(pt2, pt3, pt1) ) return CalcCircle(pt2, pt3, pt1, m_Center);	
	else if (!IsPerpendicular(pt3, pt2, pt1) ) return CalcCircle(pt3, pt2, pt1, m_Center);	
	else if (!IsPerpendicular(pt3, pt1, pt2) ) return CalcCircle(pt3, pt1, pt2, m_Center);	
	else return -1;
}

/*function TriangleCircumcenter2D(triV0, triV1, triV2: TVector2f): TVector2f;
var X1, X2, Y1, Y2, A, L10, L20: single;
begin
  X1:= triV1.x - triV0.x;
  X2:= triV2.x - triV0.x;
  Y1:= triV1.y - triV0.y;
  Y2:= triV2.y - triV0.y;

  L10:= X1*X1 + Y1*Y1;
  L20:= X2*X2 + Y2*Y2;

  A:= 0.5*(X1*Y2 - X2*Y1);
  A:= 1/(4*A);

  Result.x:= triV0.x + (Y2*L10 - Y1*L20) * A;
  Result.y:= triV0.y + (X1*L20 - X2*L10) * A;
end;*/

double TriCircle (glm::vec2 v0, glm::vec2 v1, glm::vec2 v2, glm::vec2 *res) {
	glm::vec2 p1 = v1-v0;
	glm::vec2 p2 = v2-v0;

	float angle = glm::orientedAngle(glm::normalize(p1), glm::normalize(p2));
	//if (feq<float>(angle,   pi, 0.2f)) return -angle;
	if (angle > pi-0.05f) return -angle;
	//if (feq<float>(angle, 2*pi, 0.2f)) return -angle;
	//if (feq<float>(angle,  -pi, 0.2f)) return -angle;
	//if (feq<float>(angle,-2*pi, 0.2f)) return -angle;

	double l1 = p1.x*p1.x + p1.y*p1.y;
	double l2 = p2.x*p2.x + p2.y*p2.y;

	double a = 0.5f*(p1.x*p2.y-p2.x*p1.y);
	double b = 1.f/(4.f*a);

	res->x = v0.x+(p2.y*l1-p1.y*l2)*b;
	res->y = v0.y+(p1.x*l2-p2.x*l1)*b;

	return glm::length(v0-*res);
}

inline bool PointInAngle(glm::vec2 v0, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, float angle) {
	glm::vec2 p1 = v0-v1;
	glm::vec2 p2 = v2-v1;

	//float angle2 = glm::orientedAngle(glm::normalize(p1), glm::normalize(p2));
	//if (angle2 < angle) return true;
	
	if (ClassifyPoint(v0, v1, v2) == classify::left &&
		ClassifyPoint(v0, v1, v3) == classify::right) return true;

	if (ClassifyPoint(v0, v1, v2) == classify::right &&
		ClassifyPoint(v0, v1, v3) == classify::left) return true;

	return false;
}

PolygonT *PolygonCircle(glm::vec2 center, float raduis) {
	PolygonT *p = new PolygonT();

	for (int i = 0; i < 100; i++) {
		float a = (float)i/100.0f*pi*2.0f;

		float x = cos(a)*raduis+center.x;
		float y = sin(a)*raduis+center.y;

		Vertex *v = p->Insert(glm::vec2(x, y));
	}

	return p;
}

// Editable Object ------------------------------->

void EditableObject::CopyFromDataRender() {
	Parts.clear();
	Tris.clear();
	Polygon = new PolygonT();
	Parts.push_back(Polygon);
	//
	for (int i = 0; i < Data->VertexBuffer.size(); i++) {
		Vertex *v = Polygon->Insert(Data->VertexBuffer[i]*Size, Data->TexCoordsBuffer[i], i);
	}
}

void EditableObject::CopyFromDataPolygon (PolygonT *P) {
	Parts.clear();
	Tris.clear();
	Polygon = new PolygonT();

	Vertex *s = P->Vertices[0];
	for (int i = 0; i < P->Vertices.size(); i++) {
		//glVertex2f (x0+s->Pos.x*ZoomK, y0+s->Pos.y*ZoomK);
		glm::vec2 TexCoord;
		glm::vec2 Pos;

		Pos.x = s->Pos.x;//Size.x;
		Pos.y = s->Pos.y;//Size.y;

		TexCoord.s = 0.f+((Texture->Width /2.f+s->Pos.x)/Texture->Width);
		TexCoord.t = 1.f-((Texture->Height/2.f+s->Pos.y)/Texture->Height);

		Polygon->Insert (Pos, TexCoord, i);
		s = s->cw;
	}
}

void EditableObject::BuildDataRender() {
	DataRender *NewData = new DataRender();

	NewData->RenderMode = GL_TRIANGLES;
	NewData->Static = false;

	for (int i = 0; i < Polygon->Vertices.size(); i++) {
		NewData->AddVertex (glm::vec2(Polygon->Vertices[i]->Pos.x/Size.x,
									  Polygon->Vertices[i]->Pos.y/Size.y),
									  Polygon->Vertices[i]->TexCoord);
	}

	for (int i = 0; i < Tris.size(); i++) {
		bool skip = false;
		for (int j = 0; j < 3; j++) {
			if (Tris[i]->Vertices[j]->Index == -1)
				skip = true;
		}

		if (skip) continue;

		for (int j = 0; j < 3; j++) {
			glm::vec2 tv = Tris[i]->Vertices[j]->Pos;
			//NewData->AddIndex(Tris[i]->Vertices[j]->Index);
			for (int k = 0; k < Polygon->Vertices.size(); k++) {
				glm::vec2 pv = Polygon->Vertices[k]->Pos;
				if (feq<double>(pv.x, tv.x, 0.5f) && feq<double>(pv.y, tv.y, 0.5f)) {
					NewData->AddIndex(k);
					printf("Index: %d\n", k);
					break;
				}
			}
		}
	}

	printf("Inices: %d\n", NewData->Indices.size());

	Data = NewData;
	UpdateListVertices();
}

void EditableObject::DebugRender(glm::vec2 pos) {
	glPointSize (6.0f);
	glDisable (GL_LINE_SMOOTH);
	//glLineWidth(2);

	glBegin2D();

		for (int i = 0; i < Tris.size(); i++) {
			PolygonT *Tri = Tris[i];

			/*glColor3f (0, 1.0f, 0);
			glBegin(GL_TRIANGLES);
				for (int j = 0; j < Tris[i]->Vertices.size(); j++) {
					glm::vec2 Pos = Tris[i]->Vertices[j]->Pos;
					glVertex2f (pos.x+Pos.x, pos.y+Pos.y);
				}
			glEnd();*/

			glColor3f (1.0f, 1.0f, 0);
			glBegin(GL_LINE_LOOP);
				for (int j = 0; j < Tris[i]->Vertices.size(); j++) {
					glm::vec2 Pos = Tris[i]->Vertices[j]->Pos;
					glVertex2f (pos.x+Pos.x, pos.y+Pos.y);
				}
			glEnd();
		}

		for (int i = 0; i < Parts.size(); i++) {
			PolygonT *Part = Parts[i];

			glColor3f (1.0f, 0, 0);
			glBegin(GL_LINE_LOOP);

			for (int j = 0; j < Parts[i]->Vertices.size(); j++) {
				glm::vec2 Pos = Parts[i]->Vertices[j]->Pos;
				//glVertex2f (pos.x+Pos.x, pos.y+Pos.y);
			}
			/*Part->Start();

			while (!Part->End()) {
				glVertex2f (pos.x+Part->Iter->Pos.x, pos.y+Part->Iter->Pos.y);
				Part->Next();
			}*/

			glEnd();

			glColor3f (0, 0, 1.0f);
			glBegin (GL_POINTS);

			/*Part->Start();

			while (!Part->End()) {
				glVertex2f (pos.x+Part->Iter->Pos.x, pos.y+Part->Iter->Pos.y);
				Part->Next();
			}*/
			for (int j = 0; j < Parts[i]->Vertices.size(); j++) {
				glm::vec2 Pos = Parts[i]->Vertices[j]->Pos;
				//glVertex2f (pos.x+Pos.x, pos.y+Pos.y);
			}

			glEnd();
		}

		glPointSize (6.0f);
		for (int i = 0; i < dpts.size(); i++) {
			glColor3f(1.0f,0,0);
			glBegin(GL_POINTS);
				glVertex2f(pos.x+dpts[i].x, pos.y+dpts[i].y);
			glEnd();
		}

		for (int i = 0; i < Circles.size(); i++) {
			glColor3f(float(i+1)/float(Circles.size()),float(i+1)/float(Circles.size()),float(i+1)/float(Circles.size()));
			glBegin(GL_LINE_LOOP);

			for (int j = 0; j < Circles[i]->Vertices.size(); j++) {
				glm::vec2 Pos = Circles[i]->Vertices[j]->Pos;
				glVertex2f (pos.x+Pos.x, pos.y+Pos.y);
			}

			glEnd();
		}

		/*glColor3f(1.0f,0.0f,1.0f);
		glBegin(GL_LINE_LOOP);

			for (int j = 0; j < FrontPoints.size(); j++) {
				glm::vec2 Pos = FrontPoints[j];
				glVertex2f (pos.x+Pos.x, pos.y+Pos.y);
			}

			glEnd();*/

		glColor3f(1.0f,0.0f,1.0f);
		glBegin(GL_LINES);
			for (int i = 0; i < dEdges.size(); i++) {
				glm::vec2 Pos = dEdges[i]->v1->Pos;
				glVertex2f (pos.x+Pos.x, pos.y+Pos.y);
				Pos = dEdges[i]->v2->Pos;
				glVertex2f (pos.x+Pos.x, pos.y+Pos.y);
			}
		glEnd();
		/*glColor3f(1.0f,0,0);
		glBegin(GL_POINTS);
			glVertex2f(pos.x+testpt.x, pos.y+testpt.y);
		glEnd();*/
	glEnd2D();

}

PolygonT *EditableObject::Clip (Edge *e, int orient, bool cut) {
	PolygonT *p = new PolygonT();
	glm::vec2 crossingPt;

	Polygon->Start();
	while (!Polygon->End()) {
		glm::vec2 org  = Polygon->Iter->Pos;
		glm::vec2 dest = Polygon->Iter->cw->Pos;

		int orgIsInside  = (ClassifyPoint (org , e->v1->Pos, e->v2->Pos) != orient);
		int destIsInside = (ClassifyPoint (dest, e->v1->Pos, e->v2->Pos) != orient);

		if (orgIsInside != destIsInside) {
			float t;
			Edge *tmp = new Edge (new Vertex(org), new Vertex(dest));
			e->Intersect (tmp, t);
			crossingPt = e->Point (t);

			delete tmp->v1;
			delete tmp->v2;
			delete tmp;
		}

		if (orgIsInside && destIsInside) {
			p->Insert (dest, Polygon->Iter->TexCoord, Polygon->Iter->Index);
		} else if (orgIsInside && !destIsInside) {
			if (org != crossingPt)
				p->Insert (crossingPt, Polygon->Iter->TexCoord, Polygon->Iter->Index);

			if (!cut)
				p->Insert (dest, Polygon->Iter->TexCoord, Polygon->Iter->Index);
		} else if (!orgIsInside && !destIsInside) {
			if (!cut)
				p->Insert (dest, Polygon->Iter->TexCoord, Polygon->Iter->Index);
		} else {
			p->Insert (crossingPt, Polygon->Iter->TexCoord, Polygon->Iter->Index);

			if (dest != crossingPt)
				p->Insert (dest, Polygon->Iter->TexCoord, Polygon->Iter->Index);
		}

		Polygon->Next();
	}

	return p;
}

void EditableObject::Split (Edge *e) {
	PolygonT *p = Clip (e, classify::left, false);
	delete Polygon;
	Polygon = p;

	Parts.clear();
	Parts.push_back(p);
}

//
void EditableObject::Cut (Edge *e) {
	PolygonT *p = Clip (e, classify::left, true);
	delete Polygon;
	Polygon = p;

	Parts.clear();
	Parts.push_back(p);
}

void EditableObject::Partition (Edge *e) {
	Parts.clear();

	PolygonT *LeftPart  = Clip (e, classify::left , true);
	PolygonT *RightPart = Clip (e, classify::right, true);

	Parts.push_back (LeftPart);
	Parts.push_back (RightPart);

	Polygon = LeftPart;
}

void EditableObject::Divide (int HorizN, int VertN) {
	Polygon->Start();

	glm::vec2 vmin = Polygon->Iter->Pos;
	glm::vec2 vmax = Polygon->Iter->Pos;

	Polygon->Next();

	while (!Polygon->End()) {
		if (Polygon->Iter->Pos.x < vmin.x) vmin.x = Polygon->Iter->Pos.x;
		if (Polygon->Iter->Pos.y < vmin.y) vmin.y = Polygon->Iter->Pos.y;
		if (Polygon->Iter->Pos.x > vmax.x) vmax.x = Polygon->Iter->Pos.x;
		if (Polygon->Iter->Pos.y > vmax.y) vmax.y = Polygon->Iter->Pos.y;
		//
		Polygon->Next();
	}

	glm::vec2 area = vmax-vmin;

	for (int i = 0; i < HorizN; i++) {
		float x = vmin.x+(area.x/(HorizN+1))*(i+1);
		Edge *e = new Edge(glm::vec2(x, -1), glm::vec2(x, 1));
		Split(e);
	}

	//
	for (int i = 0; i < VertN; i++) {
		float y = vmin.y+(area.y/(VertN+1))*(i+1);
		Edge *e = new Edge(glm::vec2(-1, y), glm::vec2(1, y));
		Split(e);
	}
}

#define sqr(x) x*x
bool PInLine(glm::vec2 p, Edge *e) {
	bool isec = false;
	glm::vec2 v1 = e->v2->Pos-e->v1->Pos;
	glm::vec2 v2 = p-e->v1->Pos;
	//glm::abs(v1);
	if (glm::length(v2) <= glm::length(v1)) isec = true;


	glm::vec2 vmax;
	glm::vec2 vmin;

	vmax.x = max(e->v1->Pos.x, e->v2->Pos.x);
	vmax.y = max(e->v1->Pos.y, e->v2->Pos.y);

	vmin.x = min(e->v1->Pos.x, e->v2->Pos.x);
	vmin.y = min(e->v1->Pos.y, e->v2->Pos.y);
	//
	if (p.x <= vmax.x && p.x >= vmin.x &&
		p.y <= vmax.y && p.y >= vmin.y) return true;

	return false;

	/*glm::vec2 v1 = e->v2->Pos-e->v1->Pos;
	glm::vec2 v2 = p-e->v1->Pos;
	//glm::abs(v1);
	if (glm::length(v2) <= glm::length(v1)) return true;
	return false;

	/*glm::vec2 v1 = glm::abs(e->v2->Pos-e->v1->Pos);
	float d = glm::distance(e->v1->Pos, p);
	//float d = sqrt(sqr(e->v1->Pos.x-p.x)+sqr(e->v1->Pos.y-p.y));
	if (d <= v1.x || d <= v1.y) return true;
	return false;*/
}

inline bool TriHaveV(PolygonT *tri, Vertex *v) {
	return  v == tri->Vertices[0] ||
			v == tri->Vertices[1] ||
			v == tri->Vertices[2];
}
//
bool FindTri (Edge *fe, Edge *&e1, Edge *&e2, EdgeList *el) {


	return false;

	/*	while(!finalEdges->isEmpty()) {,
		Edge *e = finalEdges->pop_back();

		for (int i = 0; i < finalEdges->size(); i++) {
			Edge *te = finalEdges->List[i];

			//if (te != e && te->HaveVertex(e-))
			if ((te!=e) && (te->HaveVertex(e->v1))) {
				for (int j = 0; j < finalEdges->size(); j++) {
					Edge *te1 = finalEdges->List[j];

					if((te1!=e)&&(te1!=te)&&(te1->IsConnectingEdge(e, te))) {
						PolygonT *p = new PolygonT (e, te, te1);
						Tris.push_back(p);
					}
				}

			}
		}*/
}
//
/*Edge *RotateEdge90 (Edge *e) {
	glm::vec2 org  = e->v1->Pos;
	glm::vec2 dest = e->v2->Pos;

	glm::vec2 m = 0.5f * (org+dest);
	glm::vec2 v = dest-org;
	glm::vec2 n(v.y, -v.x);

	org  = m-0.5f*n;
	dest = m+0.5f*n;

	return new Edge (org, dest);
}*/

void EditableObject::AddDebugTri(PolygonT *Tri) {
	if (Tri == 0) return;

	glm::vec2 v1 = Tri->Vertices[0]->Pos;
	glm::vec2 v2 = Tri->Vertices[1]->Pos;
	glm::vec2 v3 = Tri->Vertices[2]->Pos;
	//
	glm::vec2 cv = glm::vec2((v1.x+v2.x+v3.x)/3, (v1.y+v2.y+v3.y)/3);
	dpts.push_back(cv);
}

vector<PolygonT*> &EditableObject::SplitTri (int eidx, PolygonT *Tri, glm::vec2 &CrossPt) {
	PolygonT *nt1, *nt2;
	Vertex *v = new Vertex(CrossPt);
	Vertex *rv = 0;

	for (int i = 0; i < 3; i++) {
		if (!Tri->Edges->List[eidx]->HaveVertex(Tri->Vertices[i])) {
			rv = Tri->Vertices[i];
			dpts.push_back(rv->Pos);
			break;
		}
	}

	nt1 = new PolygonT(v->Pos, rv->Pos, Tri->Edges->List[eidx]->v1->Pos);
	nt2 = new PolygonT(v->Pos, rv->Pos, Tri->Edges->List[eidx]->v2->Pos);

	vector<PolygonT*> arr;
	arr.push_back(nt1);
	arr.push_back(nt2);

	return arr;

	/*dEdges.push_back(nt1->Edges->List[0]);
	dEdges.push_back(nt1->Edges->List[1]);
	dEdges.push_back(nt1->Edges->List[2]);

	dEdges.push_back(nt2->Edges->List[0]);
	dEdges.push_back(nt2->Edges->List[1]);
	dEdges.push_back(nt2->Edges->List[2]);*/

	/*for (int i = 0; i < 2; i++) {
		Vertex *pv = Tri->Vertices[i];

		Edge *e1 = new Edge(v, pv);
		Edge *e2 = new Edge(pv, pv->cw);
		Edge *e3 = new Edge(pv->cw, v);

		dEdges.push_back(e1);
		dEdges.push_back(e2);
		dEdges.push_back(e3);
	}*/

	//nt1 = new PolygonT();
	//nt2 = new PolygonT();
	/*Edge *se1 = new Edge(Tri->Edges->List[eidx]->v1->Pos, CrossPt);
	Edge *se2 = new Edge(CrossPt, Tri->Edges->List[eidx]->v2->Pos);

	switch (eidx) {
		case 0 :
			nt1 = new PolygonT(se1->v1, se1->v2, Tri->Edges->List[1]->v2);
			nt2 = new PolygonT(se2->v1, se2->v2, Tri->Edges->List[1]->v2);
			break;

		case 1 :
			nt1 = new PolygonT(Tri->Edges->List[0]->v1, se1->v1, se1->v2);
			nt2 = new PolygonT(Tri->Edges->List[0]->v1, se2->v1, se2->v2);
			break;

		case 2 :
			nt1 = new PolygonT(Tri->Edges->List[0]->v1, Tri->Edges->List[0]->v2, se1->v2);
			nt2 = new PolygonT(se1->v2, Tri->Edges->List[0]->v2, se1->v1);
			break;
	}*/
}

bool EditableObject::TrySplitTri (PolygonT *t) {
	int splitE = -1;
	int borderIdx = -1;
	vector<int> SplitEdges;
	vector<int> BoorderIdxs;

	for (int j = 0; j < Border.size(); j++) {
		if (t->Edges->List[0]->HaveVertices(Border[j]->v1, Border[j]->v2) == 0 && t->Edges->List[0]->Intersect(Border[j])) { splitE = 0; borderIdx = j; break; } else
		if (t->Edges->List[1]->HaveVertices(Border[j]->v1, Border[j]->v2) == 0 && t->Edges->List[1]->Intersect(Border[j])) { splitE = 1; borderIdx = j; break; } else
		if (t->Edges->List[2]->HaveVertices(Border[j]->v1, Border[j]->v2) == 0 && t->Edges->List[2]->Intersect(Border[j])) { splitE = 2; borderIdx = j; break; }
	}

	if (SplitEdges.empty()) return false;

	for (int i = 0; i < SplitEdges.size(); i++) {
		float dt;
		Edge *e = t->Edges->List[splitE];
	}

	//return false;
	if (splitE >= 0) {
		float dt = 0;
		Edge *e = t->Edges->List[splitE];
		e->Intersect(Border[borderIdx], dt);
		glm::vec2 cpt = e->Point(dt);
		dpts.push_back(cpt);

		Vertex *v = new Vertex(cpt);
		Vertex *rv = 0;

		for (int j = 0; j < 3; j++) {
			if (!t->Edges->List[splitE]->HaveVertex(t->Vertices[j])) {
				rv = t->Vertices[j];
				dpts.push_back(rv->Pos);
				break;
			}
		}

		PolygonT *nt1 = new PolygonT(v->Pos, rv->Pos, t->Edges->List[splitE]->v1->Pos);
		PolygonT *nt2 = new PolygonT(v->Pos, rv->Pos, t->Edges->List[splitE]->v2->Pos);
		
		Tris.push_back(nt1);
		Tris.push_back(nt2);

		return true;
	}

	return false;
}

void EditableObject::Triangulate5() {
	Vertex *v = Polygon->Vertices[0];
	dpts.clear();
	glm::vec2 vmax (v->Pos);
	glm::vec2 vmin (v->Pos);

	

	for (int i = 1; i < Polygon->Vertices.size(); i++) {
		v = Polygon->Vertices[i];
		vmax = max(vmax, v->Pos);
		vmin = min(vmin, v->Pos);
	}

	for (int i = 0; i < Polygon->Vertices.size(); i++) {
		Vertex *v1 = Polygon->Vertices[i];
		Vertex *v2 = v1->cw;
		Border.push_back(new Edge(v1, v2));
	}

	vmin -= 20;
	vmax += 20;

	PolygonT *p = new PolygonT();
	p->Insert(vmin);
	p->Insert(glm::vec2(vmin.x, vmax.y));
	p->Insert(vmax);
	p->Insert(glm::vec2(vmax.x, vmin.y));

	EdgeList *Frontier = new EdgeList();

	int n = Polygon->Vertices.size();
	EdgeList *Edges = new EdgeList();

	for (int ti = 0; ti < n/*Polygon->Vertices.size()*/; ti++) {
		v = Polygon->Vertices[ti];
		Circles.clear(); // debug
		dEdges.clear(); // debug
		Frontier->List.clear();
		Edges->List.clear();
		dpts.clear();
		dpts.push_back(v->Pos);

		// DELETE NO DELONE TRIS
		vector<PolygonT*>::iterator it = Tris.begin();
		while (it != Tris.end()) {
			PolygonT *t = *it;
			bool skip = false;

			/*for (int i = 0; i < Border.size(); i++) {
				if (t->Edges->List[0]->HaveVertices(Border[i]->v1, Border[i]->v2) == 0 && t->Edges->List[0]->Intersect(Border[i])) { t->NeedDel = true; Tris.erase(it); skip = true; break; } else
				if (t->Edges->List[1]->HaveVertices(Border[i]->v1, Border[i]->v2) == 0 && t->Edges->List[1]->Intersect(Border[i])) { t->NeedDel = true; Tris.erase(it); skip = true; break; } else
				if (t->Edges->List[2]->HaveVertices(Border[i]->v1, Border[i]->v2) == 0 && t->Edges->List[2]->Intersect(Border[i])) { t->NeedDel = true; Tris.erase(it); skip = true; break; }
			}*/

			if (skip) continue;

			glm::vec2 center;
			double r = TriCircle(t->Vertices[0]->Pos, t->Vertices[1]->Pos, t->Vertices[2]->Pos, &center);

			if (r >= 0.f) {
				double l = glm::length(v->Pos-center);
				//Circles.push_back(PolygonCircle(center, r));

				if (l <= r-0.1f) {
					
					t->NeedDel = true; // !TODO Free memory!-------------------------------------------------------------

					if (Edges->find(t->Edges->List[0]) < 0) Edges->push_back(t->Edges->List[0]);
					if (Edges->find(t->Edges->List[1]) < 0) Edges->push_back(t->Edges->List[1]);
					if (Edges->find(t->Edges->List[2]) < 0) Edges->push_back(t->Edges->List[2]);

					Tris.erase(it);
				} else it++;
			} else {
				if (PointInAngle(v->Pos, t->Vertices[0]->Pos, t->Vertices[1]->Pos, t->Vertices[2]->Pos, -r)) {
					t->NeedDel = true; // !TODO Free memory!-------------------------------------------------------------

					if (Edges->find(t->Edges->List[0]) < 0) Edges->push_back(t->Edges->List[0]);
					if (Edges->find(t->Edges->List[1]) < 0) Edges->push_back(t->Edges->List[1]);
					if (Edges->find(t->Edges->List[2]) < 0) Edges->push_back(t->Edges->List[2]);

					Tris.erase(it);
				} else it++;
			}
		}

		for (int i = 0; i < Edges->size(); i++) {
			Edge *e = Edges->List[i];
			bool nt1 = false;
			bool nt2 = false;

			for (int j = 0; j < Tris.size(); j++) {
				if (e->Tri1 == Tris[j]) e->Tri1 = 0;//nt1 = true;
				if (e->Tri2 == Tris[j]) e->Tri2 = 0; //nt2 = true;
			}

			nt1 = nt1 || e->Tri1 == 0;
			nt2 = nt2 || e->Tri2 == 0;
			//
			if (e->Tri2 == 0) {
				dEdges.push_back(e);
			}
		}


		// BUILD BORDER
		FrontPoints.clear();
		if (dEdges.size() >= 3) {
			p->Clear();
			Edge *lEdge = dEdges[0];
			p->Insert(lEdge->v1);
			p->Insert(lEdge->v2);
			//dpts.push_back(lEdge->v1->Pos);
			//dpts.push_back(lEdge->v2->Pos);

			FrontPoints.push_back(lEdge->v1->Pos);
			FrontPoints.push_back(lEdge->v2->Pos);

			dEdges.erase(dEdges.begin());
			bool skip;

			while (dEdges.size() > 1) {
				//LOG_DEBUG("LOOP 2!\n");
				skip = true;
				for (int i = 0; i < dEdges.size(); i++) {
					if (dEdges[i]->HaveVertex(lEdge->v2)) {
						lEdge = dEdges[i];
						p->Insert(lEdge->v2);
						//dpts.push_back(lEdge->v2->Pos);
						FrontPoints.push_back(lEdge->v2->Pos);
						dEdges.erase(dEdges.begin()+i);
						skip = false;
						break;
					}
				}

				if (skip) break;
			}

			for (int i = 0; i < dEdges.size()-1; i++) {
				p->Insert(dEdges[i]->v2);
			}

			/*for (int j = 1; j < dEdges.size(); j++) {
				//if (!dEdges[j]->HaveVertex(lEdge->v2)) continue;
				lEdge = dEdges[j];
				p->Insert(lEdge->v2);
				dpts.push_back(lEdge->v2->Pos);
			}*/
		}

		LOG_DEBUG("EDGES: %d\n", dEdges.size());
		LOG_DEBUG("POINTS: %d\n", dpts.size());
		
		// BUILD TRIS

		Edge     *FirstEdge = 0;
		Edge     *LastEdge = 0;
		PolygonT *FirstTri = 0;
		PolygonT *LastTri = 0;
		bool first = true;
		//
		//if (ti == n-1) break;
		for (int i = 0; i < p->Vertices.size(); i++) {
			Vertex *pv = p->Vertices[i];

			Edge *e1 = new Edge(v, pv);
			Edge *e2 = new Edge(pv, pv->cw);
			Edge *e3 = new Edge(pv->cw, v);

			PolygonT *Tri = new PolygonT(e1, e2, e3);
			Tris.push_back(Tri);

			/*if (first) {
				FirstTri  = Tri;
				FirstEdge = e1;
				LastEdge  = e3;
				LastEdge->Tri1 = Tri;
				FirstEdge->Tri1 = Tri;
				e2->Tri1 = Tri;

				first = false;
			} else {
				e1->Tri1 = Tri;
				e1->Tri2 = LastEdge->Tri1;
				LastEdge->Tri2 = Tri;
				e2->Tri1 = Tri;

				LastEdge = e3;
				LastEdge->Tri1 = Tri;
				LastTri = Tri;
			}*/
		}

		//FirstEdge->Tri2 = LastTri;
		//LastEdge ->Tri2 = FirstTri;
		//Edges->push_back(FirstEdge);

		for (int i = 0; i < Tris.size(); i++) {
			PolygonT *Tri1 = Tris[i];

			Tri1->Edges->List[0]->Tri1 = 0; Tri1->Edges->List[0]->Tri2 = 0;
			Tri1->Edges->List[1]->Tri1 = 0; Tri1->Edges->List[1]->Tri2 = 0;
			Tri1->Edges->List[2]->Tri1 = 0; Tri1->Edges->List[2]->Tri2 = 0;

			if (Tri1->Edges->List[0]->Tri1 == 0) Tri1->Edges->List[0]->Tri1 = Tri1;
			if (Tri1->Edges->List[1]->Tri1 == 0) Tri1->Edges->List[1]->Tri1 = Tri1;
			if (Tri1->Edges->List[2]->Tri1 == 0) Tri1->Edges->List[2]->Tri1 = Tri1;
		}

		for (int i = 0; i < Tris.size(); i++) {
			PolygonT *Tri1 = Tris[i];

			for (int j = 0; j < Tris.size(); j++) {
				PolygonT *Tri2 = Tris[j];

				if (Tri1->Edges->List[0]->Tri2 == 0 && Tri1->Edges->List[0]->Tri1 != Tri2 && Tri2->Edges->find(Tri1->Edges->List[0]) >= 0) Tri1->Edges->List[0]->Tri2 = Tri2;
				if (Tri1->Edges->List[1]->Tri2 == 0 && Tri1->Edges->List[1]->Tri1 != Tri2 && Tri2->Edges->find(Tri1->Edges->List[1]) >= 0) Tri1->Edges->List[1]->Tri2 = Tri2;
				if (Tri1->Edges->List[2]->Tri2 == 0 && Tri1->Edges->List[2]->Tri1 != Tri2 && Tri2->Edges->find(Tri1->Edges->List[2]) >= 0) Tri1->Edges->List[2]->Tri2 = Tri2;
			}
		}
		//
	}

	dpts.clear();
	dEdges.clear();

	// DELETE TRIS

	//for (int i = 0; i < 2; i++) {
		dpts.clear();
	vector<PolygonT*>::iterator it = Tris.begin();
	vector<PolygonT*> tTris = Tris;
	vector<int> delidx;
	//

	/*for (int i = 0; i < tTris.size(); i++) {
		PolygonT *t = tTris[i];
		if (TrySplitTri (t)) Tris.erase(Tris.begin()+i);
	}*/

	/*while (!tTris.empty()) {
		PolygonT *t = tTris.back();
		int splitE = -1;
		int borderIdx = -1;

		for (int j = 0; j < Border.size(); j++) {
			if (t->Edges->List[0]->HaveVertices(Border[j]->v1, Border[j]->v2) == 0 && t->Edges->List[0]->Intersect(Border[j])) { splitE = 0; borderIdx = j; break; } else
			if (t->Edges->List[1]->HaveVertices(Border[j]->v1, Border[j]->v2) == 0 && t->Edges->List[1]->Intersect(Border[j])) { splitE = 1; borderIdx = j; break; } else
			if (t->Edges->List[2]->HaveVertices(Border[j]->v1, Border[j]->v2) == 0 && t->Edges->List[2]->Intersect(Border[j])) { splitE = 2; borderIdx = j; break; }
		}

		if (splitE >= 0) {
			float dt = 0;
			Edge *e = t->Edges->List[splitE];
			e->Intersect(Border[borderIdx], dt);
			glm::vec2 cpt = e->Point(dt);
			dpts.push_back(cpt);

			Vertex *v = new Vertex(cpt);
			Vertex *rv = 0;

			for (int j = 0; j < 3; j++) {
				if (!t->Edges->List[splitE]->HaveVertex(t->Vertices[j])) {
					rv = t->Vertices[j];
					dpts.push_back(rv->Pos);
					break;
				}
			}

			PolygonT *nt1 = new PolygonT(v->Pos, rv->Pos, t->Edges->List[splitE]->v1->Pos);
			PolygonT *nt2 = new PolygonT(v->Pos, rv->Pos, t->Edges->List[splitE]->v2->Pos);
			
			Tris.push_back(nt1);
			Tris.push_back(nt2);

			//delete t;
			//AddDebugTri(t);
			//continue;
			
		}

		//tTris.erase(tTris.begin());
		tTris.pop_back();
	}*/

	//while (it != Tris.end()) {
	/*for (int i = 0; i < Tris.size(); i++) {
		PolygonT *t = Tris[i];
		int splitE = -1;
		int borderIdx = -1;

		//while (TriNeedSplit(t, Border, splitE, borderIdx)) {
		for (int j = 0; j < Border.size(); j++) {
			if (t->Edges->List[0]->HaveVertices(Border[j]->v1, Border[j]->v2) == 0 && t->Edges->List[0]->Intersect(Border[j])) { splitE = 0; borderIdx = j; break; } else
			if (t->Edges->List[1]->HaveVertices(Border[j]->v1, Border[j]->v2) == 0 && t->Edges->List[1]->Intersect(Border[j])) { splitE = 1; borderIdx = j; break; } else
			if (t->Edges->List[2]->HaveVertices(Border[j]->v1, Border[j]->v2) == 0 && t->Edges->List[2]->Intersect(Border[j])) { splitE = 2; borderIdx = j; break; }
		}

		if (splitE >= 0) {
			float dt = 0;
			Edge *e = t->Edges->List[splitE];
			e->Intersect(Border[borderIdx], dt);
			glm::vec2 cpt = e->Point(dt);
			dpts.push_back(cpt);

			Vertex *v = new Vertex(cpt);
			Vertex *rv = 0;

			for (int j = 0; j < 3; j++) {
				if (!t->Edges->List[splitE]->HaveVertex(t->Vertices[j])) {
					rv = t->Vertices[j];
					dpts.push_back(rv->Pos);
					break;
				}
			}

			PolygonT *nt1 = new PolygonT(v->Pos, rv->Pos, t->Edges->List[splitE]->v1->Pos);
			PolygonT *nt2 = new PolygonT(v->Pos, rv->Pos, t->Edges->List[splitE]->v2->Pos);
			
			//Tris.erase(it);
			delidx.push_back(i);
			Tris.push_back(nt1);
			Tris.push_back(nt2);

			dEdges.push_back(nt1->Edges->List[0]);
			dEdges.push_back(nt1->Edges->List[1]);
			dEdges.push_back(nt1->Edges->List[2]);

			dEdges.push_back(nt2->Edges->List[0]);
			dEdges.push_back(nt2->Edges->List[1]);
			dEdges.push_back(nt2->Edges->List[2]);
			LOG_DEBUG("ADDED\n");

			//delete t;
			//AddDebugTri(t);
			//continue;
			
		}
		//}

		//it++;
	}*/

	//for (int j = 0; j < newTris.size(); j++) {
	//	Tris.push_back(newTris[j]);
	//}
	//newTris.clear();
	//}

	it = Tris.begin();
	//vector<PolygonT*>::iterator it = Tris.begin();
	
	// !!!UNCOMENT!!!
	/*while (it != Tris.end()) {
		PolygonT *t = *it;
		bool skip = false;

		glm::vec2 v1 = t->Vertices[0]->Pos;
		glm::vec2 v2 = t->Vertices[1]->Pos;
		glm::vec2 v3 = t->Vertices[2]->Pos;
		//
		glm::vec2 cpt = glm::vec2((v1.x+v2.x+v3.x)/3, (v1.y+v2.y+v3.y)/3);

		if (!PointInPolygon(cpt, Polygon)) {
			Tris.erase(it);
			skip = true;
		}

		if (!skip) it++;
	}*/

	LOG_DEBUG("TRIS: %d\n", Tris.size());

	/*dEdges.clear();
	dEdges.push_back(new Edge(glm::vec2(100, 100), glm::vec2(50, 50)));
	dEdges.push_back(new Edge(glm::vec2(100, 100), glm::vec2(150,50)));

	dpts.clear();
	dpts.push_back(glm::vec2(100, 80));*/

	glm::vec2 v1 = glm::vec2(50 , 50)-glm::vec2(100, 100);
	glm::vec2 v2 = glm::vec2(150, 50)-glm::vec2(100, 100);
	float ang = glm::orientedAngle(v1, v2);

	LOG_DEBUG("ANGLE : %f\n", ang);
	if (PointInAngle(dpts[0], glm::vec2(100 , 100), glm::vec2(50 , 50), glm::vec2(150, 50), ang)) LOG_DEBUG("POINT IN ANGLE\n");
}

void EditableObject::Triangulate4() {

}

void EditableObject::Triangulate3() {

}
//
void EditableObject::Triangulate2() {
	EdgeList *el = new EdgeList();
	glm::vec2 vmin(666666.0f);

	vector<Vertex*> pts = Polygon->Vertices;

	for (int i = 0; i < pts.size(); i++) {
		Vertex *v1 = pts[i];
		Vertex *v2 = v1->cw;

		vmin.x = min (vmin.x, v1->Pos.x, v2->Pos.x);
		vmin.y = min (vmin.y, v1->Pos.y, v2->Pos.y);
	}

	pts.insert(pts.begin(), new Vertex (glm::vec2(vmin.x, vmin.y-10)));
	pts.insert(pts.begin(), new Vertex (glm::vec2(vmin.x-10, vmin.y-10)));

	for (int i = 0; i < pts.size(); i++) {
		Vertex *v1 = pts[i];

		for (int j = 0; j < pts.size(); j++) {
			if (j == i) continue;
			Vertex *v2 = pts[j];
			el->push_back(new Edge(v1, v2));
		}
	}

	Edge *SE = new Edge(pts[0], pts[1]);
	SE->Type = efront;
	EdgeList *FrontsE = new EdgeList();
	FrontsE->push_back(SE);

	el->sort();
	EdgeList *finalEdges = new EdgeList();

	for (int i = 0; i < Polygon->Vertices.size(); i++) {
		Vertex *v1 = Polygon->Vertices[i];
		Vertex *v2 = v1->cw;
		finalEdges->push_back(new Edge(v1, v2));
	}

	/*for (int i = 0; i < el->size(); i++) {
		Edge *e = el->List[i];
		bool hi = false;

		if (finalEdges->find(e) < 0) {
			//
			for (int j = 0; j < finalEdges->size(); j++) {
				Edge *fe = finalEdges->List[j];
				float t = 0.1f;
				bool isec = fe->Intersect(e, t);
				glm::vec2 cpt = fe->Point(t);
				Edge re = fe->Rot();
				float t2 = 0.0f;
				fe->Intersect(&re, t2);
				int l1 = trunc(glm::distance(cpt, fe->v1->Pos));
				int l2 = trunc(fe->Length());
				bool isec2 = false;
				isec2 = l1 <= l2;

				//if (fe->HaveVertices(e->v1, e->v2) == 0 && fe->Intersect(e))
				//	hi = true;

				//LOG_DEBUG("dist %d, len %de\n", l1, l2);
				//if (l1 > l2) { LOG_DEBUG("INTERSECT! dist %d, len %d\n", l1, l2); hi = true; break; }
			}
			//exit(0);
			if (!hi) { finalEdges->push_back(e); LOG_DEBUG("ADDED!\n"); }
		}
	}*/
	bool hi = false;
	for (int i = 0; i < el->size(); i++) {
		Edge *e = el->List[i];
		if (finalEdges->find(e) >= 0) continue;

		for (int j = 0; j < finalEdges->size(); j++) {
				Edge *fe = finalEdges->List[j];

				float t = 0.0f;
				float b = 0.0f;
				bool isec = e->Intersect(e, t);

				glm::vec2 cpt = fe->Point(t);

				float l1 = glm::distance(cpt, fe->v1->Pos);
				float l2 = fe->Length();

				glm::vec2 v1 = e->v2->Pos-e->v1->Pos;
				glm::vec2 v2 = cpt-e->v1->Pos;
				//glm::abs(v1);
				//if (glm::length(v2) <= glm::length(v1)) return true;
				//return false;
				//l1 = glm::length(v2);
				//l2 = glm::length(v1);

				if (fe->HaveVertices(e->v1, e->v2) == 0 && fe->Intersect(e)) {
					//LOG_DEBUG("dist %f, len %f\n", l1, l2);
					hi = true;
					break;
				}

				/*float r;
				float s;
				//
				bool isec = fe->Intersect (e , r);
				e ->Intersect (fe, s);

				glm::vec2 cpt = fe->Point(r);

				float l1 = glm::distance(cpt, fe->v1->Pos);
				float l2 = fe->Length();

				if (l1 < l2) {
					if (fe->HaveVertices(e->v1, e->v2) == 0 && isec)
					hi = true;
				}*/
		}
		//exit(0);
		if (!hi) finalEdges->push_back(e);
		hi = false;
	}

	// BUILD TRIANGLES
	LOG_DEBUG("EDGE COUNT: %d\n", finalEdges->size());
	Tris.clear();

	//while (!FrontsE->isEmpty()) {
	for (int k = 0; k < finalEdges->size(); k++) {
		//Edge *te0 = FrontsE->pop_back();
		Edge *te0 = finalEdges->List[k];
		//
		for (int i = 0; i < finalEdges->size(); i++) {
			Edge *te1 = finalEdges->List[i];

			if (te1 != te0 /*&& te1->Type != edead*/ && te1->HaveVertex(te0->v1)) {
				for (int j = 0; j < finalEdges->size(); j++) {
					Edge *te2 = finalEdges->List[j];

					if (te2 != te0 /*&& te2->Type != edead*/ && te2 != te1 && te2->IsConnectingEdge(te0, te1)) {
						PolygonT *p = new PolygonT (te0, te1, te2);
						Tris.push_back(p);

						if (te1->Type == efront) te1->Type = edead;
						else { te1->Type = efront; FrontsE->push_back(te1); }

						if (te2->Type == efront) te2->Type = edead;
						else { te2->Type = efront; FrontsE->push_back(te2); }
					}
				}
			}
		}

		te0->Type = edead;
	}

	// Final Edges
	for (int i = 0; i < finalEdges->size(); i++) {
		PolygonT *p = new PolygonT();
		p->Insert(finalEdges->List[i]->v1); p->Insert(finalEdges->List[i]->v2);
		//Tris.push_back(p);
	}
	LOG_DEBUG("TRIS: %d\n", Tris.size());
}
//
void EditableObject::Triangulate() {

}

int Q(glm::vec2 t1, glm::vec2 t2) {
//float Q =  x(y2-y1)+y(x1-x2)+y1*x2-x1*y2;
	float Q = t2.y-t1.y+t1.x-t2.x+t1.y*t2.x-t1.x*t2.y;
	if (Q > 0) return 1;
	if (Q < 0) return -1;
	return 0;
}

int Q(Edge *e) {
	return Q(e->v1->Pos, e->v2->Pos);
}


bool isClockWise(glm::vec2 A, glm::vec2 B, glm::vec2 C) {
	return (B.x - A.x) * (C.y - A.y) - (C.x - A.x) * (B.y - A.y) < 0;
}

int inAngle (glm::vec2 D, glm::vec2 A, glm::vec2 B, glm::vec2 C)
{
  /* Случай, когда угол BCD < 180º. */
  if (isClockWise (B, C, D))
  /* Достаточно, чтобы оба угла BCA и ACD   *
   * были < 180º                    */
  /* (при проходе по часовой стрелке). */
    return (isClockWise (B, C, A) && isClockWise (A, C, D));
  else
  /* Иначе – достаточно, чтобы один из углов BCA *
   * и ACD был < 180º (при проходе по    *
   * часовой стрелке).                           */
    return (isClockWise (B, C, A) || isClockWise (A, C, D));
}

Vertex *EditableObject::WiderAngleToEdge (Edge *e, vector<Vertex*> *pts, EdgeList *cont) {
	float minAng = 666;
	Vertex *resPt = 0;
	vector<Vertex*>::iterator fit = pts->begin();

	for (int i = 0; i < pts->size(); i++) {
	//for (vector<Vertex*>::iterator it = pts->begin(); it != pts->end(); it++) {
		Vertex *pt = (*pts)[i];
		//if (pt->Classify(e->v1, e->v2) != classify::left) continue;
		bool pintri = false;

		Edge *ce1 = new Edge(pt, e->v1);
		Edge *ce2 = new Edge(pt, e->v2);

		int ie1 = cont->find(new Edge(pt, e->v1));
		int ie2 = cont->find(new Edge(pt, e->v2));

		if (ie1 >= 0)
			if (cont->List[ie1]->Type == edead) continue;

		if (ie2 >= 0)
			if (cont->List[ie2]->Type == edead) continue;

		if (e->v1 != pt && e->v2 != pt)
			if (e->Dist2(pt->Pos) > 0) {
				//if (!isecWContours(e, pt, cont)) {
				bool isec = false;

				for (int i = 0; i < cont->size(); i++) {
					Edge *fe = cont->List[i];
					//
					if (fe->HaveVertices(ce1->v1, ce1->v2) == 0 && fe->Intersect(ce1)) {
						isec = true;
						break;
					}

					if (fe->HaveVertices(ce2->v1, ce2->v2) == 0 && fe->Intersect(ce2)) {
						isec = true;
						break;
					}
				}
				//
				if (!isec) {
					float ang = e->Angle(pt->Pos);

					if (ang < minAng) {
						minAng = ang;
						resPt = pt;
						//fit = it;
					}
				}

			}

			//
			//delete ce1;
			//delete ce2;
			//delete tp;
	}

	//if (resPt) pts->erase(fit);
	return resPt;
}

// <------------------------------- Editable Object

// Polygon Object -------------------------------->

void PolygonObject::EmptyRender() {
	/*if (NeedUpdate || mCamera->NeedUpdate)
		UpdateModelMatrix();

	glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(MVPMatrix[0][0]));*/

	glEnable (GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0, 1); 
	glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT); 
	glColorMask(false, false, false, false);

	glColor3f(1.f, 1.f, 1.f);
	//glBegin2D();
	glBegin(GL_TRIANGLE_FAN);

		for (int i = 0; i < Polygon->Vertices.size(); i++) {
			glVertex2f(Polygon->Vertices[i]->Pos.x, Polygon->Vertices[i]->Pos.y);
		}

	glEnd();

	glStencilFunc(GL_NOTEQUAL, 0, 1); 
	glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO); 
	glColorMask(true, true, true, true);

	int x0 = mCamera->Position.x;
	int y0 = mCamera->Position.y;

	glBegin (GL_QUADS);
		glVertex2f (x0, y0+WindowHeight-Offset.y);
		glVertex2f (x0+WindowWidth, y0+WindowHeight-Offset.y);
		glVertex2f (x0+WindowWidth, y0-Offset.y);
		glVertex2f (x0, y0-Offset.y);
	glEnd();

	glDisable (GL_STENCIL_TEST);
	//glEnd2D();
}

void PolygonObject::DrawVertices() {
	for (int i = 0; i < VertexAbsPos.size(); i++) {		
		glm::vec2 vp = glm::vec2 (VertexAbsPos[i].x, VertexAbsPos[i].y);
		MapEditor->VertexObject->SetPosition(vp);
		MapEditor->VertexObject->BaseRender();
	}
}

void PolygonObject::DrawEdges (const bool Selected) {
	glBegin2D();
		glColor3fv(&MapEditor->SelectColor[0]);
		glBegin(GL_LINE_LOOP);

			for (int i = 0; i < VertexAbsPos.size(); i++)
				glVertex2f(VertexAbsPos[i].x-mCamera->Position.x, VertexAbsPos[i].y-mCamera->Position.y);

		glEnd();
	glEnd2D();
}


void PolygonObject::UpdatePosVertices() {
	for (int i = 0; i < VertexAbsPos.size(); i++) {
		Vertex *v = Polygon->Vertices[i];
		glm::vec2 VPos = glm::vec2 (v->Pos.x*Size.x, v->Pos.y*Size.y);

		glm::mat4 mt = glm::translate (glm::mat4(1.0f), glm::vec3(Position+Offset, 0.0f));
		glm::mat4 mr = glm::rotate    (glm::mat4(1.0f), Rotation, glm::vec3(0,0,1));
		glm::mat4 ms = glm::scale	  (glm::mat4(1.0f), glm::vec3(Core->VertexSize[0], Core->VertexSize[1], 1.0f));

		glm::mat4 mtm = glm::translate (glm::mat4(1.0f), glm::vec3(VPos, 0.0f));
		//
		ModelMatrix = mt*mr*mtm*ms;
		MVPMatrix = mCamera->MVPMatrix*ModelMatrix;

		glm::vec2 finalVertPos;
		finalVertPos[0] = ModelMatrix[3][0];
		finalVertPos[1] = ModelMatrix[3][1];
		
		VertexAbsPos[i] = glm::vec4 (finalVertPos.x, finalVertPos.y, 0, 0);
		SelectPoly->Vertices[i]->Pos = finalVertPos;
	}
}

void PolygonObject::UpdateListVertices() {
	VertexAbsPos.clear();
	SelectPoly->Clear();

	for (int i = 0; i < Polygon->Vertices.size(); i++) {
		Vertex *v = Polygon->Vertices[i];
		glm::vec2 VPos = glm::vec2 (v->Pos.x*Size.x, v->Pos.y*Size.y);

		glm::mat4 mt = glm::translate (glm::mat4(1.0f), glm::vec3(Position+Offset, 0.0f));
		glm::mat4 mr = glm::rotate    (glm::mat4(1.0f), Rotation, glm::vec3(0,0,1));
		glm::mat4 ms = glm::scale	  (glm::mat4(1.0f), glm::vec3(Core->VertexSize[0], Core->VertexSize[1], 1.0f));

		glm::mat4 mtm = glm::translate (glm::mat4(1.0f), glm::vec3(VPos, 0.0f));
		//
		ModelMatrix = mt*mr*mtm*ms;
		MVPMatrix = mCamera->MVPMatrix*ModelMatrix;

		glm::vec2 finalVertPos;
		finalVertPos[0] = ModelMatrix[3][0];
		finalVertPos[1] = ModelMatrix[3][1];
		VertexAbsPos.push_back (glm::vec4 (finalVertPos.x, finalVertPos.y, 0, 0));
		SelectPoly->Insert(finalVertPos);
	}
}


// <-------------------------------- Polygon Object

#endif

