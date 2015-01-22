//"I have \(apples + oranges) pieces of fruit."

#ifndef CPP_HOTKEYS
#define CPP_HOTKEYS

namespace HotKeys {

	/* Initialize */

	void Init() {

		/* Definite all Hot Keys */
		/* Menu */
		/* File */

		InsertHotKey ("New");
		InsertHotKey ("Open");
		InsertHotKey ("OpenRecent");
		InsertHotKey ("Save");
		InsertHotKey ("SaveAs");
		InsertHotKey ("Preferences");
		InsertHotKey ("Quit", MapEditor->Quit, MapEditor->Data->Elements["liquit"]);

		/* Edit */

		InsertHotKey ("Undo");
		InsertHotKey ("Redo");
		InsertHotKey ("Cut");
		InsertHotKey ("Copy");
		InsertHotKey ("Paste");
		InsertHotKey ("Delete");
		InsertHotKey ("DeleteAllByType");
		InsertHotKey ("SelectAll");
		InsertHotKey ("DeselectAll");
		InsertHotKey ("Dublicate", MapEditor->Quit, MapEditor->Data->Elements["liquit"]);

		/* Modify */

		InsertHotKey ("Retopology", MapEditor->Retopology);
		InsertHotKey ("CenterPivot");
		InsertHotKey ("SetPivot");
		InsertHotKey ("JoinPointToVertex");

		/* View */

		InsertHotKey ("PanelTool");
		InsertHotKey ("ObjectInspector");
		InsertHotKey ("Grid");
		InsertHotKey ("PhysBounds");
		InsertHotKey ("PhysJoins");
		InsertHotKey ("PhysOnlySelect");
		InsertHotKey ("RealTimeLighting");

		/* Help */

		InsertHotKey ("Manual");
		InsertHotKey ("Website");
		InsertHotKey ("Community");
		InsertHotKey ("SplashScreen");

		/* End Menu */
		
		/* Left Tool Panel */

		InsertHotKey ("Move"  , MapEditor->SetToolMove);
		InsertHotKey ("Rotate", MapEditor->SetToolRotate);
		InsertHotKey ("Scale" , MapEditor->SetToolScale);
		InsertHotKey ("Panorama");
		InsertHotKey ("Zoom");
		
		//

		InsertHotKey ("SetTransformMode1");
		InsertHotKey ("SetTransformMode2");
		InsertHotKey ("SetTransformMode3");

		/* Bottom Panel */

		InsertHotKey ("EditGridSnap");
		InsertHotKey ("GridSnap");
		InsertHotKey ("HalfOffsetGridSnap");
		InsertHotKey ("ObjectMode");
		InsertHotKey ("EditMode");
		InsertHotKey ("AddMode");

		/* Main Tool Panel */

		InsertHotKey ("Sprite");
		InsertHotKey ("Polygon");
		InsertHotKey ("Tiles");
		InsertHotKey ("Point");
		InsertHotKey ("Light");
		InsertHotKey ("Sound");
		//
		InsertHotKey ("Knife");
		InsertHotKey ("Divide");
		InsertHotKey ("CutByLine");
		InsertHotKey ("Partition");
		//
		InsertHotKey ("Bend");
		InsertHotKey ("Screw");
		InsertHotKey ("Wave");
		InsertHotKey ("Widen");
		InsertHotKey ("Arc");
		InsertHotKey ("ArcCompile");

		/* Additions */

		InsertHotKey ("AddSprite" , MapEditor->AddSpriteFile);
		InsertHotKey ("AddPolygon", MapEditor->AddPolygon);
		InsertHotKey ("AddTiles");
		InsertHotKey ("AddPoint"  , MapEditor->AddPointClick);
		InsertHotKey ("AddLight"  , MapEditor->AddLightClick);
		InsertHotKey ("AddSound");
		InsertHotKey ("AddLayer");

		/* Main functiond */

	}

	/* Events */

	void Step() {

		if (PollAction)
			return;

		for (map<string, HotKey>::iterator it = Keys.begin(); it != Keys.end(); it++) {

			HotKey *Key = &(it->second);
			
			if (Key->Skip)
				continue;

			if (Key->Action != 0) {

				if (Key->Ctrl  && !(isKey[keyRCtrl ] || isKey[keyLCtrl ])) continue;
				if (Key->Shift && !(isKey[keyRShift] || isKey[keyLShift])) continue;
				if (Key->Alt   && !(isKey[keyRAlt]   || isKey[keyLAlt  ])) continue;
				if (Key->Super && !(isKey[keyRSuper] || isKey[keyLSuper])) continue;
				if (Key->Key != CurKey)									   continue;

				Key->Action(0);
				PollAction = true;
			}

		}

	}

	void KeyPressed (Uint16 Key) {
		CurKey = Key;
	}

	void KeyReleased (Uint16 Key) {
		CurKey = -1;
		PollAction = false;
	}

	/* Implementation */

