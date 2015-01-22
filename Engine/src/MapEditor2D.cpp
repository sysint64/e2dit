//* Author : Kabilin Andrey, 2013 *//

#ifndef CPP_MAPEDITOR_2D
#define CPP_MAPEDITOR_2D

// BaseObject
void BaseGameObject::DrawVertices() {
	for (int i = 0; i < VertexAbsPos.size(); i++) {
		glm::vec2 vwp = glm::vec2 (VertexAbsPos[i].x, VertexAbsPos[i].y);
		glm::vec2 vp  = glm::vec2 (VertexAbsPos[i].x, VertexAbsPos[i].y);
		vwp.x -= MapEditor->Left;
		//
		float d        = glm::distance (glm::vec2(MouseX, WindowHeight-MouseY), glm::vec2(WorldToScreen(vwp)));
		float vdradius = MapEditor->VertexObject->Size.x*2;
		float dsvmin   = vdradius;
		//
		if (dsvmin > d) {
			//glUniform2f (TexAtlasShader->Locations->GetData("Size")  , Core->fVertexSize [4], Core->fVertexSize [5]);
			//glUniform2f (TexAtlasShader->Locations->GetData("Offset"), Core->VertexOffset[4], Core->VertexOffset[5]);
			//
			VertexAbsPos[i].z = 1.f;
		} else VertexAbsPos[i].z = 0.f;

		if (VertexAbsPos[i].z == 1.f || VertexAbsPos[i].w == 1.f) {
			glUniform2f (TexAtlasShader->Locations->GetData("Size")  , Core->fVertexSize [4], Core->fVertexSize [5]);
			glUniform2f (TexAtlasShader->Locations->GetData("Offset"), Core->VertexOffset[4], Core->VertexOffset[5]);
		}

		MapEditor->VertexObject->SetPosition(vp);
		MapEditor->VertexObject->BaseRender();

		if (VertexAbsPos[i].z == 1.0f || VertexAbsPos[i].w == 1.0f) {
			glUniform2f (TexAtlasShader->Locations->GetData("Size")  , Core->fVertexSize [0], Core->fVertexSize [1]);
			glUniform2f (TexAtlasShader->Locations->GetData("Offset"), Core->VertexOffset[0], Core->VertexOffset[1]);
		}
	}

AddMode:
	return;
}

void BaseGameObject::UpdatePosVertices() {

	for (int i = 0; i < JoinedPoints.size(); i++)
		JoinedPoints[i]->UpdatePosJoined();

	for (int i = 0; i < VertexAbsPos.size(); i++) {
		glm::vec2 VPos = glm::vec2 (Data->VertexBuffer[i][0]*Size[0], Data->VertexBuffer[i][1]*Size[1]);

		glm::mat4 mt = glm::translate (glm::mat4(1.0f), glm::vec3(Position+Offset, 0.0f));
		glm::mat4 mr = glm::rotate    (glm::mat4(1.0f), Rotation, glm::vec3(0,0,1));
		//glm::mat4 ms = glm::scale	  (glm::mat4(1.0f), glm::vec3(Core->VertexSize[0], Core->VertexSize[1], 1.0f));
		//glm::mat4 ms = glm::scale	  (glm::mat4(1.0f), glm::vec3(mCamera->Zoom));

		glm::mat4 mtm = glm::translate (glm::mat4(1.0f), glm::vec3(VPos, 0.0f));
		//
		//ModelMatrix = mt*mr*mtm*ms;
		ModelMatrix = mt*mr*mtm;
		MVPMatrix = mCamera->MVPMatrix*ModelMatrix;

		glm::vec2 finalVertPos;
		finalVertPos.x = ModelMatrix[3][0];
		finalVertPos.y = ModelMatrix[3][1];
		//

		VertexAbsPos[i] = glm::vec4 (finalVertPos.x, finalVertPos.y, VertexAbsPos[i].z, VertexAbsPos[i].w);
		SelectPoly->Vertices[i]->Pos = finalVertPos;
		//VertexAbsPos.push_back(finalVertPos);
		//SelectPoly->Insert(finalVertPos);
	}

}

void BaseGameObject::UpdateListVertices() {
	VertexAbsPos.clear();
	SelectPoly->Clear();

	for (int i = 0; i < Data->VertexBuffer.size(); i++) {
		glm::vec2 VPos = glm::vec2 (Data->VertexBuffer[i][0]*Size[0], Data->VertexBuffer[i][1]*Size[1]);

		glm::mat4 mt = glm::translate (glm::mat4(1.0f), glm::vec3(Position+Offset, 0.0f));
		glm::mat4 mr = glm::rotate    (glm::mat4(1.0f), Rotation, glm::vec3(0,0,1));
		//glm::mat4 ms = glm::scale	  (glm::mat4(1.0f), glm::vec3(Core->VertexSize[0], Core->VertexSize[1], 1.0f));
		//glm::mat4 ms = glm::scale	  (glm::mat4(1.0f), glm::vec3(mCamera->Zoom));
		glm::mat4 mtm = glm::translate (glm::mat4(1.0f), glm::vec3(VPos, 0.0f));
		//
		ModelMatrix = mt*mr*mtm;
		MVPMatrix = mCamera->MVPMatrix*ModelMatrix;

		glm::vec2 finalVertPos;
		finalVertPos[0] = ModelMatrix[3][0];
		finalVertPos[1] = ModelMatrix[3][1];
		VertexAbsPos.push_back (glm::vec4 (finalVertPos.x, finalVertPos.y, 0, 0));

		SelectPoly->Insert(finalVertPos);
	}
}

void BaseGameObject::DrawEdges (const bool Selected) {
	glBegin2D();
		glColor3fv(&MapEditor->SelectColor[0]);
		glBegin(GL_LINE_LOOP);

			for (int i = 0; i < VertexAbsPos.size(); i++)
				glVertex2f((VertexAbsPos[i].x-mCamera->Position.x)*mCamera->Zoom,
						   (VertexAbsPos[i].y-mCamera->Position.y)*mCamera->Zoom);

		glEnd();
	glEnd2D();
}

void BaseGameObject::DrawTopology() {
	glBegin2D();
		glColor3f(1.f, 0.f, 1.f);

		for (int i = 0; i < Data->Indices.size(); i += 3) {
			glBegin (GL_LINE_LOOP);
				for (int j = 0; j < 3; j++) {
					const int index = Data->Indices[i+j];
					glVertex2f (VertexAbsPos[index].x-mCamera->Position.x, VertexAbsPos[index].y-mCamera->Position.y);
				}
			glEnd();
		}
	glEnd2D();
}

void BaseGameObject::DisplayPolygonsList() {
	UIListMenu	*ItemsList = (UIListMenu*) MapEditor->Data->Elements["lmpolys"];
	ItemsList->Clear();

	for (int i = 0; i < PolysList.size(); i++) {
		UIFlatButton *Item = new UIFlatButton (false);

		Item->Caption = PolysList[i]->Name;
		Item->ShowIcon = true;
		Item->IconOffset [0] = 5;
		Item->IconOffset [1] = 3;

		ItemsList->AddItem (Item);
		Item->CustomData = this;
		Item->Id = i;

		// Events
		Item->OnDblClick = MapEditor->EditPPolygonDbl;
	}

	ItemsList->Init();
}

void BaseGameObject::DisplayLabelsList() {
	UIListMenu	*ItemsList = (UIListMenu*) MapEditor->Data->Elements["lmlabels"];
	ItemsList->Clear();

	for (int i = 0; i < LabelsList.size(); i++) {
		UIFlatButton *Item = new UIFlatButton (false);

		Item->Caption = LabelsList[i].Name;
		Item->ShowIcon = true;
		Item->IconOffset [0] = 8;
		Item->IconOffset [1] = 2;

		ItemsList->AddItem (Item);
		Item->CustomData = this;
		Item->Id = i;
	}

	ItemsList->Init();
}

void BaseGameObject::DisplayAnimsList() {
	UIListMenu	*ItemsList = (UIListMenu*) MapEditor->Data->Elements["lmanims"];
	ItemsList->Clear();

	for (int i = 0; i < AnimsList.size(); i++) {
		UIFlatButton *Item = new UIFlatButton (false);

		Item->Caption = AnimsList[i]->Name;
		Item->ShowIcon = true;
		Item->IconOffset [0] = 5;
		Item->IconOffset [1] = 8;

		ItemsList->AddItem (Item);
		Item->CustomData = this;
		Item->Id = i;
	}

	ItemsList->Init();
}

// Point Object
void PointObject::EmptyRender() {
	const glm::vec2 P   = Position-mCamera->Position+Offset;
	const float cRadius = 3.f;
	const float lRadius = 10.f+cRadius;

	glm::vec3 mc = glm::vec3(0.f);

	if (MapEditor->SelectedObject == this)
		mc = glm::vec3 (MapEditor->SelectColor[0]/3.f,
						MapEditor->SelectColor[1]/3.f,
						MapEditor->SelectColor[2]/3.f);

	glBegin2D();
		glBegin (GL_LINES);
			glColor3f (0.8f+mc.r, 0.2f+mc.g, 0.2f+mc.b);

			glVertex2f (P.x+cRadius, P.y);
			glVertex2f (P.x+lRadius, P.y);

			glColor3f (0.6f+mc.r, 0.1f+mc.g, 0.1f+mc.b);
			
			glVertex2f (P.x-cRadius, P.y);
			glVertex2f (P.x-lRadius, P.y);

			glColor3f (0.2f+mc.r, 0.8f+mc.g, 0.2f+mc.b);

			glVertex2f (P.x, P.y+cRadius);
			glVertex2f (P.x, P.y+lRadius);

			glColor3f (0.1f+mc.r, 0.6f+mc.g, 0.1f+mc.b);

			glVertex2f (P.x, P.y-cRadius);
			glVertex2f (P.x, P.y-lRadius);
		glEnd();

		//if (SelectedObject =)
		glColor3f (1.f, 1.f, 1.f);
		glBegin (GL_LINE_LOOP);
			for (int i = 0; i < 6; i++) {
				float a = (float)i/6.0f*pi*2.0f;

				float x = cos(a)*cRadius+P.x;
				float y = sin(a)*cRadius+P.y;

				glVertex2f (x, y);
			}
		glEnd();

		if (JoinObj != 0 && MapEditor->SelectedObject == JoinObj) {
			glm::vec2 V = glm::vec2 (JoinObj->VertexAbsPos[VertexJoinId].x,
									 JoinObj->VertexAbsPos[VertexJoinId].y)-mCamera->Position;

			glBegin (GL_LINES);
				glVertex2f (P.x, P.y);
				glVertex2f (V.x, V.y);
			glEnd();
		}

		if (JoinObj != 0 && MapEditor->SelectedObject == this) {
			JoinObj->DrawEdges();
			glm::vec2 V = glm::vec2 (JoinObj->VertexAbsPos[VertexJoinId].x,
									 JoinObj->VertexAbsPos[VertexJoinId].y)-mCamera->Position;

			glBegin (GL_LINES);
				glVertex2f (P.x, P.y);
				glVertex2f (V.x, V.y);
			glEnd();

			glBegin (GL_LINE_LOOP);
				for (int i = 0; i < 6; i++) {
					float a = (float)i/6.0f*pi*2.0f;

					float x = cos(a)*cRadius+V.x;
					float y = sin(a)*cRadius+V.y;

					glVertex2f (x, y);
				}
			glEnd();
		}

	glEnd2D();
}

void PointObject::UpdatePosVertices() {
	const float cRadius = 3.f;
	const float lRadius = 10.f+cRadius;
	const glm::vec2 P   = Position+Offset;

	SelectPoly->Vertices[0]->Pos = glm::vec2 (P.x-lRadius, P.y-lRadius);
	SelectPoly->Vertices[1]->Pos = glm::vec2 (P.x+lRadius, P.y-lRadius);
	SelectPoly->Vertices[2]->Pos = glm::vec2 (P.x+lRadius, P.y+lRadius);
	SelectPoly->Vertices[3]->Pos = glm::vec2 (P.x-lRadius, P.y+lRadius);
}

void PointObject::UpdatePosJoined() {
	if (JoinObj == 0)
		return;

	//Position = glm::vec2 (JoinObj->VertexAbsPos[VertexJoinId].x,
	//					  JoinObj->VertexAbsPos[VertexJoinId].y)+dPosition-Offset;
	int i = VertexJoinId;
	glm::vec2 VPos = glm::vec2 (JoinObj->Data->VertexBuffer[i][0]*JoinObj->Size[0], JoinObj->Data->VertexBuffer[i][1]*JoinObj->Size[1]);

	glm::mat4 mt = glm::translate (glm::mat4(1.0f), glm::vec3(JoinObj->Position, 0.0f));
	glm::mat4 mr = glm::rotate    (glm::mat4(1.0f), JoinObj->Rotation, glm::vec3(0,0,1));
	//glm::mat4 ms = glm::scale	  (glm::mat4(1.0f), glm::vec3(Core->VertexSize[0], Core->VertexSize[1], 1.0f));
	//glm::mat4 ms = glm::scale	  (glm::mat4(1.0f), glm::vec3(mCamera->Zoom));

	glm::mat4 mtm = glm::translate (glm::mat4(1.0f), glm::vec3(dPositionLast-JoinObj->Position, 0.0f));
	//
	//ModelMatrix = mt*mr*mtm*ms;
	ModelMatrix = mt*mr*mtm;
	MVPMatrix = mCamera->MVPMatrix*ModelMatrix;

	glm::vec2 finalVertPos;
	finalVertPos.x = ModelMatrix[3][0];
	finalVertPos.y = ModelMatrix[3][1];
	//
	//Position = finalVertPos;

	UpdatePosVertices();
}

