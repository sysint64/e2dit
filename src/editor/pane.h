#pragma once


#include <ui/panel.h>
#include <ui/all.h>

class Pane : public ui::UIPanel {
private:
	void initParams() {
		background = ui::UIPanel::Background::Transparent;
		blackSplit = true;
		showSplit = true;
		allowResize = true;
		minSize = 200;
		allowScroll = false;
	}
public:
	ui::Orientation orientation;
	float percent = 50.f;

	Pane(ui::UIManager *manager) : ui::UIPanel(manager) {
		initParams();
	}

	Pane(ui::UIManager *manager, ui::Orientation orientation) : ui::UIPanel(manager) {
		initParams();
		this->orientation = orientation;

		switch (orientation) {
			case ui::Orientation::Horizontal:
				align = ui::Align::Left;
				break;

			case ui::Orientation::Vertical:
				align = ui::Align::Top;
		}
	}

	inline void calcSize() {
		switch (orientation) {
			case ui::Orientation::Horizontal:
				width = static_cast<int>(static_cast<float>(parent->width)*(percent/100.f));

				if (width < minSize)
					width = minSize;

				break;

			case ui::Orientation::Vertical:
				height = static_cast<int>(static_cast<float>(parent->height)*(percent/100.f));

				if (height < minSize)
					height = minSize;

				break;
		}
	}

	inline  void getPercent() {
		switch (orientation) {
			case ui::Orientation::Horizontal:
				percent = width*100.f/parent->width;
				break;

			case ui::Orientation::Vertical:
				percent = height*100.f/parent->height;
				break;
		}
	}

	virtual void render() override {
		switch (orientation) {
			case ui::Orientation::Horizontal:
				maxSize = parent->width - minSize;
				break;

			case ui::Orientation::Vertical:
				maxSize = parent->height - minSize;
				break;
		}

		if (splitClick) {
			getPercent();
		} else {
			calcSize();
		}

		ui::UIPanel::render();
	}
};
