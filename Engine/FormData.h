#ifndef H_UIDATA
#define H_UIDATA

class UIData : public DataMap {
private:
	inline void FillData();

public:
	map <string, UIElement*> Elements;
	UIManager *Manager;
	//
	virtual void LoadFromFile     (const char *fn);
	virtual void LoadFromBytecode (const char *fn);
	//
	UIData() { FillType = Vector; }
};

#include "src/FormData.cpp"
#endif
