//* Author : Kabilin Andrey, 2013 *//

#ifndef H_SHADERS
#define H_SHADERS

class BaseShader {
public:
	bool Enable;
	string FileName;
	GLuint ProgramObject;
	HashSet <GLuint> *Locations;
	//
	inline void EnableShader() { Enable = true; glLinkProgram(ProgramObject); }
	inline void ApplyShader();
	inline void UnApplyShader();
	
	BaseShader (const int locations = 4) {
		Enable = false;
		ProgramObject = glCreateProgram();
		Locations = new HashSet<GLuint>(locations);
	}

	~BaseShader() {
		//Locations.Free();
		delete Locations;
	}
	
	void AddVertexShader   (const char *fn);
	void AddFragmentShader (const char *fn);
	bool AddGLSLShader     (const char *fn);
};

BaseShader *LastShader = 0;

#endif
