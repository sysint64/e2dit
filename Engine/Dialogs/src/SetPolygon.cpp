//* Author : Kabilin Andrey, 2013-2014 *//

#ifndef CPP_SET_POLYGON
#define CPP_SET_POLYGON

//
void SetPolygonForm::Init() {
	Dialog = (UIDialog*) Data->Elements["setpolygondialog"];
	BrowsePanel =      (UIPanel*) Data->Elements["pbrowse"];
	GridImage   = (UIColorPanel*) Data->Elements["transparent"];

	Data->Elements["bcancel"]  ->OnClick = OnCancelClick;
	Data->Elements["bok"]      ->OnClick = OnOKClick;
	//
	Data->Elements["libox"]    ->OnClick = OnBoxTypeClick;
	Data->Elements["licircle"] ->OnClick = OnCircleTypeClick;
	Data->Elements["lipolygon"]->OnClick = OnPolygonTypeClick;
	Data->Elements["bzoom"]    ->OnClick = OnSetDefaulrZoom;
	//
	((UIEdit*) Data->Elements["ezoom"])->OnChange = OnChangeZoom;
}

void SetPolygonForm::Open (BaseGameObject *Obj, int aPolyId) {
	//
	SrcObj   = Obj;
	Object   = Obj->Template;
	TmpPos0  = Object->Position;
	TmpSize0 = Object->Size;
	
	Poly = new PolyData();
	Poly->Copy(Obj->PolysList[PolyId]);

	PolyId = aPolyId;

	Dialog->Width  = clamp (int(Object->Size.x+275), 520, WindowWidth -55)+5;
	Dialog->Height = clamp (int(Object->Size.y+60 ), 300, WindowHeight-55)+5;

	Dialog->Left = (WindowWidth  >> 1) - (Dialog->Width  >> 1);
	Dialog->Top  = (WindowHeight >> 1) - (Dialog->Height >> 1);

	Dialog->Open();
	cClick = false;
	//
	Data->Elements["libox"]    ->Checked = false;
	Data->Elements["licircle"] ->Checked = false;
	Data->Elements["lipolygon"]->Checked = false;
	//
	Zoom  = 100;
	ZoomK = 1.f;
	///
	switch (Poly->Type) {
		case pdtBox     : Data->Elements["libox"]    ->Checked = true; break;
		case pdtCircle  : Data->Elements["licircle"] ->Checked = true; break;
		case pdtPolygon : Data->Elements["lipolygon"]->Checked = true; break;
	}
}

