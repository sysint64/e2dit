#ifndef H_GAMEOBJECTS
#define H_GAMEOBJECTS
//
class BaseGameObject : public EditableObject {
public:
	glm::vec2 TexOffset;
	glm::vec2 TexSize;

	virtual void DrawVertices();
	virtual void DrawEdges (const bool Selected = false);
};

#include "EditableObject.h"

#endif
