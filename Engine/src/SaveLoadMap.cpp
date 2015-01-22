
// Save Level -------------------------------------------------------------------------------->

static int objectId = 0;

void MapEditor2D::SaveToTextFile (const char *fn) {
	objectId = 0;
	FILE *F = fopen (fn, "w+");
	string ws = "                   ";

	fputs ("Room\n", F);
	fputs (string("    Size : "+IntToStr (RoomSize.x)+", "+IntToStr (RoomSize.y)+";\n").c_str(), F);
	fputs ("end;\n\n", F);

	// Cameras
	for (int i = 0; i < Cameras.size(); i++) {
		fputs ("Camera\n", F);
		fputs (string("    Id : "+IntToStr(i)+";\n").c_str(), F);
		fputs (string("    Name : \""+wstring2string(Cameras[i]->Name)+"\";\n").c_str(), F);
		fputs (string("    Position : "+IntToStr (Cameras[i]->Position.x)+", "+IntToStr (Cameras[i]->Position.y)+";\n").c_str(), F);
		fputs (string("    Size : "+IntToStr (Cameras[i]->Size.x)+", "+IntToStr (Cameras[i]->Size.y)+";\n").c_str(), F);
		fputs ("end;\n\n", F);
	}

	// Layers
	for (int i = 0; i < Layers.size(); i++) {
		string bVisible = Layers[i]->Visible ? "True" : "False";
		fputs ("Layer\n", F);
		fputs (string("    Id : "+IntToStr(i)+";\n").c_str(), F);
		fputs (string("    Name : \""+Layers[i]->Name+"\";\n").c_str(), F);
		fputs (string("    Visible : "+bVisible+";\n").c_str(), F);
		fputs ("end;\n\n", F);

		for (int j = 0; j < Layers[i]->Objects.size(); j++) {
			Layers[i]->Objects[j]->Id = j;
			Layers[i]->Objects[j]->LayerId = i;
		}
	}

	// Objects
	for (int i = 0; i < Layers.size(); i++) {
		for (int j = 0; j < Layers[i]->Objects.size(); j++) {
			BaseGameObject *Obj = Layers[i]->Objects[j];
			string Type = "";

			if (dynamic_cast<PolygonObject*>(Obj)) Type = "Polygon";
			else if (dynamic_cast<PointObject*>(Obj)) Type = "Point";
			else if (dynamic_cast<LightObject*>(Obj)) Type = "Light";
			else Type = "Sprite";

			fputs (string("Object ("+Type+")\n").c_str(), F);
			fputs (string("    Id : "      +IntToStr(j)+";\n").c_str(), F);
			fputs (string("    Layer : "   +IntToStr(i)+";\n").c_str(), F);
			fputs (string("    Class : \"" +Obj->GameClass+";\"\n").c_str(), F);
			fputs (string("    Position : "+FloatToStr(Obj->Position.x)+", "+FloatToStr(Obj->Position.y)+";\n").c_str(), F);
			fputs (string("    Size : "    +FloatToStr(Obj->Size.x)+", "+FloatToStr(Obj->Size.y)+";\n").c_str(), F);
			fputs (string("    Rotation : "+FloatToStr(Obj->Rotation)+";\n").c_str(), F);

			if (dynamic_cast<PolygonObject*>(Obj))    WritePolygonObjectData ((PolygonObject*) Obj, F);
			else if (dynamic_cast<PointObject*>(Obj)) WritePointObjectData   (  (PointObject*) Obj, F);
			else if (dynamic_cast<LightObject*>(Obj)) WriteLightObjectData   (  (LightObject*) Obj, F);
			else WriteSpriteObjectData (Obj, F);
			//
			WriteMaterialData (Obj, F);
			WritePolygonsData (Obj, F);
			WriteLabelsData   (Obj, F);

			fputs ("end;\n\n", F);
			/*if (dynamic_cast<PolygonObject*>(Obj)) {

			} else {
				
				
				fputs ("end;\n\n", F);
			}*/
		}
	}

	fclose (F);
}

// Write Data to File
void MapEditor2D::WritePolygonObjectData (PolygonObject *Obj, FILE *F) {
	WriteJoinedPoints (Obj, F);
}

void MapEditor2D::WriteLightObjectData (LightObject *Obj, FILE *F) {
	fputs (string("    Radius : "+FloatToStr(Obj->Radius)+";\n").c_str(), F);

	if (Obj->Attenuation > 0.f)
		fputs (string("    Attenuation : "+FloatToStr(Obj->Attenuation)+";\n").c_str(), F);

	fputs (string("    Color : "+FloatToStr(Obj->Color.r)+","
								+FloatToStr(Obj->Color.g)+","
								+FloatToStr(Obj->Color.b)+","
								+FloatToStr(Obj->Color.a)+";\n").c_str(), F);

	fputs (string("    LightType : " +IntToStr(Obj->Type)+";\n").c_str(), F);
	fputs (string("    CastShadow : "+IntToStr(Obj->CastShadow)+";\n").c_str(), F);

}

