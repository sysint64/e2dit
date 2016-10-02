#include "mapeditor.h"


void MapEditor::render() {
	scene->render();
}

void MapEditor::step() {

}

MapEditor *MapEditor::getInstance() {
	static MapEditor *editor = nullptr;

	if (editor != nullptr)
		return editor;

	return new MapEditor();
}

void MapEditor::initUI() {
	rootPane = (UIPanel*) uiManager->findElement("rootPane");
	rootPane->onFocus = [this](UIElement *el) { activePane = (UIPanel*) el; };
	activePane = rootPane;
}

void MapEditor::splitView(Orientation orientation) {
	std::unique_ptr<Pane> pane = std::make_unique<Pane>(uiManager, orientation);
	pane->onFocus = [this](UIElement *el) { activePane = (UIPanel*) el; };
	auto panePtr = pane.get();

	if (activePane != nullptr)
	activePane->addElement(std::move(pane));
	activePane = panePtr;
}
