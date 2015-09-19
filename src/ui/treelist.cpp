/*
 * E2DIT - is a 2D Map Editor to create a levels for 2d games
 * Copyright (C) 2015 Kabylin Andrey <andrey@kabylin.ru>

 * This file is part of E2DIT-UI.

 * E2DIT-UI is free library: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.

 * E2DIT-UI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with E2DIT-UI. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Kabylin Andrey <andrey@kabylin.ru>
 */

#include "ui/treelist.h"

void UITreeListNode::renderLines() {

	int depth = this->depth-treeList->depth;
	if (depth >= 3)
		return;

	manager->atlasShader->unbind();

	glBegin2D();
		glBegin (GL_LINES);

			int offset = elements.size() > 0 ? 8 : 4;

			int x0 = absLeft-btnOffset+offset;     int x1 = absLeft;
			int y0 = app->windowHeight-absTop-5-4; int y1 = 0;

			glColor3f  (0.3f, 0.3f, 0.3f);

			glVertex2f (x0, y0);
			glVertex2f (x1, y0);

			if (depth == 2) {

				x0 = absLeft-btnOffset+5;

				if (this != parent->firstEl) {

					y0 = app->windowHeight-absTop-5-offset;
					y1 = app->windowHeight-prev->absTop-5-4;

					if (prev->elements.size() > 0) y1 -= 4;

				} else {

					y0 = app->windowHeight-absTop-5-offset;
					y1 = app->windowHeight-parent->absTop-20;
					//puts ("!");

				}

				glVertex2f (x0, y0);
				glVertex2f (x0, y1);

			}

		glEnd();
	glEnd2D();

	manager->atlasShader->bind();

}

void UITreeListNode::render() {

	updateAbsPos();
	btnOffset = parent == treeList ? 20 : 14;
	renderLines();

	/* Render Node */

	int depth = this->depth-treeList->depth;
	width = treeList->width-20*depth;

	glUniform1f (manager->atlasShader->locations["Alpha"], 1.f);
	UIButton::render();

	heightIn = height;

	if (elements.size() > 0) {

		int n  = open ? 12 : 13;
		int bx = absLeft-btnOffset; // Button left
		int by = absTop +5 ; // Button Top

		buttonEnter = pointInRect (app->mouseX, app->mouseY, bx, by, iWidths[12], iHeights[12]);

		glUniform1f (manager->atlasShader->locations["Alpha"], buttonEnter ? 1.f : 0.7f);
		renderElement (n, bx, by, iWidths[12], iHeights[12], expandElement.get());

	}

	treeList->height += height; // Update Tree List height

	if (!open)
		return;

	for (const auto &kvp : elements) {

		UIElement *el = kvp.second.get();
		UITreeListNode *node = dynamic_cast<UITreeListNode*> (el);

		if (!node)
			continue;

		node->left = 20;
		node->top  = heightIn;
		node->render();

		heightIn += node->heightIn;

	}

}

void UITreeListNode::mouseUp (int x, int y, int button) {

	UIButton::mouseUp (x, y, button);

	if (buttonEnter && button == mouseLeft)
		open = !open;

}

void UITreeList::render() {

	height = 0;
	UIElement::render();

}