void MapEditor2D::WritePointObjectData (PointObject *Obj, FILE *F) {
	
	if (Obj->JoinObj == 0)
		return;

	fputs (string("    VertexJoinObjId : "+IntToStr(Obj->JoinObj->Id)+";\n").c_str(), F);
	fputs (string("    VertexJoinLayerId : "+IntToStr(Obj->JoinObj->LayerId)+";\n").c_str(), F);
	fputs (string("    VertexJoinId : "+IntToStr(Obj->VertexJoinId)+";\n").c_str(), F);
}

void MapEditor2D::WriteSpriteObjectData (BaseGameObject *Obj, FILE *F) {
	string Data = "Default";
	if (Obj->Data != SpriteDat) Data = "Custom";

	// Write Data Render
	if (Data == "Default") fputs (string("    Data : "+Data+";\n").c_str(), F);
	else {
		string vb, tb, ib, ws;
		ws = "                 ";

		for (int k = 0; k < Obj->Data->VertexBuffer.size(); k++) {
			glm::vec2 val = Obj->Data->VertexBuffer[k];
			string comma = k == 0 ? "" : ", ";
			vb += comma+FloatToStr(val.x)+", "+FloatToStr(val.y);
			if (k != Obj->Data->VertexBuffer.size()-1) vb += "\n"+ws;
		}

		ws = "                   ";
		for (int k = 0; k < Obj->Data->TexCoordsBuffer.size(); k++) {
			glm::vec2 val = Obj->Data->TexCoordsBuffer[k];
			string comma = k == 0 ? "" : ", ";
			tb += comma+FloatToStr(val.x)+", "+FloatToStr(val.y);
			if (k != Obj->Data->TexCoordsBuffer.size()-1) tb += "\n"+ws;
		}

		ws = "            ";
		for (int k = 0; k < Obj->Data->Indices.size(); k++) {
			int val = Obj->Data->Indices[k];
			string comma = k == 0 ? "" : ", ";
			ib += comma+IntToStr(val);

			if ((k+1) % 6 == 0 && k != Obj->Data->Indices.size()-1)
				ib += "\n"+ws;
		}

		fputs (string("    Data : "+Data+";\n").c_str(), F);
		fputs (string("    VertexBuffer : "+vb+";\n\n").c_str(), F);
		fputs (string("    TexCoordBuffer : "+tb+";\n\n").c_str(), F);
		fputs (string("    Indices : "+ib+";\n\n").c_str(), F);
	}

	WriteJoinedPoints (Obj, F);
}

//
void MapEditor2D::WriteJoinedPoints (BaseGameObject *Obj, FILE *F) {
	string Data;
	string ws = "                 ";

	for (int i = 0; i < Obj->JoinedPoints.size(); i++) {
		string VertexId = IntToStr (Obj->JoinedPoints[i]->VertexJoinId);

		string comma = i == 0 ? "" : ", ";
		Data += comma+VertexId;

		if ((i+1) % 6 == 0 && i != Obj->Data->Indices.size()-1)
			Data += "\n"+ws;
	}

	if (Obj->JoinedPoints.size() > 0)
		fputs (string("    JoinedPoints : "+Data+";\n").c_str(), F);
}