void PointObject::UpdateListVertices() {
	const float cRadius = 3.f;
	const float lRadius = 10.f+cRadius;
	const glm::vec2 P   = Position+Offset;

	SelectPoly->Clear();
	
	SelectPoly->Insert (glm::vec2(P.x-lRadius, P.y-lRadius));
	SelectPoly->Insert (glm::vec2(P.x+lRadius, P.y-lRadius));
	SelectPoly->Insert (glm::vec2(P.x+lRadius, P.y+lRadius));
	SelectPoly->Insert (glm::vec2(P.x-lRadius, P.y+lRadius));
}

/* Light Source */

void LightObject::UpdatePosVertices() {
	const float cRadius = 12.f;
	const float lRadius = 10.f+cRadius;
	const glm::vec2 P   = Position+Offset;

	SelectPoly->Vertices[0]->Pos = glm::vec2 (P.x-lRadius, P.y-lRadius);
	SelectPoly->Vertices[1]->Pos = glm::vec2 (P.x+lRadius, P.y-lRadius);
	SelectPoly->Vertices[2]->Pos = glm::vec2 (P.x+lRadius, P.y+lRadius);
	SelectPoly->Vertices[3]->Pos = glm::vec2 (P.x-lRadius, P.y+lRadius);
}

void LightObject::UpdateListVertices() {
	const float cRadius = 12.f;
	const float lRadius = 10.f+cRadius;
	const glm::vec2 P   = Position+Offset;

	SelectPoly->Clear();
	
	SelectPoly->Insert (glm::vec2(P.x-lRadius, P.y-lRadius));
	SelectPoly->Insert (glm::vec2(P.x+lRadius, P.y-lRadius));
	SelectPoly->Insert (glm::vec2(P.x+lRadius, P.y+lRadius));
	SelectPoly->Insert (glm::vec2(P.x-lRadius, P.y+lRadius));
}

void LightObject::EmptyRender() {

	glBegin2D();

	const glm::vec2 P = Position-mCamera->Position+Offset;
	const float inRadius = 6.f;

	if (MapEditor->ShowLightRadius || MapEditor->SelectedObject == this || MapEditor->SpriteCursor == this) {
		glColor3f (Color.r, Color.g, Color.b);
		glBegin (GL_LINE_LOOP);
			for (int i = 0; i < 100; i++) {
				float a = (float)i/100*pi*2.0f;

				float x = cos(a)*Radius+P.x;
				float y = sin(a)*Radius+P.y;

				glVertex2f (x, y);
			}
		glEnd();
	}

	float m = 1.f;
	//
	if (MapEditor->SelectedObject == this || MapEditor->SpriteCursor == this)
		glColor3fv (&MapEditor->SelectColor[0]);
	else
		glColor3f (Color.r, Color.g, Color.b);

	glBegin (GL_LINE_LOOP);
	for (int i = 0; i < int(inRadius*2); i++) {
		float a = (float)i/(inRadius*2)*pi*2.0f;

		float x = cos(a)*inRadius*1.7f+P.x;
		float y = sin(a)*inRadius*1.7f+P.y;

		glVertex2f (x, y);
	}
	glEnd();

	/* */
	glBegin(GL_LINE_STRIP);
		glVertex2f (P.x-inRadius/m, P.y+inRadius*2);
		glVertex2f (P.x, P.y+inRadius*3);
		glVertex2f (P.x+inRadius/m, P.y+inRadius*2);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex2f (P.x-inRadius/m, P.y-inRadius*2);
		glVertex2f (P.x, P.y-inRadius*3);
		glVertex2f (P.x+inRadius/m, P.y-inRadius*2);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex2f (P.x+inRadius*2, P.y-inRadius/m);
		glVertex2f (P.x+inRadius*3, P.y);
		glVertex2f (P.x+inRadius*2, P.y+inRadius/m);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex2f (P.x-inRadius*2, P.y-inRadius/m);
		glVertex2f (P.x-inRadius*3, P.y);
		glVertex2f (P.x-inRadius*2, P.y+inRadius/m);
	glEnd();

	/**/

	glBegin(GL_LINE_STRIP);
		glVertex2f (P.x+inRadius/m, P.y+inRadius*2);
		glVertex2f (P.x+inRadius*2, P.y+inRadius*2);
		glVertex2f (P.x+inRadius*2, P.y+inRadius/m);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex2f (P.x+inRadius/m, P.y-inRadius*2);
		glVertex2f (P.x+inRadius*2, P.y-inRadius*2);
		glVertex2f (P.x+inRadius*2, P.y-inRadius/m);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex2f (P.x-inRadius/m, P.y-inRadius*2);
		glVertex2f (P.x-inRadius*2, P.y-inRadius*2);
		glVertex2f (P.x-inRadius*2, P.y-inRadius/m);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex2f (P.x-inRadius/m, P.y+inRadius*2);
		glVertex2f (P.x-inRadius*2, P.y+inRadius*2);
		glVertex2f (P.x-inRadius*2, P.y+inRadius/m);
	glEnd();

	glEnd2D();

}

// Layer
void Layer::Render() {
	//
	for (int i = 0; i < Objects.size(); i++) {
		if (Objects[i]->Material == 0) {
			Objects[i]->EmptyRender();
			continue;
		}

		if (!Objects[i]->Visible)
			continue;

		if (Objects[i]->NeedUpdate || Objects[i]->mCamera->NeedUpdate)
			Objects[i]->UpdateModelMatrix();

		if (Objects[i]->Material->Uniforms->Exist ("MVP"))
			Objects[i]->Material->Uniforms->SetMat4f ("MVP", Objects[i]->MVPMatrix);

		Objects[i]->Material->Apply();
		Objects[i]->EmptyRender();
		Objects[i]->Material->UnApply();
	}
}
//
void MapEditor2D::Render() {
	//
	MapEditor->LastTex = 0;
	MapEditor->LastShader = 0;

	PolygonGoupsList->Width = DetailsPanel->Width-23-DetailsPanel->ScrollWidth;
	SpriteGoupsList ->Width = PolygonGoupsList->Width;
	//
	int ParentWidth  = WindowWidth;
	int ParentHeight = WindowHeight;
	//
	int x0 = 0; int mWidth  = 0;
	int y0 = 0; int mHeight = 0;
	//
	//
	for (int i = 0; i < Manager->Elements.size(); i++) {
		//
		if (Manager->Elements[i]->Align == alNone)
			continue;

		if (dynamic_cast<UIPanel*>(Manager->Elements[i]) && !((UIPanel*)Manager->Elements[i])->DrawQuad)
			continue;
		//
		switch (Manager->Elements[i]->Align) {
			//
			case alTop :
				y0 += Manager->Elements[i]->Height;
				break;
			//
			case alBottom :
				mHeight += Manager->Elements[i]->Height;
				break;
			//
			case alLeft :
				x0 += Manager->Elements[i]->Width;
				break;
			//
			case alRight :
				mWidth += Manager->Elements[i]->Width;
				break;
		}
		//
	}
	//
	//x0 = 0; mWidth  = 0;
	//y0 = 0; mHeight = 0;
	//
	//clamp (&Camera->Position[0], -320.0f, 2048.0f-Width+320.0f);
	//clamp (&Camera->Position[1], -320.0f, 1024.0f-Height+320.0f);
	//
	Width  = ParentWidth-mWidth-x0;
	Height = ParentHeight-mHeight-y0;
	Left = x0; Top = y0;
	//
	int cLeft = Left-Camera->Position[0];
	int cTop  =     -Camera->Position[1];
	//
	SnapElements->Left = x0+10;
	SnapElements->Top  = y0+Height-26;

	ModeElements->Top = SnapElements->Top;
	ModeElements->Left = x0+100;
	//
	// Draw Grid
	glBegin2D();
	glScalef(Camera->Zoom, Camera->Zoom, Camera->Zoom);
	glBegin (GL_LINES);
	glDisable (GL_LINE_SMOOTH);
	glLineWidth(0.5);
	//glColor3f (61.0f/255.0f, 61.0f/255.0f, 61.0f/255.0f);
	glColor3fv(&GridColor[0]);
	//
	int xx = x0;
	int n = 0;
	//
	int i0 = x0-Camera->Position[0]+dgx;
	int i1 = x0+Width;
	//
	if (i0 < x0-StepGrid) { i0 = x0; dgx += StepGrid; }
	if (i0 > x0+StepGrid) { i0 = x0; dgx -= StepGrid; }
	//
	for (int i = i0; i < i1; i += StepGrid) {
		glVertex2f (i, WindowHeight-y0);
		glVertex2f (i, WindowHeight-y0-Height);
	}

	//
	//i0 = y0+Camera->Position[1]+dgy;
	//i1 = y0+Height;
	i0 = -Camera->Position[1]+dgy-StepGrid*10;
	i1 = Height;
	//
	if (i0 < -StepGrid) { i0 = 0; dgy += StepGrid; }
	if (i0 > +StepGrid) { i0 = 0; dgy -= StepGrid; }
	//
	//glVertex2f (x0, i0);
	//glVertex2f (x0+Width, i0);
	for (int i = i0; i < i1; i += StepGrid) {
		glVertex2f (x0, i);
		glVertex2f (x0+Width, i);
	}

	glEnd ();

	//
	glEnable (GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0, 1); 
	glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT); 
	glColorMask(false, false, false, false); 

	glBegin (GL_QUADS);
		glVertex2f (x0, WindowHeight-y0);
		glVertex2f (x0+Width, WindowHeight-y0);
		glVertex2f (x0+Width, WindowHeight-y0-Height);
		glVertex2f (x0, WindowHeight-y0-Height);
	glEnd();

	glBegin (GL_QUADS);
		glVertex2f (cLeft, cTop);
		glVertex2f (cLeft+RoomSize.x, cTop);
		glVertex2f (cLeft+RoomSize.x, cTop+RoomSize.y);
		glVertex2f (cLeft, cTop+RoomSize.y);
	glEnd();

	glStencilFunc(GL_NOTEQUAL, 0, 1); 
	glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO); 
	glColorMask(true, true, true, true); 

	glColor4f (0.0f, 0.0f, 0.0f, 0.1f);
	glBegin (GL_QUADS);
		glVertex2f (x0, WindowHeight-y0);
		glVertex2f (x0+Width, WindowHeight-y0);
		glVertex2f (x0+Width, WindowHeight-y0-Height);
		glVertex2f (x0, WindowHeight-y0-Height);
	glEnd();

	glDisable (GL_STENCIL_TEST);
	glClear (GL_STENCIL_BUFFER_BIT);

	// Border
	glBegin (GL_LINE_LOOP);
		glColor3fv(&RoomBorderColor[0]);
		glVertex2f (cLeft, cTop);
		glVertex2f (cLeft+RoomSize.x, cTop);
		glVertex2f (cLeft+RoomSize.x, cTop+RoomSize.y);
		glVertex2f (cLeft, cTop+RoomSize.y);
	glEnd();

	glColor3fv(&CameraColor[0]);

	/*for (int i = 0; i < Cameras.size(); i++) {
		glPushMatrix();
		glActiveTexture(GL_TEXTURE0);
		glTranslatef(cLeft+Cameras[i]->Position.x+10, cTop+Cameras[i]->Position.y+Cameras[i]->Size.y+10, 0);
		string CameraName = wstring2string(Cameras[i]->Name);
		ftglRenderFont(Manager->Theme->Font, CameraName.c_str(), RENDER_ALL);
		glPopMatrix();
		//
		glBegin (GL_LINE_LOOP);
			glColor3fv(&CameraColor[0]);
			glVertex2f (cLeft+Cameras[i]->Position.x                   , cTop+Cameras[i]->Position.y);
			glVertex2f (cLeft+Cameras[i]->Position.x+Cameras[i]->Size.x, cTop+Cameras[i]->Position.y);
			glVertex2f (cLeft+Cameras[i]->Position.x+Cameras[i]->Size.x, cTop+Cameras[i]->Position.y+Cameras[i]->Size.y);
			glVertex2f (cLeft+Cameras[i]->Position.x                   , cTop+Cameras[i]->Position.y+Cameras[i]->Size.y);
		glEnd();
	}*/
	//
	//glEnd2D();
	//
	Camera->Update();
	SpriteDat->SetAttrs();
	//
	//TexAtlasShader->ApplyShader();
	for (int i = Layers.size()-1; i >= 0; i--) {
		if (Layers[i]->Visible)
			Layers[i]->Render();
	}

	if (AddOnMapObj) {
		//
		if (GridSnap) {
			//
			int snx = round (MouseX+Camera->Position[0]-Left);
			int sny = round (WindowHeight-MouseY+Camera->Position[1]);
			int sox = 0; int soy = -1;
			//
			if (SnapOffset) {
				sox = ((int) SpriteCursor->Size[0] >> 1);
				soy = ((int) SpriteCursor->Size[1] >> 1);
			}
			//
			SpriteCursor->Position[0] = snx - snx % StepGrid + sox;
			SpriteCursor->Position[1] = sny - sny % StepGrid + soy;
			SpriteCursor->NeedUpdate = true;
		} else {
			SpriteCursor->Position[0] = MouseX/Camera->Zoom+Camera->Position[0]-Left;
			SpriteCursor->Position[1] = (WindowHeight-MouseY)/Camera->Zoom+Camera->Position[1];
			SpriteCursor->NeedUpdate = true;
		}
		//
		if (SpriteCursor->Material != 0) {
			TexAtlasShader->ApplyShader();
			glUniform1i (TexAtlasShader->Locations->GetData("Texture"), 0);
			glUniform2f (TexAtlasShader->Locations->GetData("Size")  , SpriteCursor->tSize[0], SpriteCursor->tSize[1]);
			glUniform2f (TexAtlasShader->Locations->GetData("Offset"), SpriteCursor->tOffset[0], SpriteCursor->tOffset[1]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, SpriteCursor->Texture->Handle);

			SpriteCursor->BaseRender();
			TexAtlasShader->UnApplyShader();
		} else {
			SpriteCursor->EmptyRender();
		}
	}
	
	// Draw Vertices -------------------------------------------------->
	//
	/*for (int i = Layers.size()-1; i >= 0; i--) {
		for (int j = 0; j < Layers[i]->Objects.size(); j++)
			Layers[i]->Objects[j]->DrawVertices();
	}*/

	if (SelectedObjects.size() > 1) {

		for (int i = 0; i < SelectedObjects.size(); i++) {
			SelectedObjects[i]->DrawTopology();
			SelectedObjects[i]->DrawEdges();

			if (CurrMode == tEdit || CurrMode == tAdd) {
				TexAtlasShader->ApplyShader();

				glUniform1i (TexAtlasShader->Locations->GetData("Texture"), 0);
				glUniform2f (TexAtlasShader->Locations->GetData("Size")  , Core->fVertexSize [0], Core->fVertexSize[1]);
				glUniform2f (TexAtlasShader->Locations->GetData("Offset"), Core->VertexOffset[0], Core->VertexOffset[1]);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, Core->Theme->Skin->Handle);

				SelectedObjects[i]->DrawVertices();
				TexAtlasShader->UnApplyShader();
			}

			if (CurrMode == tEdit) {
				SelectedObjects[i]->UpdatePosVertices();
			}
		}

	} else

	if (SelectedObject != 0 && SelectedObject->Template != SelectedObject) {
		SelectedObject->DrawTopology();
		SelectedObject->DrawEdges();

		//glBegin2D();
			glPointSize (6.0f);
			glColor3f(1.f,0,0);
			/*glBegin(GL_POINTS);
				glVertex2f(SelectedObject->WorldToScreen().x, SelectedObject->WorldToScreen().y);

				glColor3f(0.f,1.f,0);
				glVertex2f(dPoint.x, dPoint.y);

				glVertex2f(eDebug[0], eDebug[1]);
				glVertex2f(eDebug[2], eDebug[3]);

				glVertex2f(eDebug2[0], eDebug2[1]);
				glVertex2f(eDebug2[2], eDebug2[3]);
			glEnd();

			glBegin(GL_LINES);
				glVertex2f(eDebug[0], eDebug[1]);
				glVertex2f(eDebug[2], eDebug[3]);

				glVertex2f(eDebug2[0], eDebug2[1]);
				glVertex2f(eDebug2[2], eDebug2[3]);
			glEnd();*/
		//glEnd2D();

		if (CurrMode == tEdit || CurrMode == tAdd) {
			TexAtlasShader->ApplyShader();

			glUniform1i (TexAtlasShader->Locations->GetData("Texture"), 0);
			glUniform2f (TexAtlasShader->Locations->GetData("Size")  , Core->fVertexSize [0], Core->fVertexSize[1]);
			glUniform2f (TexAtlasShader->Locations->GetData("Offset"), Core->VertexOffset[0], Core->VertexOffset[1]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Core->Theme->Skin->Handle);

			SelectedObject->DrawVertices();
			TexAtlasShader->UnApplyShader();
		}

		if (CurrMode == tEdit) {
			SelectedObject->UpdatePosVertices();
		}
	}

	// <-------------------------------------------------- Draw Vertices
	

	GUIElementDat->SetAttrs();
	//
	if (SelectedObject != 0) {
		glColor3f(1,0,0);
		glBegin(GL_POINTS);
			glVertex2fv (&(TransformPivot-Camera->Position+SelectedObject->Offset)[0]);
		glEnd();
		//SelectedObject->DebugRender(glm::vec2(SelectedObject->Position.x-Camera->Position.x+SelectedObject->Offset.x, SelectedObject->Position.y-Camera->Position.y+SelectedObject->Offset.y));
		//SelectedObject->DrawEdges();
	}

	/* Draw Selected Rect */

	//glBegin2D();

	if (Shift == mouseLeft && !isKey[keyLAlt] && !isKey[keyRAlt] && !SkipMouseUp) {

		SelectRect[1] = glm::vec2 (MouseX, WindowHeight-MouseY);
		
		SelectRectWorld[1].x = SelectRect[1].x+Camera->Position.x;
		SelectRectWorld[1].y = SelectRect[1].y+Camera->Position.y;

		/* Draw */

		glColor3f (1, 1, 1);
		glBegin (GL_LINE_LOOP);

			glVertex2f (SelectRect[0].x, SelectRect[0].y);
			glVertex2f (SelectRect[0].x, SelectRect[1].y);
			glVertex2f (SelectRect[1].x, SelectRect[1].y);
			glVertex2f (SelectRect[1].x, SelectRect[0].y);

		glEnd ();

	} else {

		SelectRect[0] = glm::vec2 (MouseX, WindowHeight-MouseY);
		
		SelectRectWorld[0].x = SelectRect[0].x+Camera->Position.x;
		SelectRectWorld[0].y = SelectRect[0].y+Camera->Position.y;

	}

	glEnd2D();
}
//
void MapEditor2D::Step() {
	//
	if (isKey[keyLCtrl] && !CtrlSnap) {
		GridSnap = !GridSnap;
		CtrlSnap = true;
	} else if (CtrlSnap && !isKey[keyLCtrl]) {
		GridSnap = !GridSnap;
		CtrlSnap = false;
	}
}

