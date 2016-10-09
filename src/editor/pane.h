#pragma once


#include <ui/panel.h>
#include <ui/all.h>

class Pane : public ui::Panel {
private:
	void initParams() {
		background = ui::Panel::Background::Transparent;
		blackSplit = true;
		showSplit = true;
		allowResize = true;
		minSize = 20;
		allowScroll = false;
	}
public:
	ui::Orientation orientation;
	float percent = 50.f;

	Pane(ui::Manager *manager) : ui::Panel(manager) {
		initParams();
	}

	Pane(ui::Manager *manager, ui::Orientation orientation) : ui::Panel(manager) {
		initParams();
		this->orientation = orientation;

		switch (orientation) {
			case ui::Orientation::Vertical:
				align = ui::Align::Left;
				break;

			case ui::Orientation::Horizontal:
				align = ui::Align::Top;
				break;

			case ui::Orientation::None:
				align = ui::Align::Client;
				break;
		}
	}

	inline void calcSize() {
		switch (orientation) {
			case ui::Orientation::Vertical:
				width = static_cast<int>(static_cast<float>(parent->width)*(percent/100.f));

				if (width < minSize)
					width = minSize;

				break;

			case ui::Orientation::Horizontal:
				height = static_cast<int>(static_cast<float>(parent->height)*(percent/100.f));

				if (height < minSize)
					height = minSize;

				break;

			case ui::Orientation::None:
				break;
		}
	}

	inline  void getPercent() {
		switch (orientation) {
			case ui::Orientation::Vertical:
				percent = width*100.f/parent->width;
				break;

			case ui::Orientation::Horizontal:
				percent = height*100.f/parent->height;
				break;

			case ui::Orientation::None:
				break;
		}
	}

	virtual void render() override {
		switch (orientation) {
			case ui::Orientation::Vertical:
				maxSize = parent->width - minSize;
				break;

			case ui::Orientation::Horizontal:
				maxSize = parent->height - minSize;
				break;

			case ui::Orientation::None:
				break;
		}

		if (splitClick) {
			getPercent();
		} else {
			calcSize();
		}

		ui::Panel::render();
	}
};
