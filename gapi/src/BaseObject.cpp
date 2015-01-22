#ifndef CPP_BASEOBJECT
#define CPP_BASEOBJECT
//
// Data Render
//
void DataRender::CreateVBO (const bool Static) {
	//
	// Vertex Buffer
	glGenBuffers (1, &vId);

	glBindBuffer (GL_ARRAY_BUFFER, vId);

	if (Static) {
		glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat)*2*vCount, &VertexBuffer[0], GL_STATIC_DRAW);
	} else {
		int Size = sizeof(GLfloat)*2*vCount;

		glBufferData    (GL_ARRAY_BUFFER, Size, 0, GL_STREAM_DRAW);
		glBufferSubData (GL_ARRAY_BUFFER, 0, Size, &VertexBuffer[0]);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*2*vCount, &VertexBuffer[0], GL_STREAM_DRAW);
	}
	
	// TexCoord Buffer
	glGenBuffers (1, &tId);
	glBindBuffer (GL_ARRAY_BUFFER, tId);
	glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat)*2*vCount, &TexCoordsBuffer[0], GL_STATIC_DRAW);

	// Indices Buffer
	glGenBuffers (1, &iId);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, iId);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*iCount, &Indices[0], GL_STATIC_DRAW);
	//
	if (VAOEXT)
		if (OGLMajor >= 3) CreateVAO_33();
		else CreateVAO_21();
	//
	fInit = true;
}

//
void DataRender::CreateVAO_33() {
	//
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	int BindingIndex = 0;
	int attrLoc = 0;
	//
	attrLoc = 0; // in_Position
	glBindBuffer(GL_ARRAY_BUFFER, vId);
	glEnableVertexAttribArray(attrLoc);
	glVertexAttribPointer(attrLoc, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	
	attrLoc = 2; // in_TexCoord
	glBindBuffer(GL_ARRAY_BUFFER, tId);
	glEnableVertexAttribArray(attrLoc);
	glVertexAttribPointer(attrLoc, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	//
}

//
void DataRender::CreateVAO_21() {
	//
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	//Vertices
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2*vCount, 0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vId);
	glVertexPointer(2, GL_FLOAT, 0, NULL);
	
	////TexCoords
	glEnableClientState( GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, tId);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	
	//Indices
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iId);
}

void DataRender::SetAttrs() {
	if (!fInit) CreateVBO();
	if (RenderMode != GL_NONE) {
		//
		if (!VAOEXT) {
			//
			//TexCoords
			glEnableClientState( GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, tId);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);

			//Vertices
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2*vCount, 0);
			glEnableClientState(GL_VERTEX_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, vId);
			glVertexPointer(2, GL_FLOAT, 0, NULL);
				
			//Indices
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iId);
			//
		} else {
			glBindVertexArray(VAO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iId);
		}
	}
}
//
void DataRender::RenderVBO() {
	//
	if (!fInit) CreateVBO (Static);
	else if (RenderMode != GL_NONE) {
		//
		if (!VAOEXT) {
			//
			//TexCoords
			glEnableClientState( GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, tId);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);

			//Vertices
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2*vCount, 0);
			glEnableClientState(GL_VERTEX_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, vId);
			glVertexPointer(2, GL_FLOAT, 0, NULL);
				
			//Indices
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iId);
			//
		} else {
			glBindVertexArray(VAO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iId);
		}
			
		//Draw
		Render();
	}
	//
}

//
void DataRender::Render() {
	glDrawElements(RenderMode, iCount, GL_UNSIGNED_INT, NULL);
}

//
DataRender::DataRender() {
	//
	RenderMode = GL_TRIANGLES;
	fInit = false; Static = true;
	vCount = 0; iCount = 0;
}

DataRender::~DataRender() {
	//
	glDeleteBuffers(1, &vId);
	glDeleteBuffers(1, &tId);
	glDeleteBuffers(1, &iId);
}
//