// Main Tab

void MapEditor2D::ChangeClassName (UIElement *Sender) {

	if (MapEditor->SelectedObject == 0)
		return;

	UIEdit *Elem = (UIEdit*) Sender;
	MapEditor->SelectedObject->GameClass = wstring2string (Elem->wText);

}

void MapEditor2D::SetLightColorCallback () {
	((UIColorPanel*) MapEditor->Data->Elements["clightcolor"])->Color = colorDialog->NewColor->Color;

	if (MapEditor->SelectedObject != 0 && dynamic_cast<LightObject*>(MapEditor->SelectedObject)) {
		LightObject *Light = (LightObject*) MapEditor->SelectedObject;

		Light->Color = colorDialog->NewColor->Color;
	}
}

void MapEditor2D::SetLightColor (UIElement *Sender) {
	colorDialog->Open(((UIColorPanel*) MapEditor->Data->Elements["clightcolor"])->Color, SetLightColorCallback);
}

void MapEditor2D::SetDynamicLightType (UIElement *Sender) {

	if (MapEditor->SelectedObject != 0 && dynamic_cast<LightObject*>(MapEditor->SelectedObject)) {
		LightObject *Light = (LightObject*) MapEditor->SelectedObject;
		if (Sender->Checked) Light->Type = ltDynamic;
	}

}

void MapEditor2D::SetStaticLightType (UIElement *Sender) {
	
	if (MapEditor->SelectedObject != 0 && dynamic_cast<LightObject*>(MapEditor->SelectedObject)) {
		LightObject *Light = (LightObject*) MapEditor->SelectedObject;
		if (Sender->Checked) Light->Type = ltStatic;
	}

}

void MapEditor2D::SetStationarLightType (UIElement *Sender) {
	
	if (MapEditor->SelectedObject != 0 && dynamic_cast<LightObject*>(MapEditor->SelectedObject)) {
		LightObject *Light = (LightObject*) MapEditor->SelectedObject;
		if (Sender->Checked) Light->Type = ltStationar;
	}

}

void MapEditor2D::SetLightCastShadowType (UIElement *Sender) {
	
	if (MapEditor->SelectedObject != 0 && dynamic_cast<LightObject*>(MapEditor->SelectedObject)) {
		LightObject *Light = (LightObject*) MapEditor->SelectedObject;
		Light->CastShadow = Sender->Checked;
	}

}

// Polygons Tab ------------------------------------------------------------------->

void MapEditor2D::EditPPolygonDbl (UIElement *Sender, int x, int y, int Shift) {
	setPolygonForm->Open ((BaseGameObject*) Sender->CustomData, Sender->Id);
}

// <------------------------------------------------------------------- Polygons Tab

// Select Tool ----------------------------------------->

void MapEditor2D::SelectObject (int x, int y) {

	if (DialogOpened) return;
	
	if (!isKey[keyLCtrl ] && !isKey[keyRCtrl ] &&
		!isKey[keyLShift] && !isKey[keyRShift])
	{
		SelectedObjects.clear();
		SelectedObject = 0;
	}

	if (SelectRect[0] == SelectRect[1]) {

		int snx = round (x+Camera->Position[0]);
		int sny = round (WindowHeight-y+Camera->Position[1]);
		//
		for (int i = Layers.size()-1; i >= 0; i--) {
			if (!Layers[i]->Visible || !Layers[i]->Select) continue;
			for (int j = Layers[i]->Objects.size()-1; j >= 0; j--) {
				if (Layers[i]->Objects[j]->Visible)
				if (PointInPolygon(glm::vec2(snx, sny), Layers[i]->Objects[j]->SelectPoly)) {

					if (isKey[keyLCtrl ] || isKey[keyRCtrl]) {
						
						for (int k = 0; k < SelectedObjects.size(); k++) {
							if (SelectedObjects.size() == 1) break;
							if (SelectedObjects[k] == Layers[i]->Objects[j]) {
								
								if (SelectedObject == SelectedObjects[k]) {

									if (k == 0) SelectedObject = *(SelectedObjects.begin()+k+1);
									else        SelectedObject = *(SelectedObjects.begin()+k-1);

								}

								SelectedObjects.erase(SelectedObjects.begin()+k);
								return;

							}

						}

					}

					bool skip = false;

					for (int k = 0; k < SelectedObjects.size(); k++) {
						if (SelectedObjects[k] == Layers[i]->Objects[j]) {
							skip = true;
							break;
						}
					}

					if (skip)
						continue;

					SelectedObject = Layers[i]->Objects[j];
					SelectedObjects.push_back (Layers[i]->Objects[j]);
					SelectedObject->DisplayParams();

					if (dynamic_cast<PolygonObject*>(SelectedObject)) CurSelMode = tPolygon;
					else if (dynamic_cast<PointObject*>(SelectedObject)) CurSelMode = tPoint;
					else CurSelMode = tSprite;

					UpdateModeButtons();
					UpdateTransformPivot();
					return;
				}
			}
		}


		CurSelMode = tNone;
		UpdateModeButtons();

		// Clear Var Panel && Materials Panel
		UIPanel *VarsTabPanel = ((UIPanel*) MapEditor->Data->Elements["pvarstab"]);

		for (int i = 0; i < VarsTabPanel->Elements.size(); i++)
			delete VarsTabPanel->Elements[i];
		
		VarsTabPanel->Elements.clear();
		((UIListMenu*) MapEditor->Data->Elements["lmpolys"] )->Clear();
		((UIListMenu*) MapEditor->Data->Elements["lmlabels"])->Clear();
		((UIListMenu*) MapEditor->Data->Elements["lmanims"] )->Clear();

	} else {

		/* */

		if (isKey[keyLCtrl ] || isKey[keyRCtrl]) {
							
			for (int k = 0; k < SelectedObjects.size(); k++) {
				if (SelectedObjects.size() == 1) break;
				if (!PointInRect (SelectedObjects[k]->Position, &SelectRectWorld[0]))
					continue;
					
				/*if (SelectedObject == SelectedObjects[k]) {

					if (k == 0) SelectedObject = *(SelectedObjects.begin()+k+1);
					else        SelectedObject = *(SelectedObjects.begin()+k-1);
					
				}*/

				SelectedObjects[k]->NoSelect = true;
				SelectedObjects.erase (SelectedObjects.begin()+k);

				SelectedObject = SelectedObjects[0];
			}

		}

		/* */

		for (int i = Layers.size()-1; i >= 0; i--) {
			if (!Layers[i]->Visible || !Layers[i]->Select) continue;
			for (int j = Layers[i]->Objects.size()-1; j >= 0; j--) {

				if (Layers[i]->Objects[j]->Visible)
				if (PointInRect (Layers[i]->Objects[j]->Position, &SelectRectWorld[0])) {

					if (Layers[i]->Objects[j]->NoSelect) {
						Layers[i]->Objects[j]->NoSelect = false;

					} else {

						bool skip = false;

						for (int k = 0; k < SelectedObjects.size(); k++) {
							if (SelectedObjects[k] == Layers[i]->Objects[j]) {
								skip = true;
								break;
							}
						}

						if (skip)
							continue;

						SelectedObjects.push_back (Layers[i]->Objects[j]);
						SelectedObject = Layers[i]->Objects[j];
						
					}

				}

			}
		}

		UpdateTransformPivot();

	}

}

