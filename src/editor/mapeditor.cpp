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
	std::unique_ptr<Pane> pane = std::make_unique<Pane>(uiManager);
	pane->align = ui::Align::Client;
	pane->onFocus = [this](ui::Widget *el) { activePane = (ui::Panel*) el; };
	rootPane = pane.get();
	uiManager->addElement(std::move(pane));
	activePane = rootPane;
}

void MapEditor::splitView(ui::Orientation orientation) {
	std::unique_ptr<Pane> pane = std::make_unique<Pane>(uiManager, orientation);
	pane->onFocus = [this](ui::Widget *el) { activePane = (ui::Panel*) el; };
	auto panePtr = pane.get();

	assert(activePane != nullptr);
	activePane->addElement(std::move(pane));
	activePane = panePtr;
}
