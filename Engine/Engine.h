//* Author : Kabilin Andrey, 2013 *//

#ifndef H_ENGINE
#define H_ENGINE
//
class Engine {
public:
	UITheme		*Theme;
	UIManager	*GUIManager;
	UIElement	*SplashScreen;
	UITabs		*ToolTabs;

	glm::vec2	 TargetSize;
	glm::vec2	 TargetTexSize;
	glm::vec2	 TargetTexOffset;

	int		  VertexSize[6];
	float	 fVertexSize[6];
	float	VertexOffset[6];

	// Threads
	//sf::Thread UIThread;
	//
	//UIData *MainForm;
	//UIData *FileDialog;
	//MapEditor2D *MapEditor;

	//
	Engine();
	~Engine();
	///
	void Draw();
	void Step();
	void Init();
	
	// Events
	void Resized     (int Width, int Height);
	void KeyPressed  (Uint16 key);
	void KeyReleased (Uint16 key);
	void TextEntered (Uint16 key);
	void MouseDown   (int x, int y, int shift);
	void MouseUp     (int x, int y, int shift);
	void MouseMove   (int x, int y, int shift);
	void MouseWheel  (int delta, int x, int y);
	void DblClick    (int x, int y, int shift);
};

DataMap *Config;
Engine  *Core;

#endif