void MapEditor2D::SelectVertices() {

	if (DialogOpened) return;
	if (SelectedObject == 0) return;
	
	SelectedVertices.clear();

	if (SelectRect[0] == SelectRect[1]) {

		for (int i = 0; i < SelectedObject->VertexAbsPos.size(); i++) {
			glm::vec4 v = SelectedObject->VertexAbsPos[i];

			//if (v.z == 1.f) v.w = 1.f;
			if (v.z == 1.0f) { 
				
				v.w = 1.f;
				SelectedVertices.push_back(i);
				UpdateTransformPivot();
				SelectedObject->VertexAbsPos[i] = v;
				return;

			} else {

				v.w = 0.f;
				SelectedObject->VertexAbsPos[i] = v;

			}

		}

	} else {

		for (int i = 0; i < SelectedObject->VertexAbsPos.size(); i++) {
			glm::vec4 v = SelectedObject->VertexAbsPos[i];

			if (PointInRect (glm::vec2(v), &SelectRectWorld[0]))
			{
				v.w = 1.f;
				SelectedVertices.push_back(i);
			} else {

				v.w = 0.f;

			}

			SelectedObject->VertexAbsPos[i] = v;
		}

		UpdateTransformPivot();

	}

}

// <----------------------------------------- Select Tool

// Update Groupes -------------------------------------->

void MapEditor2D::UpdateSpriteGroup () {
	SpritesList->Clear();

	for (int i = 0; i < MapEditor->SpritesGroups[MapEditor->CurSpriteGroup].Items.size(); i++) {
		SpriteGroupItem *gItem = &(MapEditor->SpritesGroups[MapEditor->CurSpriteGroup].Items[i]);
		UITallFlatButton *Item = new UITallFlatButton();

		Item->Caption = gItem->Name;

		Item->Texture  = gItem->Texture;
		Item->Caption  = gItem->Name;
		Item->Descr    = StrToWStr (IntToStr (gItem->TexSize[0])+" x "+IntToStr (gItem->TexSize[1]));
		//
		Item->SpriteW  = gItem->TexSize[0];
		Item->SpriteH  = gItem->TexSize[1];
		//
		Item->tWidth   = (float) gItem->TexSize[0] / (float) gItem->Texture->Width;
		Item->tHeight  = (float) gItem->TexSize[1] / (float) gItem->Texture->Height;

		Item->tOffsetX = gItem->TexOffset[0];
		Item->tOffsetY = gItem->TexOffset[0];
		//
		Item->OnMouseDown = MapEditor2D::SelectSprite;
		Item->OnDblClick  = MapEditor2D::AddSpriteOnMap;
		Item->OnClick = 0;

		MapEditor->SpritesList->AddItem (Item);
	}

	MapEditor->SpritesList->Init();
}

void MapEditor2D::UpdatePolygonGroup () {
	PolygonsList->Clear();

	for (int i = 0; i < MapEditor->PolygonsGroups[MapEditor->CurPolygonGroup].Items.size(); i++) {
		UIFlatButton *Item = new UIFlatButton (false);

		Item->Caption = MapEditor->PolygonsGroups[MapEditor->CurPolygonGroup].Items[i].Name;
		Item->OnKeyPressed = EditPolygonName;
		Item->OnMouseDown = SelectPolygon;
		Item->OnDblClick = EditPolygonNameDbl;
		Item->OnClick = 0;

		Item->ShowIcon = true;
		Item->IconOffset [0] = 5;
		Item->IconOffset [1] = 3;

		MapEditor->PolygonsList->AddItem (Item, i);
		MapEditor->CurPolygonId = Item->Id;
	}

	MapEditor->PolygonsList->Init();
}

// <-------------------------------------- Update Groupes

// Set Tool Panel ------------------------------------------------------------------------------
//
// --- Select Tool ---------------------------------
void MapEditor2D::SetToolSelRect    (UIElement *Sender) {
	MapEditor->SelectTool->Top = Sender->Top;
	MapEditor->CurrSelectTool  = tRect;
}

void MapEditor2D::SetToolSelCircle (UIElement *Sender) {
	MapEditor->SelectTool->Top = Sender->Top;
	MapEditor->CurrSelectTool  = tCircle;
}

void MapEditor2D::SetToolSelLaso (UIElement *Sender) {
	MapEditor->SelectTool->Top = Sender->Top;
	MapEditor->CurrSelectTool  = tLaso;
}
//
// --- Transform Tool ------------------------------
void MapEditor2D::SetToolMove (UIElement *Sender) {
	MapEditor->SelectTool->Top   = MapEditor->Data->Elements["iselect"]->Top;
	MapEditor->CurrTransformTool = tMove;
}

void MapEditor2D::SetToolRotate (UIElement *Sender) {
	MapEditor->SelectTool->Top   = MapEditor->Data->Elements["irotate"]->Top;
	MapEditor->CurrTransformTool = tRotate;
}

void MapEditor2D::SetToolScale (UIElement *Sender) {
	MapEditor->SelectTool->Top   = MapEditor->Data->Elements["iscale"]->Top;
	MapEditor->CurrTransformTool = tScale;
}
// --------------------------------------------------------------------------------------------

// Operations with Layers ---------------------------------------------------------------------
void MapEditor2D::SelectLayer (UIElement *Sender, int x, int y, int Shift) {
	//
	if (PointInRect (x, y, Sender->Left+5, Sender->Top, 18, Sender->Height)) {
		MapEditor->Layers[Sender->Id]->Visible = !MapEditor->Layers[Sender->Id]->Visible;

		if (MapEditor->Layers[Sender->Id]->Visible) {
			if (MapEditor->Layers[Sender->Id]->Select) Sender->Icon2Offset [0] = 6;
			Sender->IconOffset [0] = 4;
		} else {
			Sender->Icon2Offset[0] = 7;
			Sender->IconOffset [0] = 5;
		}
		//
	} else if (PointInRect (x, y, Sender->Left+25, Sender->Top, 15, Sender->Height)) {
		MapEditor->Layers[Sender->Id]->Select = !MapEditor->Layers[Sender->Id]->Select;

		if (MapEditor->Layers[Sender->Id]->Select) Sender->Icon2Offset [0] = 6;
		else Sender->Icon2Offset [0] = 7;
	}// else {
	//
	MapEditor->CurLayerId = Sender->Id;
	//}
}

void MapEditor2D::EditLayer (UIElement *Sender, Uint16 key) {
	//
	if (key == keyF2) {
		MapEditor->LayerEdit->Visible = true;
		//
		int M = 0;
		for (int i = 0; i < MapEditor->LayersList->Items.size(); i++) {
			//
			if (!MapEditor->LayersList->Items[i]->Visible) continue;
			if (MapEditor->LayersList->Items[i]->Id == MapEditor->CurLayerId) break;
			M++;
		}
		//
		MapEditor->LayerEdit->Top = 8+22*M;
		MapEditor->LayerEdit->Id = MapEditor->CurLayerId;
		MapEditor->LayerEdit->wText = StrToWStr (MapEditor->Layers[MapEditor->CurLayerId]->Name);
		MapEditor->LayerEdit->SetFocused();
	}
}

void MapEditor2D::EditLayerDbl (UIElement *Sender, int x, int y, int Shift) {
	//
	if (!PointInRect (x, y, Sender->Left+25+18, Sender->Top, Sender->Width-25-18, Sender->Height))
		return;
	//
	MapEditor->LayerEdit->Visible = true;
	//
	int M = 0;
	for (int i = 0; i < MapEditor->LayersList->Items.size(); i++) {
		//
		if (!MapEditor->LayersList->Items[i]->Visible) continue;
		if (MapEditor->LayersList->Items[i]->Id == MapEditor->CurLayerId) break;
		M++;
	}
	//
	MapEditor->LayerEdit->Top = 8+22*M;
	MapEditor->LayerEdit->Id = MapEditor->CurLayerId;
	MapEditor->LayerEdit->wText = StrToWStr (MapEditor->Layers[MapEditor->CurLayerId]->Name);
	MapEditor->LayerEdit->SetFocused();
}

void MapEditor2D::ChangeLayerName (UIElement *Sender) {
	MapEditor->LayerEdit->Visible = false;
	MapEditor->Layers[MapEditor->LayerEdit->Id]->Name = wstring2string (MapEditor->LayerEdit->wText);
	MapEditor->LayersList->Items[MapEditor->LayerEdit->Id]->Caption = MapEditor->LayerEdit->wText;
}

void MapEditor2D::AddLayer (UIElement *Sender) {
	// Add Main Layer
	UIFlatButton *Item = new UIFlatButton (false);
	//
	Item->Caption = L"New Layer";
	Item->OnKeyPressed = EditLayer;
	Item->OnMouseDown = SelectLayer;
	Item->OnDblClick = EditLayerDbl;
	Item->OnClick = 0;
	//Item->Checked = true;
	//
	Item->ShowIcon = true;    Item->ShowIcon2 = true;
	Item->IconOffset [0] = 4; Item->IconOffset [1] = 6;
	Item->Icon2Offset[0] = 6; Item->Icon2Offset[1] = 6;
	//
	MapEditor->LayersList->AddItem (Item, MapEditor->CurLayerId);
	MapEditor->LayersList->Init();
	//
	MapEditor->CurLayerId = Item->Id;
	MapEditor->LayersList->CheckItem (Item);
	//
	Layer *NewLayer = new Layer();
	NewLayer->Name = "New Layer";
	//MapEditor->Layers.push_back (NewLayer);
	MapEditor->Layers.insert (MapEditor->Layers.begin()+MapEditor->CurLayerId, NewLayer);
	//
	//std::swap (MapEditor->Layers[MapEditor->CurLayerId], MapEditor);
}

void MapEditor2D::DeleteLayer (UIElement *Sender) {
	//
	MapEditor->Layers[MapEditor->CurLayerId]->Visible = false;
	MapEditor->LayersList->Items[MapEditor->CurLayerId]->Visible = false;
	MapEditor->LayersList->Items[MapEditor->CurLayerId]->Checked = false;
	//
	bool Checked = false;
	for (int i = MapEditor->CurLayerId+1; i < MapEditor->LayersList->Items.size(); i++) {
		//
		if (!MapEditor->LayersList->Items[i]->Visible)
			continue;
		//
		MapEditor->LayersList->LastCheck = MapEditor->LayersList->Items[i];
		MapEditor->LayersList->Items[i]->Checked = true;
		MapEditor->CurLayerId = i;
		Checked = true;
		break;
	}

	//
	if (!Checked)
		for (int i = MapEditor->CurLayerId-1; i >= 0; i--) {
			if (!MapEditor->LayersList->Items[i]->Visible)
				continue;
			//
			MapEditor->LayersList->LastCheck = MapEditor->LayersList->Items[i];
			MapEditor->LayersList->Items[i]->Checked = true;
			MapEditor->CurLayerId = i;
			Checked = true;
			break;
		}
	//
	if (!Checked) {
		MapEditor->Layers[MapEditor->CurLayerId]->Visible = true;
		MapEditor->LayersList->Items[MapEditor->CurLayerId]->Visible = true;
	}
}
// --------------------------------------------------------------------------------------------

// Mode, Toolbar ----------------------------------------------------------------------------->

