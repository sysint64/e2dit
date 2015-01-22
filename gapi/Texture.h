//
#ifndef H_TEXTURE
#define H_TEXTURE

//
class Texture2D {
public:
	//
	int	 Width;
	int	 Height;
	int	 bpp;
	int	 Format;
	int	 ImageType;
	//
	BYTE	*data;
	GLuint	 type;
	GLuint	 Handle;
	GLuint	 WrapS;
	GLuint	 WrapT;
	GLuint	 EnvMode;
	GLuint	 Filter;
	string	 FileName;
	bool	 BuildMipmaps;
	bool	 Compressed;
	bool	 PremutableAlpha;
	
	//
	Texture2D();
	~Texture2D();;
	//
	bool LoadFromFile(const char *fn);
	void ApplyTexture();
	void UnApplyTexture();
	void Update();
};

#endif
