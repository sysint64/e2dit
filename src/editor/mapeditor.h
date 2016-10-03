#pragma once

#include <ui/panel.h>
#include <ui/all.h>
#include "scene.h"
#include "pane.h"


class MapEditor {
public:
	std::unique_ptr<Scene> scene;
	ui::Panel *rootPane;
	ui::Panel *activePane;
	std::vector<Pane*> panes;
	ui::Manager *uiManager = ui::Manager::getInstance();

	virtual void render();
	virtual void step();

	static MapEditor *getInstance();

	MapEditor() {
		scene = std::make_unique<Scene>();
	}

	void initUI();
	void splitView(ui::Orientation orientation);
};