void MapEditor2D::SetSpriteObject   (UIElement *Sender) { MapEditor->CurrAddTool = tSprite;   MapEditor->SetVisibileElements (false, false); }
void MapEditor2D::SetPolygonObject  (UIElement *Sender) { MapEditor->CurrAddTool = tPolygon;  MapEditor->SetVisibileElements (false, false); }
void MapEditor2D::SetLightObject    (UIElement *Sender) { MapEditor->CurrAddTool = tLight;    MapEditor->SetVisibileElements (true , false);  }
void MapEditor2D::SetPointObject    (UIElement *Sender) { MapEditor->CurrAddTool = tPoint;    MapEditor->SetVisibileElements (false, true ); }
void MapEditor2D::SetRailpolyObject (UIElement *Sender) { MapEditor->CurrAddTool = tRailpoly; MapEditor->SetVisibileElements (false, false); }
void MapEditor2D::SetSoundObject    (UIElement *Sender) { MapEditor->CurrAddTool = tSound;    MapEditor->SetVisibileElements (false, false); }
void MapEditor2D::SetTilesObject    (UIElement *Sender) { MapEditor->CurrAddTool = tTiles;    MapEditor->SetVisibileElements (false, false); }

// Mode
void MapEditor2D::SetObjectMode (UIElement *Sender) {
	MapEditor->CurrMode    = tObject;
	MapEditor->AddOnMapObj = false;
	MapEditor->SkipMouseUp = true;
	HideCursor             = false;
}

void MapEditor2D::SetEditMode (UIElement *Sender) {
	MapEditor->CurrMode    = tEdit;
	MapEditor->AddOnMapObj = false;
	MapEditor->SkipMouseUp = true;
	HideCursor             = false;
}

void MapEditor2D::SetAddMode (UIElement *Sender) {
	MapEditor->CurrMode    = tAdd;
	MapEditor->SkipMouseUp = true;

	if (MapEditor->CurrAddTool == tPolygon) {
		MapEditor->AddOnMapObj = true;
		HideCursor             = true;
	} else {
		MapEditor->AddOnMapObj = false;
		HideCursor             = false;
	}
}

// <----------------------------------------------------------------------------- Mode, Toolbar

// Set Snap -----------------------------------------------------------------------------------
void MapEditor2D::SetSnap (UIElement *Sender) {
	MapEditor->GridSnap = !MapEditor->GridSnap;
	((UICheckButton*)Sender)->Checked = MapEditor->GridSnap;
}

// --------------------------------------------------------------------------------------------

// Materials Tab ----------------------------------------------------------------------------->

void MapEditor2D::AddUniform (UIElement *Sender) {
	addUniformDialog->Open();
}

// <----------------------------------------------------------------------------- Materials Tab

// Vars Tab ----------------------------------------------------------------------------->

void MapEditor2D::AddVar (UIElement *Sender) {
	addVarDialog->Open();
}

// <----------------------------------------------------------------------------- Materials Tab

// Points -------->

BaseGameObject *MapEditor2D::AddPoint (const wstring Name) {
	UIFlatButton *Item = new UIFlatButton (false);
	//
	Item->Caption = Name;
	//Item->OnKeyPressed = EditPolygonName;
	Item->OnMouseDown = SelectPoint;
	//Item->OnDblClick = EditPolygonNameDbl;
	Item->OnClick = 0;
	//Item->Checked = true;
	//
	Item->ShowIcon = true;
	Item->IconOffset [0] = 0;
	Item->IconOffset [1] = 7;
	//
	PointsList->AddItem (Item, CurPolygonId);
	PointsList->Init();
	//
	CurPointId = Item->Id;
	PointsList->CheckItem (Item);

	PointObject *NewPoint = new PointObject();
	NewPoint->Init();

	NewPoint->mCamera = Camera;
	NewPoint->Data = 0;
	
	NewPoint->SetPosition(glm::vec2(0));
	NewPoint->SetOffset (glm::vec2 (Left, 0));
	NewPoint->SetSize(glm::vec2(1));

	NewPoint->Material = 0;
	NewPoint->JoinObj = 0;
	NewPoint->UpdateListVertices();

	PointGroupItem gItem;
	gItem.Name = Item->Caption;
	gItem.Data = NewPoint;

	PointGroupes->InsertToGroup(gItem);
	MapEditor->Layers[MapEditor->CurLayerId]->Objects.push_back (NewPoint);

	return (BaseGameObject*) NewPoint;
}

void MapEditor2D::AddPointClick (UIElement *Sender) {
	BaseGameObject *NewPoint = MapEditor->AddPoint (L"New Point");

	//
	MapEditor->Data->Elements["bdeletepoint"]->Enabled = true;
	MapEditor->AddModeButton   ->Checked = false;
	MapEditor->ObjectModeButton->Checked = true;
	MapEditor->CurrMode = tObject;
	MapEditor->CurSelMode = tPoint;
	MapEditor->UpdateModeButtons();

	// Cursor
	MapEditor->SpriteCursor = NewPoint;
	sf::Mouse::setPosition (sf::Vector2i (MapEditor->Left+(MapEditor->Width >> 1), MapEditor->Top+(MapEditor->Height >> 1)), *MainWindow);
	//
	MapEditor->SnapOffset  = false;
	MapEditor->AddOnMapObj = true;
	HideCursor = true;
}

void MapEditor2D::SelectPoint (UIElement *Sender, int x, int y, int Shift) {

}

/* Lights */

BaseGameObject *MapEditor2D::AddLight (const wstring Name) {
	UIFlatButton *Item = new UIFlatButton (false);
	//
	Item->Caption = Name;
	//Item->OnKeyPressed = EditPolygonName;
	Item->OnMouseDown = SelectLight;
	//Item->OnDblClick = EditPolygonNameDbl;
	Item->OnClick = 0;
	//Item->Checked = true;
	//
	Item->ShowIcon = true;
	Item->IconOffset [0] = 10;
	Item->IconOffset [1] = 8;
	//
	LightsList->AddItem (Item, CurPolygonId);
	LightsList->Init();
	//
	CurLightId = Item->Id;
	LightsList->CheckItem (Item);

	LightObject *NewLight = new LightObject();
	NewLight->Init();

	NewLight->mCamera = Camera;
	NewLight->Data = 0;
	NewLight->CastShadow = true;
	NewLight->Radius = 300;
	NewLight->Attenuation = 0.f;
	NewLight->Type = ltStationar;
	NewLight->Color = glm::vec4(1);
	
	NewLight->SetPosition(glm::vec2(0));
	NewLight->SetOffset (glm::vec2 (Left, 0));
	NewLight->SetSize(glm::vec2(1));

	NewLight->Material = 0;
	NewLight->UpdateListVertices();

	LightGroupItem gItem;
	gItem.Name = Item->Caption;
	gItem.Data = NewLight;

	LightGroupes->InsertToGroup(gItem);
	MapEditor->Layers[MapEditor->CurLayerId]->Objects.push_back (NewLight);

	return (BaseGameObject*) NewLight;
}

void MapEditor2D::AddLightClick (UIElement *Sender) {
	BaseGameObject *NewLight = MapEditor->AddLight (L"New Light");

	//
	MapEditor->Data->Elements["bdeletelight"]->Enabled = true;
	MapEditor->AddModeButton   ->Checked = false;
	MapEditor->ObjectModeButton->Checked = true;
	MapEditor->CurrMode = tObject;
	MapEditor->CurSelMode = tLight;
	MapEditor->UpdateModeButtons();

	// Cursor
	MapEditor->SpriteCursor = NewLight;
	sf::Mouse::setPosition (sf::Vector2i (MapEditor->Left+(MapEditor->Width >> 1), MapEditor->Top+(MapEditor->Height >> 1)), *MainWindow);
	//
	MapEditor->SnapOffset  = false;
	MapEditor->AddOnMapObj = true;
	HideCursor = true;
}

void MapEditor2D::SelectLight (UIElement *Sender, int x, int y, int Shift) {

}

/* Groupes */

void MapEditor2D::AddPointGroup        (UIElement *Sender) { MapEditor->PointGroupes->AddGroup();       }
void MapEditor2D::SetPointGroup        (UIElement *Sender) { MapEditor->PointGroupes->SetGroup(Sender); }
void MapEditor2D::ChangePointGroupName (UIElement *Sender) { MapEditor->PointGroupes->ChangeName();     }
void MapEditor2D::DeletePointGroup     (UIElement *Sender) { MapEditor->PointGroupes->DeleteGroup();    }

void MapEditor2D::AddLightGroup        (UIElement *Sender) { MapEditor->LightGroupes->AddGroup();       }
void MapEditor2D::SetLightGroup        (UIElement *Sender) { MapEditor->LightGroupes->SetGroup(Sender); }
void MapEditor2D::ChangeLightGroupName (UIElement *Sender) { MapEditor->LightGroupes->ChangeName();     }
void MapEditor2D::DeleteLightGroup     (UIElement *Sender) { MapEditor->LightGroupes->DeleteGroup();    }

// <-------- Points

// Polygons ------>

void MapEditor2D::AddPolygon (UIElement *Sender) {
	UIFlatButton *Item = new UIFlatButton (false);
	//
	Item->Caption = L"New Polygon";
	Item->OnKeyPressed = EditPolygonName;
	Item->OnMouseDown = SelectPolygon;
	Item->OnDblClick = EditPolygonNameDbl;
	Item->OnClick = 0;
	//Item->Checked = true;
	//
	Item->ShowIcon = true;
	Item->IconOffset [0] = 5;
	Item->IconOffset [1] = 3;
	//
	MapEditor->PolygonsList->AddItem (Item, MapEditor->CurPolygonId);
	MapEditor->PolygonsList->Init();
	//
	MapEditor->CurPolygonId = Item->Id;
	MapEditor->PolygonsList->CheckItem (Item);
	//
	MapEditor->Data->Elements["bdeletepolygon"]->Enabled = true;
	MapEditor->AddModeButton   ->Checked = true;
	MapEditor->ObjectModeButton->Checked = false;
	MapEditor->CurrMode = tAdd;
	MapEditor->CurSelMode = tPolygon;
	MapEditor->UpdateModeButtons();

	PolygonObject *NewPolygon = new PolygonObject();
	NewPolygon->Init();

	NewPolygon->mCamera = MapEditor->Camera;
	NewPolygon->Shader = ColorShader;
	NewPolygon->Data = SpriteDat;
	NewPolygon->Texture = Core->Theme->Skin;
	NewPolygon->SetPosition(glm::vec2(0));
	NewPolygon->SetOffset(glm::vec2 (MapEditor->Left, 0));
	NewPolygon->SetSize(glm::vec2(1));

	NewPolygon->Material->Copy (MapEditor->PolygonMaterial);

	PolygonGroupItem gItem;
	gItem.Name = Item->Caption;
	gItem.Data = NewPolygon;

	MapEditor->PolygonGroupes->InsertToGroup(gItem);
	MapEditor->SelectedObject = (BaseGameObject*) NewPolygon;
	MapEditor->PolygonsGroups[MapEditor->CurPolygonGroup].Items.push_back (gItem);

	// Cursor
	MapEditor->SpriteCursor->SetSize     (Core->TargetSize);
	MapEditor->SpriteCursor->SetOffset   (glm::vec2 (MapEditor->Left, 0));
	MapEditor->SpriteCursor->SetPosition (glm::vec2 (0, 0));

	MapEditor->SpriteCursor->tSize   = Core->TargetTexSize;
	MapEditor->SpriteCursor->tOffset = Core->TargetTexOffset;
	MapEditor->SpriteCursor->Texture = Core->Theme->Skin;
	//
	sf::Mouse::setPosition (sf::Vector2i (MapEditor->Left+(MapEditor->Width >> 1), MapEditor->Top+(MapEditor->Height >> 1)), *MainWindow);
	//
	MapEditor->SnapOffset  = false;
	MapEditor->AddOnMapObj = true;
	HideCursor = true;

	MapEditor->Layers[MapEditor->CurLayerId]->Objects.push_back ((BaseGameObject*) NewPolygon);
}

void MapEditor2D::EditPolygonName (UIElement *Sender, Uint16 key) {

}

void MapEditor2D::EditPolygonNameDbl (UIElement *Sender, int x, int y, int Shift) {

}

void MapEditor2D::ChangePolygonName (UIElement *Sender) {

}

void MapEditor2D::SelectPolygon (UIElement *Sender, int x, int y, int Shift) {

}

//
void MapEditor2D::AddPolygonGroup        (UIElement *Sender) { MapEditor->PolygonGroupes->AddGroup();       }
void MapEditor2D::SetPolygonGroup        (UIElement *Sender) { MapEditor->PolygonGroupes->SetGroup(Sender); }
void MapEditor2D::ChangePolygonGroupName (UIElement *Sender) { MapEditor->PolygonGroupes->ChangeName();     }
void MapEditor2D::DeletePolygonGroup     (UIElement *Sender) { MapEditor->PolygonGroupes->DeleteGroup();    }

// <------ Polygons

// Main Menu
// - File

void MapEditor2D::ShowPreferences (UIElement *Sender) {
	preferencesDialog->Open();
}

void MapEditor2D::Quit (UIElement *Sender) {
	Terminate (0);
}

// - Modify

