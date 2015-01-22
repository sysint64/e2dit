//
//

#ifndef CPP_ANIMATION
#define CPP_ANIMATION

//
void AnimData2D::SetFrame(const int Frame) {
	OffsetY = ceil(Frame/hCount);
	OffsetX = Frame-OffsetY*hCount;
	Time = 0.0f;
}

// Initialize
void AnimData2D::Init() {
	//
	/*OffsetsTable = (glm::vec2**)LinearAlloc(&TablesBuffer, sizeof(glm::vec2*)*hCount, 4);
	//
	for (int i = 0; i < hCount; i++) {
		OffsetsTable[i] = (glm::vec2*)LinearAlloc(&TablesBuffer, sizeof(glm::vec2)*vCount, 4);
	}
	//
	for (int i = 0; i < hCount; i++) {
		//
		for (int j = 0; j < vCount; j++) {
			OffsetsTable[i][j] = glm::vec2((FrameWidth*i) / TexWidth, (FrameHeight*j) / TexHeight);
		}
		//
	}
	//
	Time = 0.0f;*/
	//
}

//
void AnimData2D::Step(const float deltaTime) {
	//
	Time += deltaTime;
	//
	if (Time == Interval) {
		OffsetX++;
		Time = 0.0f;
		
		if (OffsetX >= hCount) {
			OffsetX = 0;
			OffsetY++;
		}
		//
		if (OffsetY >= vCount) {
			OffsetX = 0;
			OffsetY = 0;
		}
		//
	}
	//
}

#endif
