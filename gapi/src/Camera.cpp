#ifndef CPP_CAMERA
#define CPP_CAMERA

//
inline Camera2D::Camera2D() {
	SetPosition(glm::vec2(0, 0));
	NeedUpdate = true;
	Zoom = 1.f;
}

//
inline void Camera2D::SetPosition(glm::vec2 vec) {
	Position = vec;
	NeedUpdate = true;
}

inline void Camera2D::SetZoom (float zoom) {
	Zoom = zoom;
	//NeedUpdate = true;
}

//
inline void Camera2D::UpdateMatrices() {
	ViewMatrix = glm::lookAt(glm::vec3(Position, 1), glm::vec3(Position, 0), glm::vec3(0, 1, 0));
	ProjectionMatrix = glm::ortho(0.0f, (float)ScreenWidth, 0.0f, (float)ScreenHeight/*, -1.0f, 10.0f*/);
	glm::mat4 sm = glm::scale (glm::mat4(1.0f), glm::vec3(Zoom, Zoom, 1.0f));
	//
	MVPMatrix = ProjectionMatrix*sm*ViewMatrix; // Result Matrix
	//NeedUpdate = false;
}

inline void Camera2D::Update() {
	//
	if (NeedUpdate)
		UpdateMatrices();
}

#endif
