#pragma once


#include <ui/panel.h>
#include <ui/all.h>

class Pane : public UIPanel {
public:
	Orientation orientation;
	float percent = 50.f;

	Pane(UIManager *manager, Orientation orientation) : UIPanel(manager) {
		this->orientation = orientation;
		background = UIPanel::Background::Transparent;
		blackSplit = true;
		showSplit = true;
		allowResize = true;
		minSize = 400;
		allowScroll = false;

		switch (orientation) {
			case Orientation::Horizontal:
				align = Align::Left;
				break;

			case Orientation::Vertical:
				align = Align::Top;
		}
	}

	inline void calcSize() {
		switch (orientation) {
			case Orientation::Horizontal:
				width = static_cast<int>(static_cast<float>(parent->width)*(percent/100.f));

				if (width < minSize)
					width = minSize;

				break;

			case Orientation::Vertical:
				height = static_cast<int>(static_cast<float>(parent->height)*(percent/100.f));

				if (height < minSize)
					height = minSize;

				break;
		}
	}

	inline  void getPercent() {
		switch (orientation) {
			case Orientation::Horizontal:
				percent = width*100.f/parent->width;
				break;

			case Orientation::Vertical:
				percent = height*100.f/parent->height;
				break;
		}
	}

	virtual void render() override {
		switch (orientation) {
			case Orientation::Horizontal:
				maxSize = parent->width - minSize;
				break;

			case Orientation::Vertical:
				maxSize = parent->height - minSize;
				break;
		}

		if (splitClick) {
			getPercent();
		} else {
			calcSize();
		}

		UIPanel::render();
	}
};