void MapEditor2D::Retopology (UIElement *Sender) {
	if (MapEditor->SelectedObject == 0) return;
	retopologyDialog->Open (MapEditor->SelectedObject);
}

void MapEditor2D::Boolean (UIElement *Sender) {

}

void MapEditor2D::CenterPivot (UIElement *Sender) {
	MapEditor->LockPivot = false;
}

void MapEditor2D::JoinPointToVertex (UIElement *Sender) {
	for (int i = 0; i < MapEditor->SelectedVertices.size(); i++) {
		const int vid = MapEditor->SelectedVertices[i];
		PointObject *Point = (PointObject*) MapEditor->AddPoint (L"J_V"+IntToWStr(vid));

		Point->Position = glm::vec2 (MapEditor->SelectedObject->VertexAbsPos[vid].x,
									 MapEditor->SelectedObject->VertexAbsPos[vid].y)-Point->Offset;

		Point->JoinObj = MapEditor->SelectedObject;
		Point->VertexJoinId = vid;
		Point->UpdatePosVertices();
		MapEditor->SelectedObject->JoinedPoints.push_back (Point);
	}
}

// Operations with Sprites --------------------------------------------------------------------
//

void MapEditor2D::AddSpriteFile (UIElement *Sender) {
	finder->Dialog->Left = (WindowWidth  >> 1) - (finder->Dialog->Width  >> 1);
	finder->Dialog->Top  = (WindowHeight >> 1) - (finder->Dialog->Height >> 1);
	finder->Open (ftOpenPicture, AddSpriteFormOpen);
}

void MapEditor2D::SelectSprite (UIElement *Sender, int x, int y, int Shift) {
	UITallFlatButton *This = (UITallFlatButton*) Sender;
	MapEditor->CurSpriteId = Sender->Id;

	if (!This->Checked) {
		MapEditor->eSizeW->wText = IntToWStr(This->SpriteW);
		MapEditor->eSizeH->wText = IntToWStr(This->SpriteH);
		MapEditor->eAngle->wText = L"0.000";
	}

	if (Sender->CustomData != 0) {
		MapEditor->SelectedObject = (BaseGameObject*) Sender->CustomData;
		MapEditor->SelectedObject->DisplayParams();
	}
}

void MapEditor2D::AddSpriteOnMap (UIElement *Sender, int x, int y, int Shift) {
	//
	UITallFlatButton *This = (UITallFlatButton*) Sender;
	//
	if (!MapEditor->Layers[MapEditor->CurLayerId]->Visible) // Show Error or Warning
		return;

	MapEditor->SpriteCursor = (BaseGameObject*) Sender->CustomData;
	sf::Mouse::setPosition (sf::Vector2i (MapEditor->Left+(MapEditor->Width >> 1), MapEditor->Top+(MapEditor->Height >> 1)), *MainWindow);
	//
	MapEditor->AddOnMapObj = true;
	HideCursor = true;
}

void MapEditor2D::AddSpriteGroup (UIElement *Sender) {
	MapEditor->UpdateSpriteGroup();
}

// --------------------------------------------------------------------------------------------
//
void MapEditor2D::Init() {
	//
	GridColor[0] = Core->Theme->Data["general"].Func["gridcolor"][0].Num/255.0f;
	GridColor[1] = Core->Theme->Data["general"].Func["gridcolor"][1].Num/255.0f;
	GridColor[2] = Core->Theme->Data["general"].Func["gridcolor"][2].Num/255.0f;

	SelectColor[0] = Core->Theme->Data["general"].Func["selectcolor"][0].Num/255.0f;
	SelectColor[1] = Core->Theme->Data["general"].Func["selectcolor"][1].Num/255.0f;
	SelectColor[2] = Core->Theme->Data["general"].Func["selectcolor"][2].Num/255.0f;

	CameraColor[0] = Core->Theme->Data["general"].Func["cameracolor"][0].Num/255.0f;
	CameraColor[1] = Core->Theme->Data["general"].Func["cameracolor"][1].Num/255.0f;
	CameraColor[2] = Core->Theme->Data["general"].Func["cameracolor"][2].Num/255.0f;

	RoomBorderColor[0] = Core->Theme->Data["general"].Func["roombordercolor"][0].Num/255.0f;
	RoomBorderColor[1] = Core->Theme->Data["general"].Func["roombordercolor"][1].Num/255.0f;
	RoomBorderColor[2] = Core->Theme->Data["general"].Func["roombordercolor"][2].Num/255.0f;
	//
	Data->Elements["baddsprite"]  ->OnClick = AddSpriteFile;
	Data->Elements["baddlayer"]   ->OnClick = AddLayer;
	Data->Elements["bdeletelayer"]->OnClick = DeleteLayer;
	Data->Elements["dmsnap"]	  ->OnClick = SetSnap;
	Data->Elements["baddpolygon"] ->OnClick = AddPolygon;
	Data->Elements["baddpoint"]   ->OnClick = AddPointClick;
	Data->Elements["baddlight"]   ->OnClick = AddLightClick;
	//
	//Data->Elements["spriteelement"] ->OnClick = SetSpriteObject;
	//Data->Elements["polygonelement"]->OnClick = SetPolygonObject;

	// Group
	Data->Elements["baddspritegroup"] ->OnClick = AddSpriteGroup;
	Data->Elements["baddpolygongroup"]->OnClick = AddPolygonGroup;
	Data->Elements["bdeletepolygongroup"]->OnClick = DeletePolygonGroup;
	Data->Elements["lipolygonsmaingroup"]->OnClick = SetPolygonGroup;
	Data->Elements["epolygongroupname"]->OnUnFocused = ChangePolygonGroupName;

	Data->Elements["spriteelement"]  ->OnClick = SetSpriteObject;
	Data->Elements["polygonelement"] ->OnClick = SetPolygonObject;
	//Data->Elements["railpolyelement"]->OnClick = SetRailpolyObject;
	Data->Elements["pointelement"]   ->OnClick = SetPointObject;
	Data->Elements["lightelement"]   ->OnClick = SetLightObject;
	Data->Elements["soundelement"]   ->OnClick = SetSoundObject;
	Data->Elements["tileselement"]   ->OnClick = SetTilesObject;

	
	// Tool Panel
	Data->Elements["iselect"]->OnClick = SetToolMove;
	Data->Elements["irotate"]->OnClick = SetToolRotate;
	Data->Elements["iscale"] ->OnClick = SetToolScale;

	// Mode Panel
	Data->Elements["dmobjectmode"]->OnClick = SetObjectMode;
	Data->Elements["dmeditmode"]  ->OnClick = SetEditMode;
	Data->Elements["dmaddmode"]   ->OnClick = SetAddMode;

	// Main Tab
	((UIEdit*) Data->Elements["eclass"])->OnChange = ChangeClassName;

	/* Light */
	Data->Elements["clightcolor"] ->OnClick = SetLightColor;
	Data->Elements["bltdynamic"]  ->OnClick = SetDynamicLightType;
	Data->Elements["bltstatic"]   ->OnClick = SetStaticLightType;
	Data->Elements["bltstationar"]->OnClick = SetStationarLightType;
	Data->Elements["cbcastshadow"]->OnClick = SetLightCastShadowType;

	// Material Tab
	Data->Elements["badduniform"]->OnClick = AddUniform;
	Data->Elements["baddvar"]    ->OnClick = AddVar;

	// Main Menu
	// - File
	Data->Elements["lipreferences"]->OnClick = ShowPreferences;
	Data->Elements["liquit"]       ->OnClick = Quit;

	// - Modify
	Data->Elements["liretopology"]       ->OnClick = Retopology;
	Data->Elements["liboolean"]          ->OnClick = Boolean;
	Data->Elements["licenterpivot"]      ->OnClick = CenterPivot;
	Data->Elements["lijoinpointtovertex"]->OnClick = JoinPointToVertex;

	/*Data->Elements["iselectrect"]->OnClick = SetToolSelRect;
	//
	Data->Elements["iaddsprite"] ->OnClick = SetToolAddSprite;
	Data->Elements["iaddpolygon"]->OnClick = SetToolAddSurface;
	Data->Elements["iaddlight"]  ->OnClick = SetToolAddLight;
	Data->Elements["iaddmark"]   ->OnClick = SetToolAddCoord;
	//
	Data->Elements["imove"]  ->OnClick = SetToolMove;
	Data->Elements["irotate"]->OnClick = SetToolRotate;
	Data->Elements["iscale"] ->OnClick = SetToolScale;*/
	
	// Set Variables from Data
	SpritesList		= (UIListMenu*) Data->Elements["lmsprites"];
	PolygonsList	= (UIListMenu*) Data->Elements["lmpolygons"];
	PointsList		= (UIListMenu*) Data->Elements["lmpoints"];
	LightsList		= (UIListMenu*) Data->Elements["lmlights"];

	MultimediaList 	= (UIListMenu*) Data->Elements["lmmultimedia"];
	LayersList		= (UIListMenu*) Data->Elements["lmlayers"];
	LayerEdit		=     (UIEdit*) Data->Elements["eeditlayer"];
	//
	SelectTool		= 	 (UIImage*) Data->Elements["itoolsel"];
	//AddTool	 		= 	 (UIImage*) Data->Elements["itooladd"];
	//TransformTool	= 	 (UIImage*) Data->Elements["itooltransform"];
	//
	SnapButton		= (UICheckButton*) Data->Elements["dmsnap"];
	//
	SpriteTabs    = (UIPanel*) Data->Elements["pspritetabs"];
	SurfaceTabs   = (UIPanel*) Data->Elements["psurfacetabs"];
	SpritesPanel  = (UIPanel*) Data->Elements["pspritegroups"];
	PolygonsPanel = (UIPanel*) Data->Elements["ppolygongroups"];
	DetailsPanel  = (UIPanel*) Data->Elements["pdetails"];
	//
	SnapElements = (UIGroupedElements*) Data->Elements["gbsnap"];
	ModeElements = (UIGroupedElements*) Data->Elements["gbmode"];
	//
	SpriteGoupsList  = (UIListMenu*) MapEditor->Data->Elements["lmspritegroupes"];
	PolygonGoupsList = (UIListMenu*) MapEditor->Data->Elements["lmpolygongroupes"];
	PointGoupsList   = (UIListMenu*) MapEditor->Data->Elements["lmpointgroupes"];
	LightGoupsList   = (UIListMenu*) MapEditor->Data->Elements["lmlightgroupes"];
	//
	ObjectModeButton = (UICheckButton*) MapEditor->Data->Elements["dmobjectmode"];
	EditModeButton   = (UICheckButton*) MapEditor->Data->Elements["dmeditmode"];
	AddModeButton    = (UICheckButton*) MapEditor->Data->Elements["dmaddmode"];
	//
	// Sprite Params
	eSizeW = (UIEdit*) Data->Elements["esizew"];
	eSizeH = (UIEdit*) Data->Elements["esizeh"];
	eAngle = (UIEdit*) Data->Elements["eangle"];
	
	// Set Default
	LayerEdit->OnUnFocused = ChangeLayerName;
	
	// Init Main Groups ----------------->

	//SpriteGroupes  = new SpriteGroupesT();
	UIEdit   *GroupName         =   (UIEdit*) MapEditor->Data->Elements["epolygongroupname"];
	UIButton *GroupDeleteButton = (UIButton*) MapEditor->Data->Elements["bdeletepolygongroup"];

	PolygonGroupes = new BaseGroupes<PolygonGroupItem>(PolygonGoupsList, PolygonsList, GroupName, GroupDeleteButton);
	PolygonGroupes->OnSetGroupEvent = SetPolygonGroup;

	GroupName         =   (UIEdit*) MapEditor->Data->Elements["epointgroupname"];
	GroupDeleteButton = (UIButton*) MapEditor->Data->Elements["bdeletepointgroup"];

	PointGroupes = new BaseGroupes<PointGroupItem>(PointGoupsList, PointsList, GroupName, GroupDeleteButton);
	PointGroupes->OnSetGroupEvent = SetPointGroup;

	GroupName         =   (UIEdit*) MapEditor->Data->Elements["elightgroupname"];
	GroupDeleteButton = (UIButton*) MapEditor->Data->Elements["bdeletelightgroup"];

	LightGroupes = new BaseGroupes<LightGroupItem>(LightGoupsList, LightsList, GroupName, GroupDeleteButton);
	LightGroupes->OnSetGroupEvent = SetLightGroup;

	/*SpriteGroup MainSpriteGroup;
	CurSpriteGroup = L"Main";
	MainSpriteGroup.Name = CurSpriteGroup;
	SpritesGroups[CurSpriteGroup] = MainSpriteGroup;

	PolygonGroup MainPolygonGroup;
	CurPolygonGroup = L"Main";
	MainPolygonGroup.Name = CurPolygonGroup;
	PolygonsGroups[CurPolygonGroup] = MainPolygonGroup;*/

	// <----------------- Init Main Groups

	// Init UI Data
	VarsUIData     = new UIData();
	UniformsUIData = new UIData();

	VarsUIData    ->Manager = Core->GUIManager;
	UniformsUIData->Manager = Core->GUIManager;

	VarsUIData->LoadFromFile(("Data/"+lang+"/VarObjParams.thm").c_str());
	//
	SpriteMaterial = new BaseMaterial(TexAtlasShader);
	SpriteMaterial->Uniforms->AddMat4f(L"Модельно видовая матрица", "MVP");
	SpriteMaterial->Uniforms->AddTex2D(L"Текстура", "Texture");
	SpriteMaterial->Uniforms->AddVec2f(L"Размер", "Size"  , glm::vec2(1));
	SpriteMaterial->Uniforms->AddVec2f(L"Смещение", "Offset", glm::vec2(0));
	SpriteMaterial->Uniforms->AddFloat(L"Прозрачность", "Alpha" , 1.0f);
	//
	PolygonMaterial = new BaseMaterial(ColorShader);
	PolygonMaterial->Uniforms->AddMat4f(L"Модельно видовая матрица", "MVP");
	PolygonMaterial->Uniforms->AddVec3f(L"Цвет", "Color", glm::vec3(1.0f, 1.0f, 1.0f));
	//
	GlobalVars = new BaseUniforms();
	//GlobalVars->AddTex2D(L"Текстура", "Texture");
	//GlobalVars->AddVec2f(L"Размер области", "AtlasSize");
	//GlobalVars->AddVec2f(L"Смещение области", "AtlasOffset");
	//GlobalVars->AddColor4f("Color");

	SpriteCursor = new BaseGameObject();
	SpriteCursor->Init();
	//
	SpriteCursor->Data    = SpriteDat;
	SpriteCursor->Shader  = TexAtlasShader;
	SpriteCursor->mCamera = Camera;

	SPCData = SpriteCursor;
	
	//
	VertexObject = new BaseObject();
	VertexObject->Init();

	VertexObject->Data = SpriteDat;
	VertexObject->Shader  = TexAtlasShader;
	VertexObject->mCamera = Camera;
	//
	//VertexObject->SetSize (glm::vec2(Core->VertexSize[0], Core->VertexSize[1]));
	VertexObject->SetSize (glm::vec2(6, 6));
	//
	// Add Main Layer ----------------------------
	UIFlatButton *Item = new UIFlatButton (false);
	//
	Item->Caption = L"Main";
	Item->OnKeyPressed = EditLayer;
	Item->OnMouseDown = SelectLayer;
	Item->OnDblClick = EditLayerDbl;
	Item->OnClick = 0;
	Item->Checked = true;
	//
	Item->ShowIcon = true;    Item->ShowIcon2 = true;
	Item->IconOffset [0] = 4; Item->IconOffset [1] = 6;
	Item->Icon2Offset[0] = 6; Item->Icon2Offset[1] = 6;
	//
	LayersList->AddItem (Item);
	LayersList->Init();
	//
	Layer *MainLayer = new Layer();
	MainLayer->Name = "Main";
	//	
	Layers.push_back (MainLayer);

	SpriteCursor->CopyFromDataRender();
	//Edge *e = new Edge(glm::vec2(-6, 6), glm::vec2(6, 6));
	//SpriteCursor->Divide(2, 2);
	//SpriteCursor->Cut(e);
	//SpriteCursor->Triangulate5();

	/*PolygonT *p = new PolygonT();

	p->Insert(glm::vec2(0,0));
	p->Insert(glm::vec2(0,10));
	p->Insert(glm::vec2(10,0));*/

	/*if (PointInPolygon(glm::vec2(8,2), p) == classify::inside) {
		LOG_DEBUG("INSIDE\n");
		exit(0);
	}*/
	// Init Material Library
}

