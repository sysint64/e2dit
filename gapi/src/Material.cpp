//
#ifndef CPP_MATERIAL
#define CPP_MATERIAL
//
void BaseUniforms::Apply() {
	int TexturesN = 1;

	for (UniformsMap::iterator it = Uniforms.begin(); it != Uniforms.end(); it++) {
		Uniform *U = &it->second;

		switch (U->Type) {
			case uFloat	: glUniform1f (U->Location, *(float*) U->Data); break;
			case uInt	: glUniform1i (U->Location,   *(int*) U->Data); break;
			case uBool	: glUniform1i (U->Location,  *(bool*) U->Data); break;
			case uVec2f : glUniform2fv (U->Location, 1, &(*(glm::vec2*) U->Data)[0]); break;

			case uVec2i :
				glUniform2i (U->Location, ((glm::vec2*) U->Data)->x, ((glm::vec2*) U->Data)->y);
				break;

			case uVec3f :
				glUniform3f (U->Location, ((glm::vec3*) U->Data)->x, ((glm::vec3*) U->Data)->y, ((glm::vec3*) U->Data)->z);
				break;

			case uVec3i :
				glUniform3i (U->Location, ((glm::vec3*) U->Data)->x, ((glm::vec3*) U->Data)->y, ((glm::vec3*) U->Data)->z);
				break;

			//case uMat2f : break;
			//case uMat3f : break;
			case uMat4f :
				glUniformMatrix4fv (U->Location, 1, GL_FALSE, &(*(glm::mat4*) U->Data)[0][0]);
				break;

			case uTex1D :
				if (LastTexN == TexturesN && LastTexID != *(int*) U->Data) {
					
				} else {
					glActiveTexture (GL_TEXTURE0+TexturesN);
					glBindTexture   (GL_TEXTURE_1D, *(int*) U->Data);
				}

				LastTexID = *(int*) U->Data;
				LastTexN  = TexturesN;

				glUniform1i (U->Location, TexturesN);
				TexturesN++;

				break;

			case uTex2D :
				//if (LastTexN == TexturesN && LastTexID != *(int*) U->Data) {
					
				//} else {
					glActiveTexture (GL_TEXTURE0+TexturesN);
					glBindTexture   (GL_TEXTURE_2D, ((Texture2D*) U->Data)->Handle);
				//}

				LastTexID = *(int*) U->Data;
				LastTexN  = TexturesN;

				glUniform1i (U->Location, TexturesN);
				TexturesN++;

				break;
		} // switch
		
	} // for
}
//
void BaseMaterial::Apply() {
	Shader->ApplyShader();
	Uniforms->Apply();
}
//
void BaseMaterial::UpdateData() {

}

void BaseMaterial::UnApply() {
	Shader->UnApplyShader();
}

#endif