void SetPolygonForm::Render() {
	if (!Dialog->isOpened) return;

	BrowsePanel->Manager->PushScissor (BrowsePanel->AbsoluteLeft, WindowHeight-BrowsePanel->Height-
									   BrowsePanel->AbsoluteTop+BrowsePanel->ScrollHeight,
									   BrowsePanel->Width -BrowsePanel->ScrollWidth,
									   BrowsePanel->Height-BrowsePanel->ScrollHeight);

	GridImage->Width  = max (BrowsePanel->Width -20, Object->Size.x)+AddedXScroll;
	GridImage->Height = max (BrowsePanel->Height-20, Object->Size.y)+AddedYScroll;
	GridImage->Update();

	int hs = 1; int vs = 1;

	//if (BrowsePanel->ScrollWidth  != 0) vs = 0;
	//if (BrowsePanel->ScrollHeight != 0) hs = 0;
	if (Object->Size.x > BrowsePanel->Width)  hs = 0;
	if (Object->Size.y > BrowsePanel->Height) vs = 0;

	float coffsetx = ((Object->Size.x-BrowsePanel->Width )/2+10)*hs;
	float coffsety = ((Object->Size.y-BrowsePanel->Height)/2+10)*vs;

	Object->Position.x = GridImage->AbsoluteLeft-MapEditor->Left-coffsetx+AddedXScroll+Object->Size.x/2;
	Object->Position.y = WindowHeight-GridImage->AbsoluteTop-Object->Size.y/2+coffsety-AddedYScroll;
	Poly->Pos.x = AddedXScroll;
	Poly->Pos.y = AddedYScroll;
	//
	Object->Size.x = TmpSize0.x*(float)Zoom/100.f;
	Object->Size.y = TmpSize0.y*(float)Zoom/100.f;

	Object->UpdateModelMatrix();

	if (Object->Material->Uniforms->Exist ("MVP"))
		Object->Material->Uniforms->SetMat4f ("MVP", Object->MVPMatrix);

	Object->Material->Apply();
	Object->mCamera = ScreenCamera;
	Object->EmptyRender();
	Object->Material->UnApply();

	// Draw Polygon
	glBegin2D();
	glDisable (GL_LINE_SMOOTH);

	//
	x0 = GridImage->AbsoluteLeft+Poly->Pos.x-coffsetx+Object->Size.x/2;
	y0 = GridImage->AbsoluteTop +Poly->Pos.y-coffsety+Object->Size.y/2;
	//
	y0 = WindowHeight-y0;
	//
	// Vertex Pos 0
	float vx0 = Object->Position.x+MapEditor->Left;//+MapEditor->VertexObject->Size.x/2;
	float vy0 = Object->Position.y;//-MapEditor->VertexObject->Size.y/2;
	MouseDist = glm::distance(glm::vec2(Object->WorldToScreen().x, WindowHeight-Object->WorldToScreen().y), glm::vec2(MouseX, MouseY));
	//
	float ZoomedCircle = Poly->CircleRadius*ZoomK;

	glColor3fv (&MapEditor->SelectColor[0]);
	glLineWidth(2);

	switch (Poly->Type) {
		case pdtBox :
			glBegin (GL_LINE_LOOP);
				glVertex2f (x0-Poly->BoxSize.x/2, y0-Poly->BoxSize.y/2);
				glVertex2f (x0-Poly->BoxSize.x/2, y0+Poly->BoxSize.y/2);
				glVertex2f (x0+Poly->BoxSize.x/2, y0+Poly->BoxSize.y/2);
				glVertex2f (x0+Poly->BoxSize.x/2, y0-Poly->BoxSize.y/2);
			glEnd ();

			glEnd2D();

			// Draw Vertices
			TexAtlasShader->ApplyShader();

			glUniform1i (TexAtlasShader->Locations->GetData("Texture"), 0);
			glUniform2f (TexAtlasShader->Locations->GetData("Size")  , Core->fVertexSize [0], Core->fVertexSize[1]);
			glUniform2f (TexAtlasShader->Locations->GetData("Offset"), Core->VertexOffset[0], Core->VertexOffset[1]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Core->Theme->Skin->Handle);

			MapEditor->VertexObject->SetPosition(glm::vec2(vx0-Poly->BoxSize.x/2, vy0-Poly->BoxSize.y/2));
			MapEditor->VertexObject->BaseRender();

			MapEditor->VertexObject->SetPosition(glm::vec2(vx0-Poly->BoxSize.x/2, vy0+Poly->BoxSize.y/2));
			MapEditor->VertexObject->BaseRender();

			MapEditor->VertexObject->SetPosition(glm::vec2(vx0+Poly->BoxSize.x/2, vy0+Poly->BoxSize.y/2));
			MapEditor->VertexObject->BaseRender();

			MapEditor->VertexObject->SetPosition(glm::vec2(vx0+Poly->BoxSize.x/2, vy0-Poly->BoxSize.y/2));
			MapEditor->VertexObject->BaseRender();

			TexAtlasShader->UnApplyShader();

			break;

		case pdtCircle :
			glBegin (GL_LINE_LOOP);

			for (int i = 0; i < 100; i++) {
				float ang = (float)i/100.0f*pi*2.0f;

				float x = x0+cos(ang)*ZoomedCircle;
				float y = y0+sin(ang)*ZoomedCircle;

				glVertex2f (x, y);
			}

			glEnd();
			glEnd2D();

			// Draw Cursor Vertex
			if ((MouseDist <= ZoomedCircle+10 &&
				 MouseDist >= ZoomedCircle-10) || cClick) {

				TexAtlasShader->ApplyShader();

				glUniform1i (TexAtlasShader->Locations->GetData("Texture"), 0);
				glUniform2f (TexAtlasShader->Locations->GetData("Size")  , Core->fVertexSize [0], Core->fVertexSize [1]);
				glUniform2f (TexAtlasShader->Locations->GetData("Offset"), Core->VertexOffset[2], Core->VertexOffset[3]);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, Core->Theme->Skin->Handle);

				//MapEditor->VertexObject->SetPosition(glm::vec2(Object->+Poly->CircleRadius, vy0+Poly->CircleRadius));
				float ang = atan2(Object->WorldToScreen().x-MouseX, WindowHeight-Object->WorldToScreen().y-MouseY);
				FinalAng += (ang-LastAng);
				LastAng   = ang;

				MapEditor->VertexObject->Position.x = vx0-ZoomedCircle*sin(FinalAng)+MapEditor->Camera->Position.x;
				MapEditor->VertexObject->Position.y = vy0+ZoomedCircle*cos(FinalAng)+MapEditor->Camera->Position.x;

				MapEditor->VertexObject->BaseRender();

				TexAtlasShader->UnApplyShader();
				MayClick = true;

				if (cClick) {
					Poly->CircleRadius = (MouseDist-DeltaRadius)/ZoomK;
					Poly->CircleRadius = min (Poly->CircleRadius, min(GridImage->Width, GridImage->Height)>>1);
					//Poly->CircleRadius = Poly->CircleRadius;

					//ZoomedCircle = MouseDist-DeltaRadius;
					//ZoomedCircle = min (Poly->CircleRadius, min(GridImage->Width, GridImage->Height)>>1);
					//Poly->CircleRadius = ZoomedCircle/((float)Zoom/100.f);
				}
			} else MayClick = false;

			break;

		case pdtPolygon :
			MapEditor->VertexObject->mCamera = ScreenCamera;

			glBegin (GL_LINE_LOOP);

			Vertex *s = Poly->Vertices[0];
			for (int i = 0; i < Poly->Vertices.size(); i++) {
				glVertex2f (x0+s->Pos.x*ZoomK, y0+s->Pos.y*ZoomK);
				s = s->cw;
			}

			glEnd();
			glEnd2D();

			// Draw Vertices
			TexAtlasShader->ApplyShader();

			glUniform1i (TexAtlasShader->Locations->GetData("Texture"), 0);
			glUniform2f (TexAtlasShader->Locations->GetData("Size")  , Core->fVertexSize [0], Core->fVertexSize [1]);
			glUniform2f (TexAtlasShader->Locations->GetData("Offset"), Core->VertexOffset[0], Core->VertexOffset[1]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Core->Theme->Skin->Handle);

			//glm::vec2 cptf;
			MayMoveVertex = false;
			bool  Finded = false;
			float dmin = 10.f;
			float vdradius = MapEditor->VertexObject->Size.x*2;
			float dsvmin   = vdradius;

			if (!MoveVertex)
				SelVertexId = -1;

			// Draw Cursor Vertex
			if (!cClick && !MoveVertex)
			for (int i = 0; i < Poly->Vertices.size(); i++) {
				Vertex *v = Poly->Vertices[i];
				float d = glm::distance(glm::vec2(MouseX, WindowHeight-MouseY), glm::vec2(x0+v->Pos.x*ZoomK, y0+v->Pos.y*ZoomK));
				
				if (dsvmin > d) {
					SelVertexId = i;
					dsvmin = d;
					Finded = false;
					MayMoveVertex = true;
				}

				if (SelVertexId >= 0)
					continue;

				Edge E = Edge (Poly->Vertices[i]->Pos*ZoomK, Poly->Vertices[i]->cw->Pos*ZoomK);

				glm::vec2 rv  = E.v2->Pos-E.v1->Pos;
				glm::vec3 PZ3 = glm::normalize(glm::cross(glm::vec3(rv, 0), glm::vec3(0, 0, 1)));
				glm::vec2 n   = glm::vec2(PZ3.x, PZ3.y);
				
				Edge P  = Edge (glm::vec2(MouseX, WindowHeight-MouseY), glm::vec2(MouseX+n.x, WindowHeight-MouseY+n.y));				
				Edge EE = Edge (glm::vec2(x0+E.v1->Pos.x, y0+E.v1->Pos.y),
								glm::vec2(x0+E.v2->Pos.x, y0+E.v2->Pos.y));

				float dt = 0;
				EE.Intersect(&P, dt);
				glm::vec2 cpt = EE.Point(dt);

				float l1 = glm::length(EE.v2->Pos-EE.v1->Pos);
				float l2 = glm::length(cpt-EE.v1->Pos);
				float l3 = glm::length(cpt-EE.v2->Pos);
				float radius = glm::length(glm::vec2(MouseX, WindowHeight-MouseY)-cpt);
				//
				if (l1 > max(l2, l3)) {
					if (dmin > radius && radius < 10.f) {
						dmin = radius;
						cptf = EE.Point(dt);
						InsVertex = Poly->Vertices[i];
						Finded = true;
					}
				}
			}

			MayClick = false;

			for (int i = 0; i < Poly->Vertices.size(); i++) {
				if (i == SelVertexId)
					continue;

				MapEditor->VertexObject->SetPosition(glm::vec2(vx0+Poly->Vertices[i]->Pos.x*ZoomK,
															   vy0+Poly->Vertices[i]->Pos.y*ZoomK));
				MapEditor->VertexObject->BaseRender();
				//MapEditor->VertexObject->mCamera = MapEditor->Camera;
			}

			if (SelVertexId >= 0) {
				glUniform2f (TexAtlasShader->Locations->GetData("Size")  , Core->fVertexSize [4], Core->fVertexSize [5]);
				glUniform2f (TexAtlasShader->Locations->GetData("Offset"), Core->VertexOffset[4], Core->VertexOffset[5]);

				MapEditor->VertexObject->SetPosition(glm::vec2(vx0+Poly->Vertices[SelVertexId]->Pos.x*ZoomK, vy0+Poly->Vertices[SelVertexId]->Pos.y*ZoomK));
				//MapEditor->VertexObject->SetSize (glm::vec2(Core->VertexSize[4], Core->VertexSize[5]));
				MapEditor->VertexObject->BaseRender();	

				//MapEditor->VertexObject->SetSize (glm::vec2(Core->VertexSize[0], Core->VertexSize[1]));
			}
			MapEditor->VertexObject->mCamera = MapEditor->Camera;
			if (Finded && !cClick && !AddedVertex && !MoveVertex) {
				MayClick = true;
				glUniform2f (TexAtlasShader->Locations->GetData("Offset"), Core->VertexOffset[2], Core->VertexOffset[3]);

				MapEditor->VertexObject->Position.x = cptf.x+MapEditor->Camera->Position.x;
				MapEditor->VertexObject->Position.y = cptf.y+MapEditor->Camera->Position.y;

				MapEditor->VertexObject->BaseRender();
			} else if (AddedVertex) {
				InsVertex->Pos.x = (MouseX-x0)/ZoomK;
				InsVertex->Pos.y = (WindowHeight-MouseY-y0)/ZoomK;
			}

			if (MoveVertex) {
				Poly->Vertices[SelVertexId]->Pos.x = (MouseX-x0)/ZoomK;
				Poly->Vertices[SelVertexId]->Pos.y = (WindowHeight-MouseY-y0)/ZoomK;
			}

			TexAtlasShader->UnApplyShader();
			MapEditor->VertexObject->mCamera = MapEditor->Camera;

			break;
	}

	Vertex *CV = 0;

	if (AddedVertex) CV = InsVertex;
	if (MoveVertex)  CV = Poly->Vertices[SelVertexId];
	if (CV == 0) {
		BrowsePanel->Manager->PopScissor();
		glLineWidth(1);
		return;
	}

	//UpdateScroll(CV);
	//UpdateScroll(CV);

	// Y
	/*float vyi = GridImage->AbsoluteTop+CV->Pos.y*ZoomK-coffsety+Object->Size.y/2;
	vyi = WindowHeight-vyi-35;

	if (vyi <= GridImage->AbsoluteTop-AddedYScroll-1) {
		float dy = (GridImage->AbsoluteTop-vyi-AddedYScroll);
		AddedYScroll = GridImage->AbsoluteTop-vyi;
		LastVY = vyi;
		sf::Mouse::setPosition (sf::Vector2i (MouseX, MouseY+dy), *MainWindow);
	} else

	if (vyi >= LastVY && BrowsePanel->ScrollWidth != 0) {
		float dy = abs(LastVY-vyi);
		AddedYScroll -= dy;
		LastVY = vyi;
		sf::Mouse::setPosition (sf::Vector2i (MouseX, MouseY-dy), *MainWindow);
	}*/
	
	BrowsePanel->Manager->PopScissor();
	glLineWidth(1);

	//Object->mCamera = MapEditor->Camera;
}

