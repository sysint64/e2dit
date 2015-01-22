//* Author : Kabilin Andrey, 2013 *//

#ifndef CPP_SHADERS
#define CPP_SHADERS

// Utility
GLenum g_OpenGLError = GL_NO_ERROR;

// проверка на ошибки OpenGL
#define OPENGL_CHECK_FOR_ERRORS() \
        if ((g_OpenGLError = glGetError()) != GL_NO_ERROR) \
                LOG_ERROR("OpenGL error 0x%X\n", (unsigned)g_OpenGLError);

//
GLint ShaderStatus(GLuint shader, GLenum param) {
	//
	GLint status, length;
	GLchar buffer[1024];
	
	glGetShaderiv(shader, param, &status);
	
	if (status != GL_TRUE) {
		glGetShaderInfoLog(shader, 1024, &length, buffer);
		LOG_ERROR("Shader: %s\n", (const char*)buffer);
	}
	
	OPENGL_CHECK_FOR_ERRORS();
	
	return status;
}

//
GLint ShaderProgramStatus(GLuint program, GLenum param) {
	//
	GLint status, length;
	GLchar buffer[1024];
	
	glGetProgramiv(program, param, &status);
	
	if (status != GL_TRUE) {
		glGetProgramInfoLog(program, 1024, &length, buffer);
		LOG_ERROR("Shader program: %s\n", (const char*)buffer);
	}
	
	OPENGL_CHECK_FOR_ERRORS();
	
	return status;
}
//

//
char *LoadTextFile(const char *fn) {
	FILE *f = fopen(fn, "r");
	char *text = (char*)malloc(sizeof(char));
	int n = 0;
	
	while (!feof(f)) {
		text = (char*)realloc(text, sizeof(char)*(n+1));
		fread(&text[n], sizeof(char), 1, f);
		n++;
	}
	
	text = (char*)realloc(text, sizeof(char)*(n+1));
	text[n-1] = '\0';

	fclose(f);
	
	return text;
}

//
// GetChar - Return next char from stream
char GetChar(FILE *InFile) {
	static int buf;

	if (feof(InFile)) return EOF;
	
	size_t a = fread(&buf, sizeof(char), 1, InFile);

	if (buf == '\r') {
		size_t a = fread(&buf, sizeof(char), 1, InFile);
	}
	
	return buf;
}

//	
void BaseShader::AddVertexShader(const char *fn) {
	char *vp = LoadTextFile(fn);
	const char *vv = vp;
	
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vv, NULL);
	
	puts(vp);
	free(vp);
	glCompileShader(vs);
	glAttachShader(ProgramObject, vs);
}

//
void BaseShader::AddFragmentShader(const char *fn) {
	char *fp = LoadTextFile(fn);
	const char *ff = fp;
	
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &ff, NULL);
	
	puts(fp);
	free(fp);
	
	glCompileShader(fs);
	glAttachShader(ProgramObject, fs);
}

//
bool BaseShader::AddGLSLShader(const char *fn) {
	//
	FileName = string(fn);
	char fp[20048], vp[20048];
	int fi = 0; int vi = 0;
	int *ci = &fi;
	char *cp = fp;
	//
	FILE *f = fopen(fn, "r");
	char ch;
	//
	while (!feof(f)) {
		ch = GetChar(f);
		
		if (ch == '#') {
			char id[20];
			int ii = 0;
			ch = GetChar(f);
			
			while (ch != EOF & ch != '\n' & ch != '\r') {
				//
				id[ii] = ch;
				ii++;
				ch = GetChar(f);
			}
			//
			id[ii] = '\0';
			//
			if (!strcmp(id ,"vertex shader\0")) {
				cp = vp;
				ci = &vi;
			} else if (!strcmp(id ,"fragment shader\0")) {
				cp = fp;
				ci = &fi;
			} else {
				cp[*ci] = '#';
				(*ci)++;
				//
				for (int i = 0; i < ii; i++) {
					cp[*ci] = id[i];
					(*ci)++;
				}
				//
			}
			//
		} else {
			cp[*ci] = ch;
			(*ci)++;
		}
	}
	//
	fp[fi] = '\0'; vp[vi] = '\0';
	//
	fclose(f);
	//
	const char *ff = fp;
	const char *vv = vp;
	//
	//LOG_DEBUG("%s\n", fp);
	//LOG_DEBUG("\n\n%s\n", vp);
	//
	ProgramObject = glCreateProgram();
	
	// Vertex shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vv, NULL);
	glCompileShader(vs);
	
	//
	if (ShaderStatus(vs, GL_COMPILE_STATUS) != GL_TRUE)
		return false;
	
	glAttachShader(ProgramObject, vs);
	
	// Fragment shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &ff, NULL);
	glCompileShader(fs);
	
	//
	//
	if (ShaderStatus(fs, GL_COMPILE_STATUS) != GL_TRUE)
		return false;
	
	glAttachShader(ProgramObject, fs);
	
	if (ShaderProgramStatus(ProgramObject, GL_LINK_STATUS) != GL_TRUE)
		return false;
	
	//
	glValidateProgram(ProgramObject);
	
	if (ShaderProgramStatus(ProgramObject, GL_VALIDATE_STATUS) != GL_TRUE)
		return false;
	//
}

//
void BaseShader::ApplyShader() {
	if (Enable) {
		LastShader = this;
		glUseProgram(ProgramObject);
	}
}

//
void BaseShader::UnApplyShader() {
	LastShader = 0;
	glUseProgram(0);
}

//

#endif
