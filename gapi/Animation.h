//
//

#ifndef H_ANIMATION
#define H_ANIMATION

//
class AnimData2D {
public:
	wstring Name;
	glm::vec2 **OffsetsTable;
	float		NormFrameWidth,
				NormFrameHeight;
	//
	int   Interval;
	float Time;
	//
	int FrameWidth, FrameHeight;
	int TexWidth  , TexHeight;
	int vCount    , hCount;
	int OffsetX   , OffsetY;
	//
	inline void Init();
	void Step(const float deltaTime);
	void SetFrame(const int Frame);
};
//

#endif