void MapEditor2D::WriteMaterialData (BaseGameObject *Obj, FILE *F) {
	fputs ("\n", F);
	fputs (string("    Material: "+IntToStr(Obj->Material != 0)+";\n").c_str(), F);

	if (Obj->Material == 0)
		return;

	// Shader
	BaseShader *Shader = Obj->Material->Shader;
	string locs;
	string ws = "              ";

	fputs (string("    Shader : \""+fs::extractFileName(Shader->FileName)+"\";\n").c_str(), F);

	for (int k = 0; k < Shader->Locations->Hashes.size(); k++) {
		string comma = k == 0 ? "" : ", ";
		locs += comma+"\""+Shader->Locations->Hashes[k]+"\"";

		if ((k+1) % 2 == 0 && k != Shader->Locations->Hashes.size()-1)
			locs += "\n"+ws;
	}

	fputs (string("    Locations : "+locs+";\n").c_str(), F);

	// Material Parameters
	BaseUniforms::UniformsMap *Uniforms = &(Obj->Material->Uniforms->Uniforms);
	
	string uniforms = "";
	ws = "             ";
	BaseUniforms::UniformsMap::iterator ite = Uniforms->end();
	ite--;
	
	for (BaseUniforms::UniformsMap::iterator it = Uniforms->begin(); it != Uniforms->end(); it++) {
		Uniform *U = &it->second;
		string loc = it->first;
		
		string comma = it == Uniforms->begin() ? "" : ", ";

		switch (U->Type) {
			case uMat4f :
				if (loc == "MVP")
					uniforms += comma+"\"MVP\", Matrix, MVP_Matrix";

				break;

			case uFloat :
				uniforms += comma+"\""+loc+"\", "+"Float, "+FloatToStr(*(float*) U->Data);
				break;

			case uInt :
				uniforms += comma+"\""+loc+"\", "+"Int, "+IntToStr(*(int*) U->Data);
				break;

			case uBool :
				uniforms += comma+"\""+loc+"\", "+"Bool, "+IntToStr(*(bool*) U->Data);
				break;

			case uVec2f :
				uniforms += comma+"\""+loc+"\", "+"Vec2f, "+FloatToStr((*(glm::vec2*) U->Data).x)+", "
													+FloatToStr((*(glm::vec2*) U->Data).y);
				break;

			case uVec2i :
				uniforms += comma+"\""+loc+"\", "+"Vec2i, "+IntToStr((*(glm::vec2*) U->Data).x)+", "
													+IntToStr((*(glm::vec2*) U->Data).y);
				break;

			case uVec3f :
				uniforms += comma+"\""+loc+"\", "+"Vec3f, "+FloatToStr((*(glm::vec3*) U->Data).x)+", "
													+FloatToStr((*(glm::vec3*) U->Data).y)+", "
													+FloatToStr((*(glm::vec3*) U->Data).z);
				break;

			case uVec3i :
				uniforms += comma+"\""+loc+"\", "+"Vec3i, "+IntToStr((*(glm::vec3*) U->Data).x)+", "
													+IntToStr((*(glm::vec3*) U->Data).y)+", "
													+IntToStr((*(glm::vec3*) U->Data).z);
				break;

			case uVec4f :
				uniforms += comma+"\""+loc+"\", "+"Vec4f, "+FloatToStr((*(glm::vec4*) U->Data).x)+", "
													+FloatToStr((*(glm::vec4*) U->Data).y)+", "
													+FloatToStr((*(glm::vec4*) U->Data).z)+", "
													+FloatToStr((*(glm::vec4*) U->Data).w);
				break;

			case uVec4i :
				uniforms += comma+"\""+loc+"\", "+"Vec4i, "+IntToStr((*(glm::vec4*) U->Data).x)+", "
													+IntToStr((*(glm::vec4*) U->Data).y)+", "
													+IntToStr((*(glm::vec4*) U->Data).z)+", "
													+IntToStr((*(glm::vec4*) U->Data).w);
				break;

			case uTex2D :
				Texture2D *Tex = (Texture2D*) U->Data;

				string WrapS  = Tex->WrapS == GL_CLAMP_TO_EDGE ? "0" : "1";
				string WrapT  = Tex->WrapT == GL_CLAMP_TO_EDGE ? "0" : "1";
				string Filter = Tex->WrapT == GL_NEAREST       ? "0" : "1";

				uniforms += comma+"\""+loc+"\""+", "+"Tex2D, \""+Tex->FileName+"\", "+WrapS+", "+WrapT+", "+Filter;
				break;
		}

		if (it != ite)
			uniforms += "\n"+ws;
	}

	fputs (string("    Uniforms : "+uniforms+";\n").c_str(), F);
}

void MapEditor2D::WritePolygonsData (BaseGameObject *Obj, FILE *F) {

	string polyData = "";

	for (int i = 0; i < Obj->PolysList.size(); i++) {
		
		PolyData *Poly = Obj->PolysList[i];
		Vertex *s = Poly->Vertices[0];

		if (i == 0) polyData += IntToStr (Poly->Vertices.size());
		else polyData += ", "+IntToStr (Poly->Vertices.size());

		for (int j = 0; j < Poly->Vertices.size(); j++) {

			//Vertex *v = Poly->Vertices[j];

			polyData += ", "+FloatToStr(s->Pos.x)+", "+FloatToStr(s->Pos.y);

			s = s->cw;

		}

	}

	if (Obj->PolysList.size() > 0)
		fputs (string("    BoundingPoly : "+polyData+";\n").c_str(), F);

}

void MapEditor2D::WriteLabelsData (BaseGameObject *Obj, FILE *F) {

}

void MapEditor2D::SaveToBinFile (const char *fn) {
	SaveToTextFile((string(fn)+".tmp").c_str());
	DataMap *Tmp = new DataMap();
	Tmp->LoadFromFile((string(fn)+".tmp").c_str());
	Tmp->SaveToBytecode(fn);
	delete Tmp;
	remove((string(fn)+".tmp").c_str());
}

// <-------------------------------------------------------------------------------- Save Level