// BaseObject
//
void BaseObject::UpdateModelMatrix() {
	//
	glm::mat4 mt = glm::translate (glm::mat4(1.0f), glm::vec3(Position+Offset, 0.0f));
	glm::mat4 mr = glm::rotate    (glm::mat4(1.0f), Rotation, glm::vec3(0,0,1));
	glm::mat4 ms = glm::scale	  (glm::mat4(1.0f), glm::vec3(Size, 1.0f));

	//
	ModelMatrix = mt*mr*ms;
	MVPMatrix = ModelMatrix;
	MVPMatrix = mCamera->MVPMatrix*ModelMatrix;
	NeedUpdate = false;
}

//
void BaseObject::Render() {
	//
	if (!Visible) return;
	if (Shader != 0) {
		//
		Shader->ApplyShader();
		
		if (NeedUpdate)
			UpdateModelMatrix();

		//
		glUniformMatrix4fv(Shader->Locations->GetData("MVP"), 1, GL_FALSE, &MVPMatrix[0][0]);//
		glUniform1i(Shader->Locations->GetData("Texture"), 0);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture->Handle);
		
		Data->RenderVBO();
		//
	} else {
		//
		if (NeedUpdate)
			UpdateModelMatrix();
		//
		Data->RenderVBO();
	}
	//
}

//
void BaseObject::BaseRender() {
	//
	if (NeedUpdate || mCamera->NeedUpdate)
		UpdateModelMatrix();

	glUniformMatrix4fv (Shader->Locations->GetData("MVP"), 1, GL_FALSE, &(MVPMatrix[0][0]));
	//float *ff = glm::value_ptr(ModelMatrix[0]);
	/*glMultiTexCoord4fv (GL_TEXTURE1, glm::value_ptr(ModelMatrix[0]));
	glMultiTexCoord4fv (GL_TEXTURE2, glm::value_ptr(ModelMatrix[1])); 
	glMultiTexCoord4fv (GL_TEXTURE3, glm::value_ptr(ModelMatrix[2]));*/ 

	Data->Render();
}

void BaseObject::EmptyRender() {
	//if (NeedUpdate || mCamera->NeedUpdate)
	//	UpdateModelMatrix();

	Data->SetAttrs();
	Data->Render();	
}
//
void BaseObject::Init() {
	//
	Visible = true;
	NeedUpdate = true;
	Type = 0;
	Size[0] = 8; Size[1] = 8;
	Pivot = glm::vec2(0);
}

void BaseObject::Free() {
	//
}

void BaseObject::Step(float deltaTime) {
//UpdateModelMatrix();
}
//
//
// Animated Object
void AnimatedObject::Render() {

}
//
// Surface
void Surface::Render() {
	//
	//if (!Visible) return;
	//

	/*glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
    glClear(GL_STENCIL_BUFFER_BIT);  // needs mask=0xFF*/
	//
	Shader->ApplyShader();
	
	//if (NeedUpdate)
	//
	//MVPMatrix = mCamera->MVPMatrix;
	//
	glUniformMatrix4fv(Shader->Locations->GetData("MVP"), 1, GL_FALSE, &MVPMatrix[0][0]); //
	glUniform3fv(Shader->Locations->GetData("Color"), 1, &Color[0]);

	/*for (int i = 0; i < Data->vCount; i++) {
		//Position[0] = Data->VertexBuffer[i][0];
		//Position[1] = Data->VertexBuffer[i][1];
		//
		//UpdateModelMatrix();
		Quad->RenderVBO();
	}*/

	/*if (!VAOEXT) {
		//
		//TexCoords
		glEnableClientState( GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, tId);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		//Vertices
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2*vCount, 0);
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, vId);
		glVertexPointer(2, GL_FLOAT, 0, NULL);
			
		//Indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iId);
		//
	} else {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iId);
	}

	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);*/
	//
	//glDisable(GL_STENCIL_TEST);
	//
}
//
#endif
