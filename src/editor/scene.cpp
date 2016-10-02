#include "scene.h"


void Scene::render() {
	for (const auto &object : objects) {
		object->render();
	}
}