/*void SetPolygonForm::UpdateScroll (Vertex* CV) {
	int hs = 1; int vs = 1;
	if (Object->Size.x > BrowsePanel->Width)  hs = 0;
	if (Object->Size.y > BrowsePanel->Height) vs = 0;

	float coffsetx = ((Object->Size.x-BrowsePanel->Width )/2+10)*hs;
	float coffsety = ((Object->Size.y-BrowsePanel->Height)/2+10)*vs;
	float vxi = GridImage->AbsoluteLeft+CV->Pos.x*ZoomK-coffsetx+Object->Size.x/2;

	if (vxi <= GridImage->AbsoluteLeft-AddedXScroll-1) {
		float dx = (GridImage->AbsoluteLeft-vxi-AddedXScroll);
		AddedXScroll = GridImage->AbsoluteLeft-vxi;
		LastVX = vxi;
		sf::Mouse::setPosition (sf::Vector2i (MouseX+dx, MouseY), *MainWindow);
	} else

	if (vxi >= LastVX && BrowsePanel->ScrollHeight != 0) { /* FIXME ScrollHeight bad cmp */
	/*	float dx = abs(LastVX-vxi);
		AddedXScroll -= dx;
		LastVX = vxi;
		sf::Mouse::setPosition (sf::Vector2i (MouseX-dx, MouseY), *MainWindow);
	}

	// Y
	/*float vyi = GridImage->AbsoluteTop+CV->Pos.y*ZoomK-coffsety+Object->Size.y/2;
	vyi = WindowHeight-vyi-35;

	if (vyi <= GridImage->AbsoluteTop-AddedYScroll-1) {
		float dy = (GridImage->AbsoluteTop-vyi-AddedYScroll);
		AddedYScroll = GridImage->AbsoluteTop-vyi;
		LastVY = vyi;
		sf::Mouse::setPosition (sf::Vector2i (MouseX, MouseY+dy), *MainWindow);
	} else

	if (vyi >= LastVY && BrowsePanel->ScrollWidth != 0) {
		float dy = abs(LastVY-vyi);
		AddedYScroll -= dy;
		LastVY = vyi;
		sf::Mouse::setPosition (sf::Vector2i (MouseX, MouseY-dy), *MainWindow);
	}*/
