#ifndef CPP_TEXTURE
#define CPP_TEXTURE

//
Texture2D::Texture2D() {
	//
	Filter = GL_LINEAR;
	WrapS = GL_CLAMP_TO_EDGE;
	WrapT = GL_CLAMP_TO_EDGE;
	BuildMipmaps = false;
	Compressed = false;
	//
	glGenTextures(1, &Handle);
}

//
Texture2D::~Texture2D() {
	//
	glDeleteTextures(1, &Handle);
	SOIL_free_image_data(data);
}

//
	
bool Texture2D::LoadFromFile (const char *fn) {
	data = 0;
	data = SOIL_load_image (fn, &Width, &Height, 0, SOIL_LOAD_RGBA);
	
	//if (data == NULL) return false;

	Format = GL_RGBA;
	ImageType = GL_UNSIGNED_BYTE;
	FileName = fs::extractFileName (fn);

	return true;
}

//
void Texture2D::ApplyTexture() {
	glBindTexture(GL_TEXTURE_2D, Handle);
}

//
void Texture2D::UnApplyTexture() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

//
void Texture2D::Update() {
	//
	//if (Compressed) return;

	glBindTexture   (GL_TEXTURE_2D, Handle);
	glTexImage2D    (GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, ImageType, data);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     WrapS);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     WrapT);
	//
	if (BuildMipmaps)
		gluBuild2DMipmaps (GL_TEXTURE_2D, Format, Width, Height, Format, ImageType, data);
}

//

#endif
