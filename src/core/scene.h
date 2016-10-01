#pragma once

#include <vector>
#include <memory>
#include "scene/sceneobject.h"


class Scene {
public:
	std::vector<std::unique_ptr<SceneObject>> objects;
};
