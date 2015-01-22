//
#ifndef H_MATERIAL
#define H_MATERIAL
//
enum {uFloat, uInt, uBool, uTex1D, uTex2D, uVec2f, uVec2i, uVec3f, uVec3i, uVec4f, uVec4i, uMat2f, uColor3f, uColor4f, uMat3f, uMat4f} typedef UType;
struct Uniform {
	wstring  Name;
	UType	 Type;
	GLuint	 Location;
	void	*Data;
};

template <class T, UType UT>
inline Uniform AddUniform (BaseShader *Shader, const wchar_t *name, const char *loc, const T &val) {
	Uniform newU;
	//
	newU.Name	  = name;
	newU.Type	  = UT;
	newU.Data	  = new T();

	if (Shader != 0)
		newU.Location = glGetUniformLocation (Shader->ProgramObject, loc);

	*((T*) newU.Data) = val;

	return newU;
}

class BaseUniforms {
public:
	BaseShader	*Shader;
	typedef map<string, Uniform> UniformsMap;
	UniformsMap	Uniforms;

	BaseUniforms() { Shader = 0; }
	BaseUniforms(BaseShader *sh) : Shader (sh) {}
	//
	inline void Apply ();
	inline bool Exist (const string loc) {
		return Uniforms.count(loc) >= 1;
	}

	inline void Copy (BaseUniforms *src) {
		*this = *src;
		UniformsMap::iterator mit = Uniforms.begin();

		for (UniformsMap::iterator sit = src->Uniforms.begin(); sit != src->Uniforms.end(); sit++) {
			Uniform *sU = &sit->second;
			Uniform *mU = &mit->second;
			
			switch (sU->Type) {
				case uFloat   : mU->Data = new float(); *((float*) mU->Data) = *((float*) sU->Data); break;
				case uBool    : mU->Data = new bool();  *( (bool*) mU->Data) = *( (bool*) sU->Data); break;
				case uInt     : mU->Data = new int();   *(  (int*) mU->Data) = *(  (int*) sU->Data); break;
				//
				case uVec2f   : mU->Data = new glm::vec2(); *((glm::vec2*) mU->Data) = *((glm::vec2*) sU->Data); break;
				case uVec2i   : mU->Data = new glm::vec2(); *((glm::vec2*) mU->Data) = *((glm::vec2*) sU->Data); break;
				case uVec3f   : mU->Data = new glm::vec3(); *((glm::vec3*) mU->Data) = *((glm::vec3*) sU->Data); break;
				case uVec3i   : mU->Data = new glm::vec3(); *((glm::vec3*) mU->Data) = *((glm::vec3*) sU->Data); break;
				case uVec4f   : mU->Data = new glm::vec4(); *((glm::vec4*) mU->Data) = *((glm::vec4*) sU->Data); break;
				case uVec4i   : mU->Data = new glm::vec4(); *((glm::vec4*) mU->Data) = *((glm::vec4*) sU->Data); break;

				case uColor3f : mU->Data = new glm::vec3(); *((glm::vec3*) mU->Data) = *((glm::vec3*) sU->Data); break;
				case uColor4f : mU->Data = new glm::vec4(); *((glm::vec4*) mU->Data) = *((glm::vec4*) sU->Data); break;

				case uMat4f   : mU->Data = new glm::mat4(); *((glm::mat4*) mU->Data) = *((glm::mat4*) sU->Data); break;
				case uTex1D   : mU->Data = new int(); *((int*) mU->Data) = *((int*) sU->Data); break;
				case uTex2D   : mU->Data = new int(); *((int*) mU->Data) = *((int*) sU->Data); break;
			}

			mit++;
		}
	}

