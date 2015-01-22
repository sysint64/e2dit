#ifndef H_DEBUG_DRAW
#define H_DEBUG_DRAW

Camera2D *DebugCamera;

struct DebugPoint {
	float x;
	float y;
	glm::vec3 Color;
};

struct DebugLine {
	float x1, y1;
	float x2, y2;
	glm::vec3 Color;
};

struct DebugCircle {
	float r;
	float x, y;
	glm::vec3 Color;
};

vector<DebugPoint>  DebugPoints;
vector<DebugLine>   DebugLines;
vector<DebugCircle> DebugCircles; // r, x, y

void DebugDraw();
void AddDebugLine   (float x1, float y1, float x2, float y2, glm::vec3 Color = glm::vec3 (1, 0, 0));
void AddDebugCircle (float x , float y , float r ,           glm::vec3 Color = glm::vec3 (1, 0, 0));
void AddDebugPoint  (float x , float y ,                     glm::vec3 Color = glm::vec3 (1, 0, 0));

#endif