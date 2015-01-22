//* Author : Kabilin Andrey, 2013 *//

#ifndef CPP_EVENTS
#define CPP_EVENTS

// Events
void Engine::Resized(int Width, int Height) {
	GUIManager->Resized(Width, Height);
}
//
void Engine::KeyPressed(Uint16 key) {
	finder->KeyPressed(key);
	GUIManager->KeyPressed(key);
	MapEditor->KeyPressed(key);
	addSpriteForm->KeyPressed(key);
	setPolygonForm->KeyPressed(key);
	HotKeys::KeyPressed(key);
}

void Engine::KeyReleased(Uint16 key) {
	HotKeys::KeyReleased(key);
}

void Engine::TextEntered(Uint16 key) {
	GUIManager->TextEntered(key);
}

//
void Engine::MouseDown(int x, int y, int shift) {
	if (!isKey[keyRAlt] && !isKey[keyLAlt])
		GUIManager->MouseDown(x, y, shift);

	MapEditor->MouseDown(x, y, shift);
	addSpriteForm->MouseDown(x, y, shift);
	colorDialog->MouseDown(x, y, shift);
	setPolygonForm->MouseDown(x, y, shift);
	SplashScreen->Visible = false;
}

void Engine::MouseUp(int x, int y, int shift) {
	if (!isKey[keyRAlt] && !isKey[keyLAlt])
		GUIManager->MouseUp(x, y, shift);
	
	MapEditor->MouseUp(x, y, shift);
	addSpriteForm->MouseUp(x, y, shift);
	colorDialog->MouseUp(x, y, shift);
	setPolygonForm->MouseUp(x, y, shift);
}

void Engine::MouseMove(int x, int y, int shift) {
	GUIManager->MouseMove(x, y, shift);
	MapEditor->MouseMove(x, y, shift);
	addSpriteForm->MouseMove(x, y, shift);
}

void Engine::MouseWheel (int delta, int x, int y) {
	//GUIManager->MouseWheel (delta, x, y);
	MapEditor ->MouseWheel (delta, x, y);
}

void Engine::DblClick(int x, int y, int shift) {
	GUIManager->DblClick(x, y, shift);
	MapEditor->DblClick(x, y, shift);
}

#endif
