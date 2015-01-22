//
#ifndef H_BASEOBJECT
#define H_BASEOBJECT
//
using namespace std;
//
#pragma pack(1)

// for OpenGL 4.3
struct tVertex {
	vector<glm::vec2>	VertexBuffer;
	vector<glm::vec2>	TexCoordsBuffer;
	vector<GLuint>		Indices;
};

#pragma pack(0)
//
class DataRender {
private:
	GLuint vId, tId, iId;
	GLuint VAO;
	bool fInit;

public:
	//glm::vec2	*VertexBuffer;
	//glm::vec2	*TexCoordsBuffer;
	//GLuint		*Indices;
	vector <glm::vec2> VertexBuffer;
	vector <glm::vec2> TexCoordsBuffer;
	vector <GLuint>    Indices;
	//
	bool Static;
	int vCount, iCount;
	GLuint RenderMode;
	//
	void CreateVBO (const bool Static = true);
	void CreateVAO_21(); // for OpenGL 2.1
	void CreateVAO_33(); // for OpenGL 3.3
	void CreateVAO_43(); // for OpenGL 4.3
	void RenderVBO();
	void SetAttrs();
	virtual void Render();
	//
	DataRender();
	~DataRender();
	//
	inline void AddVertex(glm::vec2 Vertex, glm::vec2 TexCoord) {
		//
		VertexBuffer.push_back(Vertex);
		TexCoordsBuffer.push_back(TexCoord);
		//VertexBuffer[vCount]    = Vertex;
		//TexCoordsBuffer[vCount] = TexCoord;
		
		vCount++;
	}
	//
	inline void AddIndex(GLuint Index) {
		//
		Indices.push_back(Index);
		//Indices[iCount] = Index;
		iCount++;
	}
	//
};

//
class BaseObject {
public:
	//
	DataRender	*Data;
	//
	//
	bool NeedUpdate;
	glm::mat4 ModelMatrix;
	glm::mat4 MVPMatrix; // Model View Projection Matrix
	//
	int vCount, iCount;
	bool Visible;
	//
	GLuint		 RenderMode;
	BaseShader	*Shader;
	Texture2D	*Texture;
	Camera2D	*mCamera;
	//
	unsigned char Type;
	//
	glm::vec2 Position;
	glm::vec2 Offset;
	glm::vec2 Size;
	glm::vec2 tSize;
	glm::vec2 tOffset;
	glm::vec2 Pivot;
	float Rotation;
	//
	virtual inline void Render();
	virtual inline void BaseRender();
	virtual inline void EmptyRender();
	inline void Init();
	inline void Free();
	void Step(const float deltaTime);
	//
	BaseObject() {
		Shader = 0;
		Rotation = 0;
	}
	//
	inline glm::vec2 WorldToScreen() {
		return Position-mCamera->Position+Offset;
	}

	inline glm::vec2 WorldToScreen(glm::vec2 ScreenPos) {
		return ScreenPos-mCamera->Position+Offset;
	}
	//
	inline void UpdateModelMatrix();
	inline void SetPosition(glm::vec2 pos) {
		//
		if (Position[0] == pos[0] &&
			Position[1] == pos[1]) return;
		//
		Position = pos;
		NeedUpdate = true;
	}

	inline void SetOffset(glm::vec2 offset) {
		//
		if (Offset[0] == offset[0] &&
			Offset[1] == offset[1]) return;
		//
		Offset = offset;
		NeedUpdate = true;
	}

	//
	inline void SetSize(glm::vec2 size) {
		//
		if (Size[0] == size[0] &&
			Size[1] == size[1]) return;
		//
		Size = size;
		NeedUpdate = true;
	}
	
	//
	inline void SetRotate(float angle) {
		//
		if (Rotation == angle) return;
		///
		Rotation = angle;
		NeedUpdate = true;
	}
};

//
class AnimatedObject : public BaseObject {
public:
	inline void Render();
};

//
class Surface : public BaseObject {
public:
	//
	DataRender	*Quad;
	glm::vec3	 Color;
	//
	inline void Render();
	inline void Init() { BaseObject::Init(); Type = 1; }
};

#endif
//