	// Add Uniforms
	inline void AddFloat   (const wchar_t* name, const char* loc, float val = 0.f) { Uniforms[loc] = AddUniform<float, uFloat>(Shader, name, loc, val); }
	inline void AddBool    (const wchar_t* name, const char* loc, bool  val = 0  ) { Uniforms[loc] = AddUniform<bool , uBool >(Shader, name, loc, val); }
	inline void AddInt     (const wchar_t* name, const char* loc, int   val = 0  ) { Uniforms[loc] = AddUniform<int  , uInt  >(Shader, name, loc, val); }
	inline void AddTex1D   (const wchar_t* name, const char* loc, int   val = 0  ) { Uniforms[loc] = AddUniform<int  , uTex1D>(Shader, name, loc, val); }
	inline void AddTex2D   (const wchar_t* name, const char* loc, int   val = 0  ) { Uniforms[loc] = AddUniform<int  , uTex2D>(Shader, name, loc, val); }
	//
	inline void AddVec2f   (const wchar_t* name, const char* loc, glm::vec2 val = glm::vec2(0)) { Uniforms[loc] = AddUniform<glm::vec2, uVec2f>(Shader, name, loc, val); }
	inline void AddVec2i   (const wchar_t* name, const char* loc, glm::vec2 val = glm::vec2(0)) { Uniforms[loc] = AddUniform<glm::vec2, uVec2i>(Shader, name, loc, val); }
	inline void AddVec3f   (const wchar_t* name, const char* loc, glm::vec3 val = glm::vec3(0)) { Uniforms[loc] = AddUniform<glm::vec3, uVec3f>(Shader, name, loc, val); }
	inline void AddVec3i   (const wchar_t* name, const char* loc, glm::vec3 val = glm::vec3(0)) { Uniforms[loc] = AddUniform<glm::vec3, uVec3i>(Shader, name, loc, val); }
	inline void AddVec4f   (const wchar_t* name, const char* loc, glm::vec4 val = glm::vec4(0)) { Uniforms[loc] = AddUniform<glm::vec4, uVec4f>(Shader, name, loc, val); }
	inline void AddVec4i   (const wchar_t* name, const char* loc, glm::vec4 val = glm::vec4(0)) { Uniforms[loc] = AddUniform<glm::vec4, uVec4i>(Shader, name, loc, val); }

	inline void AddColor3f (const wchar_t* name, const char* loc, glm::vec3 val = glm::vec3(0)) { Uniforms[loc] = AddUniform<glm::vec3, uColor3f>(Shader, name, loc, val); }
	inline void AddColor4f (const wchar_t* name, const char* loc, glm::vec4 val = glm::vec4(0)) { Uniforms[loc] = AddUniform<glm::vec4, uColor4f>(Shader, name, loc, val); }
	//
	inline void AddMat4f   (const wchar_t* name, const char* loc, glm::mat4 val = glm::mat4(0)) { Uniforms[loc] = AddUniform<glm::mat4, uMat4f>(Shader, name, loc, val); }

	// Set Uniforms
	inline void SetFloat   (const char* loc, float val) { *((float*) Uniforms[loc].Data) = val; }
	inline void SetBool    (const char* loc, bool  val) { *( (bool*) Uniforms[loc].Data) = val; }
	inline void SetInt     (const char* loc, int   val) { *(  (int*) Uniforms[loc].Data) = val; }
	inline void SetTex1D   (const char* loc, int   val) { *(  (int*) Uniforms[loc].Data) = val; }
	inline void SetTex2D   (const char* loc, Texture2D *val) { Uniforms[loc].Data = val; }
	//
	inline void SetVec2f   (const char* loc, glm::vec2 &val) { *((glm::vec2*) Uniforms[loc].Data) = val; }
	inline void SetVec2i   (const char* loc, glm::vec2 &val) { *((glm::vec2*) Uniforms[loc].Data) = val; }
	inline void SetVec3f   (const char* loc, glm::vec3 &val) { *((glm::vec3*) Uniforms[loc].Data) = val; }
	inline void SetVec3i   (const char* loc, glm::vec3 &val) { *((glm::vec3*) Uniforms[loc].Data) = val; }
	inline void SetVec4f   (const char* loc, glm::vec4 &val) { *((glm::vec4*) Uniforms[loc].Data) = val; }
	inline void SetVec4i   (const char* loc, glm::vec4 &val) { *((glm::vec4*) Uniforms[loc].Data) = val; }
	inline void SetColor3f (const char* loc, glm::vec3 &val) { *((glm::vec3*) Uniforms[loc].Data) = val; }
	inline void SetColor4f (const char* loc, glm::vec4 &val) { *((glm::vec4*) Uniforms[loc].Data) = val; }
	//
	inline void SetMat4f   (const char* loc, glm::mat4 &val) { *((glm::mat4*) Uniforms[loc].Data) = val; }

	// Get Uniforms
	inline Texture2D *GetTex2D (const char* loc) { return (Texture2D*) Uniforms[loc].Data; }
};

class BaseMaterial {
public:
	BaseShader	 *Shader;
	BaseUniforms *Uniforms;
	//
	BaseMaterial () : Uniforms (new BaseUniforms()) {}
	BaseMaterial (BaseShader *sh) : Shader (sh), Uniforms (new BaseUniforms(sh)) {}
	//
	inline void Copy (BaseMaterial *src) {
		this->Uniforms->Copy (src->Uniforms);
		this->Shader = src->Shader;
	}
	//
	virtual inline void UpdateData();
	virtual inline void Apply();
	virtual inline void UnApply();
};
//
class FBOMaterial : public BaseShader {
public:
	FBOMaterial (const int width, const int height) {}
	//Framebuffer	 *FBO;
	BaseShader	 *ShaderPass2;
	BaseUniforms *UniformsPass2;

	virtual inline void UpdateData (const int width, const int height);
	virtual inline void Apply();
	virtual inline void UnApply();
};

#endif