	void ParseKey (HotKey *Obj, const string Key) {
		
		string token = "";
		vector<string> Keys;

		for (int i = 0; i < Key.size(); i++) {

			if (Key[i] == ' ') {
				
				if (token != "") {
					Keys.push_back(token);
					token = "";
				}

				continue;
			}

			token += Key[i];

		}

		Keys.push_back(token);
		
		for (int i = 0; i < Keys.size(); i++) {

			/* Control Keys */

			if (Keys[i] == "Ctrl")  { Obj->Ctrl  = true; continue; }
			if (Keys[i] == "Shift") { Obj->Shift = true; continue; }
			if (Keys[i] == "Alt")   { Obj->Alt   = true; continue; }
			if (Keys[i] == "Super") { Obj->Super = true; continue; }

			/*  */

			if (Keys[i] == "Esc")       { Obj->Key = sf::Keyboard::Escape;    continue; }
			if (Keys[i] == "Menu")      { Obj->Key = sf::Keyboard::Menu;      continue; }
			if (Keys[i] == "Enter")     { Obj->Key = sf::Keyboard::Return;    continue; }
			if (Keys[i] == "Tab")       { Obj->Key = sf::Keyboard::Tab;       continue; }
			if (Keys[i] == "Space")     { Obj->Key = sf::Keyboard::Space;     continue; }
			if (Keys[i] == "BackSpace") { Obj->Key = sf::Keyboard::BackSpace; continue; }
			if (Keys[i] == "PageUp")    { Obj->Key = sf::Keyboard::PageUp;    continue; }
			if (Keys[i] == "PageDown")  { Obj->Key = sf::Keyboard::PageDown;  continue; }
			if (Keys[i] == "End")       { Obj->Key = sf::Keyboard::End;       continue; }
			if (Keys[i] == "Home")      { Obj->Key = sf::Keyboard::Home;      continue; }
			if (Keys[i] == "Ins")       { Obj->Key = sf::Keyboard::Insert;    continue; }
			if (Keys[i] == "Del")       { Obj->Key = sf::Keyboard::Delete;    continue; }
			if (Keys[i] == "Left")      { Obj->Key = sf::Keyboard::Left;      continue; }
			if (Keys[i] == "Right")     { Obj->Key = sf::Keyboard::Right;     continue; }
			if (Keys[i] == "Up")        { Obj->Key = sf::Keyboard::Up;        continue; }
			if (Keys[i] == "Down")      { Obj->Key = sf::Keyboard::Down;      continue; }
			if (Keys[i] == "Pause")     { Obj->Key = sf::Keyboard::Pause;     continue; }

			/* Symbold */

			if (Keys[i] == "+" ) { Obj->Key = sf::Keyboard::Add;       continue; }
			if (Keys[i] == "-" ) { Obj->Key = sf::Keyboard::Subtract;  continue; }
			if (Keys[i] == "*" ) { Obj->Key = sf::Keyboard::Multiply;  continue; }
			if (Keys[i] == "/" ) { Obj->Key = sf::Keyboard::Divide;    continue; }
			if (Keys[i] == "=" ) { Obj->Key = sf::Keyboard::Equal;     continue; }
			if (Keys[i] == "[" ) { Obj->Key = sf::Keyboard::LBracket;  continue; }
			if (Keys[i] == "]" ) { Obj->Key = sf::Keyboard::RBracket;  continue; }
			if (Keys[i] == ";" ) { Obj->Key = sf::Keyboard::SemiColon; continue; }
			if (Keys[i] == "," ) { Obj->Key = sf::Keyboard::Comma;     continue; }
			if (Keys[i] == "." ) { Obj->Key = sf::Keyboard::Period;    continue; }
			if (Keys[i] == "'" ) { Obj->Key = sf::Keyboard::Quote;     continue; }
			if (Keys[i] == "/" ) { Obj->Key = sf::Keyboard::Slash;     continue; }
			if (Keys[i] == "\\") { Obj->Key = sf::Keyboard::BackSlash; continue; }

			/* F1 - F12 */

			if ((Keys[i].size() == 2 || Keys[i].size() == 3) && Keys[i][0] == 'F') {

				char *ptr = const_cast<char*>(Keys[i].c_str());
				Obj->Key = sf::Keyboard::F1+StrToInt(ptr+1)-1;
				continue;

			}

			/* Numpad0-Numpad9 */

			if (Keys[i].size() == 7) {

				char *ptr = const_cast<char*>(Keys[i].c_str());
				char  num = ptr[6];
				ptr[6] = '\0';

				if (strcmp(ptr, "Numpad") == 0) {
					
					Obj->Key = sf::Keyboard::Numpad0+num-'0';
					continue;

				}

			}

			if (Keys[i].size() != 1) continue;

			/* Letters */

			if (Keys[i][0] <= 'Z' && Keys[i][0] >= 'A') {

				Obj->Key = sf::Keyboard::A+Keys[i][0]-'A';
				continue;

			}

			/* Numbers */

			if (Keys[i][0] <= '9' && Keys[i][0] >= '0') {

				Obj->Key = sf::Keyboard::Num0+Keys[i][0]-'0';
				continue;

			}
		}

	}

	inline void InsertHotKey (const string name, NotifyEvent Action, UIElement *UI) {
		HotKey Obj;
		memset(&Obj, 0, sizeof(HotKey));
		const string Key = Config->Data["hotkeys"].Func[LowerCaseS(name)][0].Str;

		if (Key != "") ParseKey (&Obj, Key);
		else Obj.Skip = true;

		Obj.Action = Action;

		if (UI != 0) {
			((UIFlatButton*) UI)->ShortKey = Key;
		}

		/* Set Key Map */

		Keys[LowerCaseS(name)] = Obj;
	}

}

#endif
