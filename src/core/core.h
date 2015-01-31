/* E2DIT - 2D Map Editor for game
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public 
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author: Kabylin Andrey <andrey@kabylin.ru>
 */

/** @file core.h
    @brief
*/

#ifndef E2DIT_CORE_H
#define E2DIT_CORE_H

#include "../system/application.h"

class Application;
class Core {
public:

	Application *app;

	/* Constructor */

	Core (Application *_app);
	~Core();

	/* Events */

	void onKeyPressed  (int key);
	void onKeyReleased (int key);
	void onTextEntered (unsigned int key);
	void onMouseMove   (int x, int y);
	void onMouseDown   (int x, int y, int button);
	void onMouseUp     (int x, int y, int button);
	void onDblClick    (int x, int y, int button);
	void onMouseWheel  (int x, int y, int button);
	void onResize      (int width, int height);

	/* */

	void render();
	void step();

};

#endif
