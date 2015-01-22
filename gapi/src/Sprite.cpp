//
//

#ifndef CPP_SPRITE
#define CPP_SPRITE

//
SpriteData::SpriteData() {
	//
	//DataRender::Init();
	DataRender();
	RenderMode = GL_TRIANGLES;
	Static = false;

	// QUAD
	AddVertex (glm::vec2(-0.5f, -0.5f), glm::vec2(0.0f, 1.0f));
	AddVertex (glm::vec2( 0.5f, -0.5f), glm::vec2(1.0f, 1.0f));
	AddVertex (glm::vec2( 0.5f,  0.5f), glm::vec2(1.0f, 0.0f));
	AddVertex (glm::vec2(-0.5f,  0.5f), glm::vec2(0.0f, 0.0f));
	//
	AddIndex(0); AddIndex(3); AddIndex(1);
	AddIndex(2); AddIndex(3); AddIndex(1);
	//
}

//
SpriteData::~SpriteData() {

}
//

//
GUIElementData::GUIElementData() {
	//
	//DataRender::Init();
	DataRender();
	RenderMode = GL_TRIANGLE_STRIP;
	
	// QUAD
	AddVertex (glm::vec2 (0.0f, 0.0f), glm::vec2 (0.0f, 1.0f));
	AddVertex (glm::vec2 (1.0f, 0.0f), glm::vec2 (1.0f, 1.0f));
	AddVertex (glm::vec2 (1.0f, 1.0f), glm::vec2 (1.0f, 0.0f));
	AddVertex (glm::vec2 (0.0f, 1.0f), glm::vec2 (0.0f, 0.0f));
	//
	AddIndex(0); AddIndex(3); AddIndex(1); AddIndex(2);
	//
}

//
GUIElementData::~GUIElementData() {

}

//

#endif
