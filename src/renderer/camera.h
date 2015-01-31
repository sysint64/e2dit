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

/** @file camera.h
    @brief 2D Camera
*/

#include "../math/glm.h"
#include <string>

class Camera {
public:

	/* Parameters */

	float zoom;
	bool needUpdate;
	glm::vec2 position;
	
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 MVPMatrix;
	
	glm::ivec2 size;
	std::wstring name;

	int viewWidth;
	int viewHeight;

	/* Methods */
	
	Camera (int width, int height);

	/** Update camera position and set needUpdate = true for
		anew update matrices */

	void setPosition (glm::vec2 vec);

	/** Update camera zoom and set needUpdate = true for
		anew update matrices */

	void setZoom (float zoom);

	/** Update view, projection and mvp matrices and set
		needUpdate = false */

	void updateMatrices();

	/** if needUpdate then update matrices else return */

	void update();
};