//}

void SetPolygonForm::UpdateScroll (Vertex* CV) {
	int hs = 1; int vs = 1;
	if (Object->Size.x > BrowsePanel->Width)  hs = 0;
	if (Object->Size.y > BrowsePanel->Height) vs = 0;

	float coffsetx = ((Object->Size.x-BrowsePanel->Width )/2+10)*hs;
	float coffsety = ((Object->Size.y-BrowsePanel->Height)/2+10)*vs;

	float vxmax = -99999.f; float vymax = -99999.f;
	float vxmin =  99999.f; float vymin =  99999.f;

	for (int i = 0; i < Poly->Vertices.size(); i++) {
		Vertex *CV = Poly->Vertices[i];
		float vxi = GridImage->AbsoluteLeft+CV->Pos.x*ZoomK-coffsetx+Object->Size.x/2;
		float vyi = GridImage->AbsoluteTop +CV->Pos.y*ZoomK-coffsety+Object->Size.y/2;
		vyi = WindowHeight-vyi-35;
		//
		vxmax = max (vxmax, vxi); vxmin = min (vxmin, vxi);
		vymax = max (vymax, vyi); vymin = min (vymin, vyi);
		//
		Poly->Vertices[i]->AbsPos = glm::vec2(vxi, vyi);
	}

	// X ------------------------------------------------------------------------------------>
	// Min
	if (vxmin <= GridImage->AbsoluteLeft-AddedXScroll) {
		float dx = (GridImage->AbsoluteLeft-vxmin-AddedXScroll);
		AddedXScroll = GridImage->AbsoluteLeft-vxmin;
		LastVX = vxmin;

		if (CV != 0 && CV->AbsPos.x <= vxmin) {
			BrowsePanel->SetScrollX(0);
			sf::Mouse::setPosition (sf::Vector2i (MouseX+dx, MouseY), *MainWindow);
		}
	} else

	if (CV != 0 && vxmin >= LastVX && BrowsePanel->ScrollHeight != 0) {
		if (CV->AbsPos.x <= vxmin) {
			float dx = abs(LastVX-vxmin);
			AddedXScroll -= dx;
			LastVX = vxmin;
			BrowsePanel->SetScrollX(0);
			sf::Mouse::setPosition (sf::Vector2i (MouseX-dx, MouseY), *MainWindow);
		}
	}

	// Max

	// <------------------------------------------------------------------------------------ X

	// Y ------------------------------------------------------------------------------------>
	// Min
	if (vymin <= GridImage->AbsoluteTop-AddedYScroll-1) {
		float dy = (GridImage->AbsoluteTop-vymin-AddedYScroll);
		AddedYScroll = GridImage->AbsoluteTop-vymin;
		LastVY = vymin;

		if (CV != 0 && CV->AbsPos.y <= vymin) {
			BrowsePanel->SetScrollX(0);
			sf::Mouse::setPosition (sf::Vector2i (MouseX, MouseY+dy), *MainWindow);
		}
	} else

	if (CV != 0 && vymin >= LastVY && BrowsePanel->ScrollWidth != 0) {
		if (CV->AbsPos.y <= vymin) {
			BrowsePanel->SetScrollY(0);
			float dy = abs(vymin-LastVY);
			AddedYScroll -= dy;
			LastVY = vymin;
			sf::Mouse::setPosition (sf::Vector2i (MouseX, MouseY-dy), *MainWindow);
		}

		//if (CV->AbsPos.y <= vymin)
		//	sf::Mouse::setPosition (sf::Vector2i (MouseX, MouseY-dy), *MainWindow);
	}
	// <------------------------------------------------------------------------------------ Y	

}

