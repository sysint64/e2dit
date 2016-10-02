#pragma once


#include "scene.h"


class MapEditor {
public:
	std::unique_ptr<Scene> scene;

	virtual void render();
	static MapEditor *getInstance();

	MapEditor() {
		scene = std::make_unique<Scene>();
	}
};
