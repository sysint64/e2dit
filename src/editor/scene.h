#pragma once

#include <vector>
#include <memory>
#include "scene/sceneobject.h"


class Scene {
public:
	std::vector<std::unique_ptr<SceneObject>> objects;
	std::array<int, 4> bound;

	virtual void render();
};