void SetPolygonForm::OnCancelClick (UIElement *Sender) {
	setPolygonForm->Object->Position = setPolygonForm->TmpPos0;
	setPolygonForm->Object->Size     = setPolygonForm->TmpSize0;
	setPolygonForm->Dialog->Close();

	delete setPolygonForm->Poly;
}

void SetPolygonForm::OnOKClick (UIElement *Sender) {
	setPolygonForm->Object->Position = setPolygonForm->TmpPos0;
	setPolygonForm->Object->Size     = setPolygonForm->TmpSize0;
	setPolygonForm->Dialog->Close();

	delete setPolygonForm->SrcObj->PolysList[setPolygonForm->PolyId];
	setPolygonForm->SrcObj->PolysList[setPolygonForm->PolyId] = setPolygonForm->Poly;
}

// Polygon Type

void SetPolygonForm::OnBoxTypeClick (UIElement *Sender) {
	setPolygonForm->Poly->Type = pdtBox;
}

void SetPolygonForm::OnCircleTypeClick (UIElement *Sender) {
	setPolygonForm->Poly->Type = pdtCircle;
}

void SetPolygonForm::OnPolygonTypeClick (UIElement *Sender) {
	setPolygonForm->Poly->Type = pdtPolygon;
}

// Set Zoom

