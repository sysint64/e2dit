//"I have \(apples + oranges) pieces of fruit."

#ifndef H_HOTKEYS
#define H_HOTKEYS

//
namespace HotKeys {

	Uint16 CurKey;
	struct HotKey {

		/* Keys */

		Uint16 Key;
		bool   Shift;
		bool   Ctrl;
		bool   Alt;
		bool   Super;
		bool   Skip;

		/* Mouse */

		bool   LMC; // Left   Mouse Click
		bool   RMC; // Right  Mouse Click
		bool   MMC; // Middle Mouse Click
		bool   MMW; // Middle Mouse Wheel

		/* Actions */

		NotifyEvent Action; 
	};

	map<string, HotKey> Keys;
	bool PollAction = false;

	/* Main functiond */

	void Init();
	void Step();
	void KeyPressed   (Uint16 Key);
	void KeyReleased  (Uint16 Key);
	void InsertHotKey (const string name, NotifyEvent Action = 0, UIElement *UI = 0);
}

#endif