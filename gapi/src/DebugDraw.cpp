#ifndef CPP_DEBUG_DRAW
#define CPP_DEBUG_DRAW

void DebugDraw() {

	glBegin2D();

	/* Draw Points */

	glPointSize (6.0f);
	glBegin (GL_POINTS);

	for (int i = 0; i < DebugPoints.size(); i++) {
		glColor3fv (&DebugPoints[i].Color[0]);
		glVertex2f (DebugPoints[i].x-DebugCamera->Position.x+27, DebugPoints[i].y-DebugCamera->Position.y);
		//glVertex2f (DebugPoints[i].x+27, DebugPoints[i].y-DebugCamera->Position.y);
	}

	glEnd();

	/* Draw Lines */

	glBegin (GL_LINES);

	for (int i = 0; i < DebugLines.size(); i++) {
		glColor3fv (&DebugLines[i].Color[0]);
		glVertex2f (DebugLines[i].x1, DebugLines[i].y1);
		glVertex2f (DebugLines[i].x2, DebugLines[i].y2);
	}

	glEnd();

	/* Draw Circles */

	glBegin (GL_LINE_LOOP);

	for (int i = 0; i < DebugCircles.size(); i++) {

		glColor3fv (&DebugCircles[i].Color[0]);
		const int steps = 100;

		for (int j = 0; j < steps; j++) {

			float a = (float)j/float(steps)*pi*2.0f;

			float x = cos(a)*DebugCircles[i].r+DebugCircles[i].x;
			float y = sin(a)*DebugCircles[i].r+DebugCircles[i].y;

			glVertex2f (x, y);
		}
	}

	glEnd();

	glEnd2D();
}

void AddDebugLine (float x1, float y1, float x2, float y2, glm::vec3 Color) {
	DebugLines.push_back ({x1, y1, x2, y2, Color});
}

void AddDebugCircle (float x, float y, float r, glm::vec3 Color) {
	DebugCircles.push_back ({x, y, r, Color});
}

void AddDebugPoint (float x, float y, glm::vec3 Color) {
	DebugPoints.push_back ({x, y, Color});
}

void DebugInit (int count = 0) {
	for (int i = 0; i < count; i++) {
		AddDebugLine   (0, 0, 0, 0);
		AddDebugCircle (0, 0, 0);
		AddDebugPoint  (0, 0);
	}
}

#endif