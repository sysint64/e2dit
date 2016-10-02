#include "mapeditor.h"


void MapEditor::render() {
	scene->render();
}

MapEditor *MapEditor::getInstance() {
	static MapEditor *editor = nullptr;

	if (editor != nullptr)
		return editor;

	return new MapEditor();
}