void MapEditor2D::UpdateTransformPivot() {
	
	if (LockPivot) return;
	if (SelectedObject == 0) return;
	if (!GroupedPivot || (SelectedObjects .size() == 1 && CurrMode == tObject) ||
						 (SelectedVertices.size() <= 1 && CurrMode == tEdit))
	{
		
		if (CurrMode == tObject){

			TransformPivot = SelectedObject->Position+SelectedObject->Pivot;

		}

		if (CurrMode == tEdit) {

			if (SelectedVertices.size() == 0) {

				TransformPivot = SelectedObject->Position+SelectedObject->Pivot;

			} else {

				const int vid  = SelectedVertices[0];
				TransformPivot = glm::vec2(SelectedObject->VertexAbsPos[vid])-SelectedObject->Offset;

			}

		}
	
	} else {

		glm::vec2 MinPos, MaxPos;

		if (CurrMode == tObject) {

			MinPos = SelectedObject->Position;
			MaxPos = SelectedObject->Position;

			for (int i = 0; i < SelectedObjects.size(); i++) {

				MinPos.x = min (MinPos.x, SelectedObjects[i]->Position.x);
				MinPos.y = min (MinPos.y, SelectedObjects[i]->Position.y);
				MaxPos.x = max (MaxPos.x, SelectedObjects[i]->Position.x);
				MaxPos.y = max (MaxPos.y, SelectedObjects[i]->Position.y);

			}

		}

		if (CurrMode == tEdit) {

			MinPos = glm::vec2(SelectedObject->VertexAbsPos[SelectedVertices[0]]);
			MaxPos = MaxPos;

			for (int i = 0; i < SelectedVertices.size(); i++) {

				const int vid = SelectedVertices[i];

				MinPos.x = min (MinPos.x, SelectedObject->VertexAbsPos[vid].x);
				MinPos.y = min (MinPos.y, SelectedObject->VertexAbsPos[vid].y);
				MaxPos.x = max (MaxPos.x, SelectedObject->VertexAbsPos[vid].x);
				MaxPos.y = max (MaxPos.y, SelectedObject->VertexAbsPos[vid].y);

			}

			MinPos -= SelectedObject->Offset;
			MaxPos -= SelectedObject->Offset;
		}

		float hx = (MaxPos.x - MinPos.x) * 0.5;
		float hy = (MaxPos.y - MinPos.y) * 0.5;

		TransformPivot.x = MinPos.x + hx;
		TransformPivot.y = MinPos.y + hy;

	}

	TransformPivotScreen.x = TransformPivot.x-Camera->Position.x+SelectedObject->Offset.x;
	TransformPivotScreen.y = WindowHeight-(TransformPivot.y-Camera->Position.y+SelectedObject->Offset.y);

}

// Events

void MapEditor2D::DblClick  (int x, int y, int Shift) {

}

void MapEditor2D::MouseDown (int x, int y, int Shift) {
	dx = x-mx; dy = y-my;
	cx = x; cy = y;
	MouseWay = glm::vec2(0, 0);
	tmpAng = 0;

	if (SelectedObject != 0) {
		/*switch (CurrTransformTool) {
			case tMove   :  break;
			case tScale  :  break;
		}*/

		int snx = round (x+Camera->Position[0]);
		int sny = round (WindowHeight-y+Camera->Position[1]);
		//
		ClickedOnSO = false;

		for (int i = 0; i < SelectedObjects.size(); i++) {

			if (PointInPolygon(glm::vec2(snx, sny), SelectedObjects[i]->SelectPoly)) {
				ClickedOnSO = true;
				SelectedObject = SelectedObjects[i];

				if (!GroupedPivot) {
					UpdateTransformPivot();
					lastAng = atan2(SelectedObjects[i]->WorldToScreen().x-x, WindowHeight-SelectedObjects[i]->WorldToScreen().y-y);
				}

			}

			SelectedObjects[i]->Size0   = SelectedObjects[i]->Size;
			SelectedObjects[i]->Pos0    = SelectedObjects[i]->Position;
			SelectedObjects[i]->lastAng = SelectedObjects[i]->Rotation;
		}

		if (GroupedPivot) {
			lastAng = atan2(TransformPivotScreen.x-x, TransformPivotScreen.y-y);
		}

		for (int i = 0; i < SelectedObject->JoinedPoints.size(); i++) {
			PointObject *P = SelectedObject->JoinedPoints[i];
			P->dPositionLast = P->Position;
		}

		/* Find Area for Scale Tool */

		if (CurrTransformTool == tScale) {
			glm::vec2 spos = SelectedObject->WorldToScreen (TransformPivot);
			spos.y = WindowHeight-spos.y;

			/* */

			if (x > spos.x && y < spos.y) ScaleArea = 0;
			if (x < spos.x && y > spos.y) ScaleArea = 1;
			if (x > spos.x && y > spos.y) ScaleArea = 2;
			if (x < spos.x && y < spos.y) ScaleArea = 3;
		}
	}
	//
	if (!PointInRect (x, y, Left, Top, Width, Height-28))
		return;
	else mShift = Shift; 
	//if (PointInRect (mx, my, Left, Top, Width, Height))
	//	mShift = Shift;
	//
	mx = x; my = y;

	if (!AddOnMapObj || isKey[keyRAlt] || isKey[keyLAlt]) return;
	if (CurrAddTool == tSprite) {
		//
		BaseGameObject *Object;
		//
		Object = new BaseGameObject();
		Object->Init();
		//
		Object->Data      = SpriteDat;
		Object->Shader    = TexAtlasShader;
		Object->mCamera   = Camera;
		Object->Texture   = SpriteCursor->Texture;
		Object->Size      = SpriteCursor->Size;
		Object->Position  = SpriteCursor->Position;
		Object->Rotation  = SpriteCursor->Rotation;
		Object->Offset    = SpriteCursor->Offset;
		Object->TexOffset = glm::vec2 (0, 0);
		Object->TexSize   = glm::vec2 (1, 1);
		Object->tSize     = SpriteCursor->tSize;
		Object->tOffset   = SpriteCursor->tOffset;
		Object->Template  = SpriteCursor;
		Object->GameClass = SpriteCursor->GameClass;
		//
		//Object->PolysList  = SpriteCursor->PolysList;
		for (int i = 0; i < SpriteCursor->PolysList.size(); i++) {
			PolyData *P = new PolyData();
			P->Copy(SpriteCursor->PolysList[i]);
			Object->PolysList.push_back(P);
		}

		Object->LabelsList = SpriteCursor->LabelsList;
		//
		Object->Material->Copy (SpriteMaterial);

		Object->Material->Uniforms->SetTex2D ("Texture", Object->Texture);
		Object->Material->Uniforms->SetVec2f ("Size"   , Object->tSize);
		Object->Material->Uniforms->SetVec2f ("Offset" , Object->tOffset);
		//
		if (Object->Vars->Exist("Texture"))
			Object->Vars->SetTex2D("Texture", Object->Texture);
		//
		Object->UpdateListVertices();
		//
		SelectedObject = Object;
		SelectedObject->DisplayParams();
		Layers[CurLayerId]->Objects.push_back (Object);
		CurSelMode = tSprite;
		UpdateModeButtons();
		//
		if (Shift == mouseRight)
			return;
		//
		AddOnMapObj = false;
		HideCursor  = false;

		SpriteCursor = SPCData;

		//SpriteCursor->CopyFromDataRender();
		//Edge *e = new Edge(glm::vec2(-6, -6), glm::vec2(6, 6));
		//SpriteCursor->Divide(0, 2);
		//SpriteCursor->Divide(100, 50);
		//SpriteCursor->Triangulate5();
		//delete e;
	}
	//
	if (SelectedObject != 0 && CurSelMode == tPolygon && CurrMode == tAdd) {
		PolygonObject *p = (PolygonObject*) SelectedObject;
		p->AddVertex(SpriteCursor->Position);
		p->UpdateListVertices();
	}

	if (CurrAddTool == tPoint) {
		AddOnMapObj  = false;
		HideCursor   = false;
		SelectedObject = SpriteCursor;
		SelectedObject->UpdateListVertices();
		SpriteCursor = SPCData;
	}

	if (CurrAddTool == tLight) {
		AddOnMapObj  = false;
		HideCursor   = false;
		SelectedObject = SpriteCursor;
		SelectedObject->UpdateListVertices();
		SpriteCursor = SPCData;
	}
}

