#ifndef H_CAMERA
#define H_CAMERA

//
class Camera2D {
public:
	float Zoom;
	bool NeedUpdate;
	glm::vec2 Position;
	//
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::mat4 MVPMatrix;
	//
	glm::ivec2 Size;
	wstring Name;
	//
	inline Camera2D();
	inline void SetPosition(glm::vec2 vec);
	inline void SetZoom (float zoom);
	inline void UpdateMatrices();
	inline void Update();
};

#endif
