//
//

#ifndef H_SPRITE
#define H_SPRITE

//
class SpriteData : public DataRender {
public:
	float Width, Height;
	//
	SpriteData();
	~SpriteData();
};

//
class GUIElementData : public DataRender {
public:
	float Width, Height;
	//
	GUIElementData();
	~GUIElementData();
};

#endif
