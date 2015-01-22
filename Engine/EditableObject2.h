#ifndef H_EDITABLEOBJECT
#define H_EDITABLEOBJECT
#define DBL_MAX 0;
//
namespace traverse {
	enum { closwise, counterCloswise };
}

namespace classify {
	enum {left, right, beyond, behind, between, origin, destination};
	enum {inside, outside};
	enum {touching, crossing, inessential};
	enum {collinear, parallel, skew, skewCross, skewNoCross};
	enum {upper, lower};
}

//
class Vertex {
public:
	Vertex *cw;  // next
	Vertex *ccw; // prev
	glm::vec2 Point;
	glm::vec2 TexCoord;

	Vertex (glm::vec2 p) { Point = p; cw = this; ccw = this; }
	
	inline Vertex *Neighbor(const int rotation) {
		return ((rotation == traverse::closwise) ? cw : ccw);
	}

	inline Vertex *Insert(Vertex *v);
	inline Vertex *Remove();
	inline void Splice(Vertex *b);
	inline Vertex* Split(Vertex *b);
};

class Edge;
class PolygonT {
private:
	Vertex *_v;
	void Resize() {
		if (_v == 0) {
			size = 0;
		} else {
			Vertex *vv = _v->cw;
			for (size = 1; vv != _v; ++size, vv = vv->cw);
		}
	}

public:
	int		size;

	PolygonT() : _v(0), size(0) {}
	PolygonT (PolygonT &p);
	PolygonT (Vertex *vv) : _v(vv) { Resize(); }
	~PolygonT();

	inline Edge edge();
	inline glm::vec2 Point() { return _v->Point; }
	inline Vertex *cw()      { return _v->cw;    }
	inline Vertex *ccw()     { return _v->ccw;   }

	inline Vertex *Advance (const int rotation) {
		return _v = _v->Neighbor(rotation);
	}

	inline Vertex *Neighbor (const int rotation) {
		return _v->Neighbor(rotation);
	}

	inline Vertex *Insert (const glm::vec2 p);
	inline void Remove();
	inline PolygonT *Split (Vertex *b);
	inline Vertex *v() { return _v; }
	inline void SetV(Vertex *v) { _v = v; }
};



class Edge {
public:
	glm::vec2 org;
	glm::vec2 dest;

	Edge (const glm::vec2 &_org, const glm::vec2 &_dest)
		: org(_org), dest(_dest) {}

	Edge() : org(glm::vec2(0, 0)), dest(glm::vec2(1.0f, 0)) {}

	inline Edge &Rotate();
	inline Edge &Flip() { return Rotate().Rotate(); }
	inline glm::vec2 Point(float t) { return org+t*(dest-org); }

	int Intersect (Edge e, float &t);
	int Cross (Edge &e, float &t);

	inline bool IsVertical() {
		return org.x == dest.x;
	}

	inline float Slope() {
		if (org.x != dest.x)
			return (dest.y-org.y) / (dest.x-org.x);

		return DBL_MAX;
	}

	inline float y (float x) {
		return Slope()*(x-org.x)+org.y;
	}
};

//
class EditableObject : public BaseObject {
private:
	PolygonT *Polygon;
	vector<PolygonT*> Polygons;

	void TriangulateFanPoly(PolygonT &p, vector<PolygonT*> *tris);

public:
	EditableObject() {
		Polygon = new PolygonT();
	}

	void Triangulate(); // -
	void Split(glm::vec2 &e); // -
	void Cut(Edge e); // +
	void Divide(int VertN, int HorizN); // -
	void Partition(glm::vec2 &PartEdge); // -
	void BuildDataRender(); // -
	void DebugRender(glm::vec2 pos); // +
	void CopyFromDataRender(); // +
};

#include "EditableObject.cpp"
#endif