void MapEditor2D::MouseMove (int x, int y, int Shift) {
	dx = x-mx; dy = y-my;
	mx = x; my = y;
	//
	if (Shift != mouseLeft) return; 
	//
	if (isKey[keyRAlt] || isKey[keyLAlt]) {
		Camera->Position[0] -= dx/Camera->Zoom;
		Camera->Position[1] += dy/Camera->Zoom;
		Camera->NeedUpdate = true;
		//UpdateTransformPivot();
		return;
	}

	// Transform
	if (CurrMode != tObject) return;
	if (SelectedObject == 0) return;
	if (DialogOpened) return;
	if (!ClickedOnSO) return;

	/* */
	float dang = 0;

	if (CurrTransformTool == tRotate) {
		MouseWay.x += dx;
		MouseWay.y -= dy;

		if (GridSnap) {
			
		} else {

			if (!GroupedPivot || SelectedObjects.size() == 1) {

				float ang = atan2(SelectedObject->WorldToScreen().x-x, WindowHeight-SelectedObject->WorldToScreen().y-y);

				dang = (ang-lastAng);
				SelectedObject->Rotation += dang;

				for (int i = 0; i < SelectedObject->JoinedPoints.size(); i++) {

					PointObject *P = SelectedObject->JoinedPoints[i];

					glm::vec2 a1 = SelectedObject->Position;
					glm::vec2 a2 = P->Position;

					float angle = SelectedObject->Rotation;
					float x = a1.x + (a2.x-a1.x) * cos (angle) + (a2.y-a1.y) * sin (angle);
	           		float y = a1.y - (a2.x-a1.x) * sin (angle) + (a2.y-a1.y) * cos (angle);

	           		glm::vec2 nPos = glm::vec2(x, y);

				}

				lastAng = ang;

			} else {

				float ang = atan2 (TransformPivotScreen.x-x, TransformPivotScreen.y-y);

				dang = (ang-lastAng);
				tmpAng += dang;
				lastAng = ang;

			}
		}
		
		SelectedObject->NeedUpdate = true;
		SelectedObject->UpdatePosVertices();
		//HideCursor = true;
		SkipMouseUp = true;
	}

	glm::vec2 smin = SelectedObject->Position;
	glm::vec2 smax = SelectedObject->Position;

	for (int i = 0; i < SelectedObjects.size(); i++) {
		BaseGameObject *Obj = SelectedObjects[i];

		smin = min (smin, Obj->Position-Obj->Size*0.5f);
		smax = max (smax, Obj->Position+Obj->Size*0.5f);
	}

	/* */

	for (int i = 0; i < SelectedObjects.size(); i++) {

		BaseGameObject *Obj = SelectedObjects[i];

		if (CurrTransformTool == tMove) {
			
			UpdateTransformPivot();

			if (isKey[keyRShift] || isKey[keyLShift]) {
				if (!PosSnaped) {
					PosSnaped = true;

					if (abs(dx) > abs(dy)) dSnapPos = glm::vec2(1, 0);
					if (abs(dy) > abs(dx)) dSnapPos = glm::vec2(0, 1);
				}

			}

			int snx = round (x-(cx-Obj->WorldToScreen(Obj->Pos0).x)-((int) Obj->Size.x >> 1)+(StepGrid >> 1)+Camera->Position.x-Left);
			int sny = round (WindowHeight-y-(WindowHeight-cy-Obj->WorldToScreen(Obj->Pos0).y)-((int) Obj->Size.y >> 1)+(StepGrid >> 1)+Camera->Position.y);

			int sox = 0; int soy = 0;

			if (SnapOffset) {
				sox = ((int) Obj->Size.x >> 1);
				soy = ((int) Obj->Size.y >> 1);
			}

			if (GridSnap) {
				if (PosSnaped) {
					if (dSnapPos.x == 0) Obj->Position.y = sny - sny % StepGrid + soy;
					if (dSnapPos.y == 0) Obj->Position.x = snx - snx % StepGrid + sox;
				} else {
					Obj->Position.x = snx - snx % StepGrid + sox;
					Obj->Position.y = sny - sny % StepGrid + soy;
				}
			} else {
				if (PosSnaped) {
					Obj->Position.x += dx*dSnapPos.x;
					Obj->Position.y -= dy*dSnapPos.y;
				} else {
					Obj->Position.x += dx;
					Obj->Position.y -= dy;
				}
			}

		}

		/* Rotate */

		if (CurrTransformTool == tRotate) {

			if (!GroupedPivot || SelectedObjects.size() == 1) {
			
				if (Obj != SelectedObject)
					Obj->Rotation += dang;
			
			} else {

				Obj->Position = rot2D (TransformPivot, Obj->Pos0, -tmpAng);

	           	/* */

	           	if (!OnlyTranslate) {

					glm::vec2 NormPos = glm::vec2(Obj->Pos0.x, Obj->Pos0.y+1);
					NormPos = rot2D (Obj->Pos0, NormPos, -Obj->lastAng);
					NormPos = rot2D (TransformPivot, NormPos, -tmpAng);

					float newAng = atan2 (NormPos.x-Obj->Position.x, NormPos.y-Obj->Position.y);
					Obj->Rotation = -newAng;

	           	}

			}

		}

		/* Scale */

		if (CurrTransformTool == tScale) {

			/*if (!GroupedPivot || SelectedObjects.size() == 1) {
				glm::mat4 mt  = glm::translate (glm::mat4(1.0f), glm::vec3(dx, dy, 0));
				glm::mat4 mr  = glm::rotate    (glm::mat4(1.0f), SelectedObject->Rotation, glm::vec3(0,0,1));
				glm::mat4 pres = mr*mt;

				glm::vec2 pn = glm::vec2(SelectedObject->WorldToScreen(SelectedObject->Pos0).x+cos(SelectedObject->Rotation)*9999,
									     SelectedObject->WorldToScreen(SelectedObject->Pos0).y+sin(SelectedObject->Rotation)*9999);

				glm::vec2 p0 = glm::vec2(SelectedObject->WorldToScreen(SelectedObject->Pos0).x-cos(SelectedObject->Rotation)*9999,
										 SelectedObject->WorldToScreen(SelectedObject->Pos0).y-sin(SelectedObject->Rotation)*9999);

				if (ClassifyPoint(glm::vec2(cx, WindowHeight-cy), p0, pn) == classify::right) pres[3][1] *= -1;

				pn = glm::vec2(SelectedObject->WorldToScreen(SelectedObject->Pos0).x+cos(SelectedObject->Rotation+pi/2)*9999,
							   SelectedObject->WorldToScreen(SelectedObject->Pos0).y+sin(SelectedObject->Rotation+pi/2)*9999);

				p0 = glm::vec2(SelectedObject->WorldToScreen(SelectedObject->Pos0).x-cos(SelectedObject->Rotation+pi/2)*9999,
							   SelectedObject->WorldToScreen(SelectedObject->Pos0).y-sin(SelectedObject->Rotation+pi/2)*9999);

				if (ClassifyPoint(glm::vec2(cx, WindowHeight-cy), p0, pn) == classify::left) pres[3][0] *= -1;

				float dx, dy;

				if (isKey[keyRShift] || isKey[keyLShift]) {
					if (fabs(pres[3][0]) > fabs(pres[3][1])) {
						MouseWay.x += pres[3][0];
						MouseWay.y += pres[3][0]*(SelectedObject->Size0.y/SelectedObject->Size0.x);
					} else {
						MouseWay.x -= pres[3][1];
						MouseWay.y -= pres[3][1]*(SelectedObject->Size0.y/SelectedObject->Size0.x);
					}
				} else {
					MouseWay.x += pres[3][0];
					MouseWay.y -= pres[3][1];
				}

				if (GridSnap) {
					dx = (int(MouseWay.x) - int(MouseWay.x) % StepGrid);
					dy = (int(MouseWay.y) - int(MouseWay.y) % StepGrid);
				} else {
					dx = MouseWay.x;
					dy = MouseWay.y;
				}

				Obj->Size.x = Obj->Size0.x+dx;
				Obj->Size.y = Obj->Size0.y+dy;
			} else*/ {

				vector<glm::vec2> Points;
				glm::vec2 ScaleFactor;

				/* Auxiliary Points */

				Points.push_back (Obj->Position); // Center

				if (!OnlyTranslate) {
					Points.push_back (Obj->Position+Obj->Size*0.5f);
					Points.push_back (Obj->Position-Obj->Size*0.5f);
					Points.push_back (Obj->Position+glm::vec2(-Obj->Size.x*0.5f, Obj->Size.y*0.5f));
				}

				/* Translate auxiliary points from TransformPivot */

				for (int i = 0; i < Points.size(); i++) {

					Points[i] = rot2D (Points[0], Points[i], -Obj->Rotation);

					switch (ScaleArea) {
						case 0 : ScaleFactor = (Points[i]-TransformPivot) / (smax-TransformPivot);                      break;
						case 1 : ScaleFactor = (Points[i]-TransformPivot) / (smin-TransformPivot);                      break;
						case 2 : ScaleFactor = (Points[i]-TransformPivot) / (glm::vec2(smax.x, smin.y)-TransformPivot); break;
						case 3 : ScaleFactor = (Points[i]-TransformPivot) / (glm::vec2(smin.x, smax.y)-TransformPivot); break;
					}

					Points[i].x += dx*ScaleFactor.x;
					Points[i].y -= dy*ScaleFactor.y;

					DebugPoints[i].x = Points[i].x;
					DebugPoints[i].y = Points[i].y;
				}

				if (!OnlyTranslate) {
					float w = glm::length(Points[1]-Points[3]);
					float h = glm::length(Points[2]-Points[3]);

					Obj->Size.x = w;
					Obj->Size.y = h;
				}

				Obj->Position = Points[0];//Points[0]-newSize/2.f;

			}

		}

		/* End */

		Obj->NeedUpdate = true;
		Obj->UpdatePosVertices();	

	}

	SkipMouseUp = true;
	return;
}

void MapEditor2D::MouseUp (int x, int y, int Shift) {
	mShift = mouseNone;
	PosSnaped = false;

	if (DialogOpened) return;
	if (SkipMouseUp) {
		HideCursor  = false;
		SkipMouseUp = false;
		return;
	}

	if (!ClickedOnSO) {

		if (!PointInRect (x, y, Left, Top, Width, Height-28)) return;
		if (Shift != mouseLeft || isKey[keyRAlt] || isKey[keyLAlt]) return;
		
		if (SelectedObject != 0) {
			for (int i = 0; i < SelectedObject->JoinedPoints.size(); i++) {
				PointObject *P = SelectedObject->JoinedPoints[i];
				P->dPositionLast = P->Position;
			}
		}

	}

	if (CurrMode == tObject) SelectObject (x, y);
	if (CurrMode == tEdit)   SelectVertices();

}

void MapEditor2D::MouseWheel (int delta, int x, int y) {
	if (SelectedObject != 0 && dynamic_cast<LightObject*>(SelectedObject)) {
		LightObject *Light = (LightObject*) SelectedObject;
		Light->Radius += delta*10;
	}

	if (SpriteCursor != 0 && dynamic_cast<LightObject*>(SpriteCursor)) {
		LightObject *Light = (LightObject*) SpriteCursor;
		Light->Radius += delta*10;
	}
}
//
void MapEditor2D::KeyDown (int key) {

}

void MapEditor2D::KeyPressed (Uint16 key) {
	if (isKey[keyLCtrl] && key == keyS) {
		SaveToTextFile ("../../Levels/test.thm");
		SaveToBinFile  ("../../Levels/test.lv");
	}

	if (key == keyEscape) {
		if (CurrAddTool == tPoint || CurrAddTool == tLight) {
			SelectedObject->Visible = false;
			SelectedObject->Deleted = true;
			SelectedObject = 0;
		}

		AddOnMapObj = false;
		HideCursor  = false;
		CurrMode = tObject;
		//SelectedObject->Triangulate5();
	}

	if (key == keyDelete) {
		if (SelectedObject != 0) {
			
			for (int i = 0; i < SelectedObjects.size(); i++) {

				SelectedObjects[i]->Visible = false;
				SelectedObjects[i]->Deleted = true;

			}

			SelectedObjects.clear();
			SelectedObject = 0;

		}
	}

	if ((isKey[keyRCtrl] || isKey[keyLCtrl]) && key == keyAdd) {
	//if (key == keyAdd) {
		Camera->Zoom += 0.1f;
	}

	//if ((isKey[keyRCtrl] || isKey[keyLCtrl]) && isKey[keySub]) {
	if ((isKey[keyRCtrl] || isKey[keyLCtrl]) && key == keySub) {
		Camera->Zoom -= 0.1f;
	}

	if (SelectedObject == 0) return;
	switch (CurrTransformTool) {
		case tMove   : 
			if (key == keyLeftArrow ) SelectedObject->Position.x--;
			if (key == keyRightArrow) SelectedObject->Position.x++;
			if (key == keyDownArrow ) SelectedObject->Position.y--;
			if (key == keyUpArrow   ) SelectedObject->Position.y++;
			break;
		
		case tRotate :
			if (key == keyLeftArrow ) SelectedObject->Rotation += pi/180.f;
			if (key == keyRightArrow) SelectedObject->Rotation -= pi/180.f;
			if (key == keyDownArrow ) SelectedObject->Rotation += pi/180.f;
			if (key == keyUpArrow   ) SelectedObject->Rotation -= pi/180.f;

			break;

		case tScale  : 
			if (key == keyLeftArrow ) SelectedObject->Size.x--;
			if (key == keyRightArrow) SelectedObject->Size.x++;
			if (key == keyDownArrow ) SelectedObject->Size.y--;
			if (key == keyUpArrow   ) SelectedObject->Size.y++;

			break;
	}

	SelectedObject->NeedUpdate = true;
	SelectedObject->UpdatePosVertices();
}
//

#endif