void SetPolygonForm::OnChangeZoom (UIElement *Sender) {
	setPolygonForm->Zoom  = WStrToInt(((UIEdit*)Sender)->wText);
	setPolygonForm->ZoomK = (float)setPolygonForm->Zoom/100.f;
	setPolygonForm->UpdateScroll();
}

void SetPolygonForm::OnSetDefaulrZoom (UIElement *Sender) {
	setPolygonForm->Zoom  = 100;
	setPolygonForm->ZoomK = 1.f;
	setPolygonForm->UpdateScroll();
}

// Events

void SetPolygonForm::KeyPressed (Uint16 key) {

}

void SetPolygonForm::MouseDown (int x, int y, int shift) {
	if (MayClick && !MayMoveVertex) {
		cClick = true;
		AddedVertex = true;
		DeltaRadius = MouseDist-Poly->CircleRadius*ZoomK;

		if (InsVertex != 0) {
			Poly->Iter = InsVertex;
			InsVertex = Poly->Insert (cptf-glm::vec2(x0, y0));
		}
	}

	if (MayMoveVertex)
		MoveVertex = true;
}

void SetPolygonForm::MouseUp (int x, int y, int shift) {
	cClick = false;
	AddedVertex = false;
	InsVertex = 0;
	MoveVertex = false;
	SelVertexId = -1;
}

#endif
