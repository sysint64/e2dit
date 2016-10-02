#include <GL/glew.h>
#include <utility/renderer.h>
#include "scene.h"


void Scene::render() {
	glBegin2D();
	glBegin(GL_LINE_LOOP);
	glColor3f(0.f, 0.8f, 0.f);

	glVertex2i(32, 32);
	glVertex2i(32, 768);
	glVertex2i(1024, 768);
	glVertex2i(1024, 32);

	glEnd();
	glEnd2D();
//	for (const auto &object : objects) {
//		object->render();
//	}
}
