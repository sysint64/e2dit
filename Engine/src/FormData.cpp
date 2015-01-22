#ifndef CPP_UIDATA
#define CPP_UIDATA

//
inline void UIData::FillData() {
	//
	//int n = DataStack.size();
	//if (n % 2 != 0) n++;
	//
	//Elements = new HashSet <UIElement*> (n);
	//
	for (int i = 0; i < DataStack.size(); i++) {
		//
		UIElement *Element = 0;
		//
		if (DataStack[i].Type == "panel") {
			Element = new UIPanel();
			//

			if (DataStack[i].Func.count ("clamp") != 0) {
				((UIPanel*)Element)->MinSize = DataStack[i].Func["clamp"][0].Num;
				((UIPanel*)Element)->MaxSize = DataStack[i].Func["clamp"][1].Num;
			}
			//
			if (DataStack[i].Func.count ("resize") != 0)
				((UIPanel*)Element)->AllowResize = (bool) DataStack[i].Func["resize"][0].Num;
			//
			if (DataStack[i].Func.count ("scroll") != 0)
				((UIPanel*)Element)->ShowScroll = (bool) DataStack[i].Func["scroll"][0].Num;	

			if (DataStack[i].Func.count ("canhide") != 0)
				((UIPanel*)Element)->CanHide = (bool) DataStack[i].Func["canhide"][0].Num;	

			if (DataStack[i].Func.count ("caption") != 0)
				((UIPanel*)Element)->Caption = StrToWStr (DataStack[i].Func["caption"][0].Str);

			if (DataStack[i].Func.count ("useincolor") != 0)
				((UIPanel*)Element)->UseInColor = (bool) DataStack[i].Func["useincolor"][0].Num;

			if (DataStack[i].Func.count ("actioncolor") != 0) {
				((UIPanel*)Element)->UseActionColor = (bool) DataStack[i].Func["actioncolor"][0].Num;
			}

			if (DataStack[i].Func.count ("join") != 0)
				((UIPanel*)Element)->Join = (UIPanel*) Elements[const_cast<char*>(DataStack[i].Func["join"][0].Str.c_str())];

			if (DataStack[i].Func.count ("tabjoin") != 0)
				((UIPanel*)Element)->TabJoin = Elements[const_cast<char*>(DataStack[i].Func["tabjoin"][0].Str.c_str())];
				//((UITabElement*)Elements[const_cast<char*>(DataStack[i].Func["tabjoin"][0].Str.c_str())])->Join = ((UIPanel*)Element);

			if (DataStack[i].Func.count ("transparent") != 0)
				((UIPanel*)Element)->DrawQuad = !(bool) DataStack[i].Func["transparent"][0].Num;
		}
		//
		if (DataStack[i].Type == "dialog") {
			//
			Element = new UIDialog();
			//
			if (DataStack[i].Func.count ("resize") != 0)
				((UIPanel*)Element)->AllowResize = (bool) DataStack[i].Func["resize"][0].Num;
			//
			if (DataStack[i].Func.count ("scroll") != 0)
				((UIPanel*)Element)->ShowScroll = (bool) DataStack[i].Func["scroll"][0].Num;

			//
			if (DataStack[i].Func.count ("caption") != 0)
				((UIPanel*)Element)->Caption = StrToWStr (DataStack[i].Func["caption"][0].Str);
		}
		//
		if (DataStack[i].Type == "tabs") {
			Element = new UITabs();	

			if (DataStack[i].Func.count("offset") != 0)
				((UITabs*)Element)->TabsOffset = DataStack[i].Func["offset"][0].Num;
		}

		if (DataStack[i].Type == "tabelement") {
			Element = new UITabElement();	

			if (DataStack[i].Func.count("caption") != 0)
				((UITabElement*)Element)->Caption = StrToWStr (DataStack[i].Func["caption"][0].Str);
			//
			if (DataStack[i].Func.count("checked") != 0) {
				((UITabElement*)Element)->Checked = (bool) DataStack[i].Func["checked"][0].Num;
			}

			if (DataStack[i].Func.count("cancheck") != 0)
				((UITabElement*)Element)->CanCheck = (bool) DataStack[i].Func["cancheck"][0].Num;
		}

		if (DataStack[i].Type == "tabsplit")
			Element = new UITabSplit();

		if (DataStack[i].Type == "tab") {
			//
			Element = new UITab();

			if (DataStack[i].Func.count("caption") != 0)
				((UIButton*)Element)->Caption = StrToWStr (DataStack[i].Func["caption"][0].Str);
			//
			if (DataStack[i].Func.count("active") != 0)
				((UITab*)Element)->Active = (bool) DataStack[i].Func["active"][0].Num;
		}
		//
		if (DataStack[i].Type == "hr") Element = new UIHr();
		if (DataStack[i].Type == "image") {
			Element = new UIImage();
			//
			if (DataStack[i].Func.count("texture") != 0)
				((UIImage*)Element)->Texture = Textures::GetByName (DataStack[i].Func["texture"][0].Str);
			//
			if (DataStack[i].Func.count("offset") != 0) {
				((UIImage*)Element)->itOffsetX = floor(DataStack[i].Func["offset"][0].Num);
				((UIImage*)Element)->itOffsetY = floor(DataStack[i].Func["offset"][1].Num);
			}
			//
			if (DataStack[i].Func.count("texsize") != 0) {
				//
				((UIImage*)Element)->itWidth  = floor(DataStack[i].Func["texsize"][0].Num);
				((UIImage*)Element)->itHeight = floor(DataStack[i].Func["texsize"][1].Num);
				//
			} else if (DataStack[i].Func.count("size") != 0) {
				//
				((UIImage*)Element)->itWidth  = floor(DataStack[i].Func["size"][0].Num);
				((UIImage*)Element)->itHeight = floor(DataStack[i].Func["size"][1].Num);
				//
			} else {
				((UIImage*)Element)->itWidth  = ((UIImage*)Element)->Texture->Width;
				((UIImage*)Element)->itHeight = ((UIImage*)Element)->Texture->Height;
			}
			//
		}
		//
		if (DataStack[i].Type == "colorpanel") {
			Element = new UIColorPanel();
			//
			/*if (DataStack[i].Func.count("offset") != 0) {
				((UIImage*)Element)->itOffsetX = floor(DataStack[i].Func["offset"][0].Num);
				((UIImage*)Element)->itOffsetY = floor(DataStack[i].Func["offset"][1].Num);
			}*/
			//
			if (DataStack[i].Func.count("size") != 0) {
				((UIColorPanel*)Element)->itWidth  = floor(DataStack[i].Func["size"][0].Num);
				((UIColorPanel*)Element)->itHeight = floor(DataStack[i].Func["size"][1].Num);
			}
			//
			if (DataStack[i].Func.count("showgrid") != 0) {
				((UIColorPanel*)Element)->ShowGrid = (bool) DataStack[i].Func["showgrid"][0].Num;
			}
			//
			if (DataStack[i].Func.count("color") != 0) {
				((UIColorPanel*)Element)->Color = glm::vec4(DataStack[i].Func["color"][0].Num/255.f,
															DataStack[i].Func["color"][1].Num/255.f,
															DataStack[i].Func["color"][2].Num/255.f,
															DataStack[i].Func["color"][3].Num/100.f);
			}
			//
		}
		//
		if (DataStack[i].Type == "groupedelements") {
			Element = new UIGroupedElements();
		}
		//
		if (DataStack[i].Type == "label") {
			Element = new UILabel();

			if (DataStack[i].Func.count("caption") != 0)
				((UILabel*)Element)->Caption = StrToWStr (DataStack[i].Func["caption"][0].Str);

			if (DataStack[i].Func.count("textalign") != 0) {
				if (DataStack[i].Func["textalign"][0].Str == "left")   ((UILabel*)Element)->TextAlign = alLeft;
				if (DataStack[i].Func["textalign"][0].Str == "right")  ((UILabel*)Element)->TextAlign = alRight;
				if (DataStack[i].Func["textalign"][0].Str == "center") ((UILabel*)Element)->TextAlign = alCenter;
			}
		}
		//
		if (DataStack[i].Type == "button") {
			//
			if (DataStack[i].Func.count("flat") != 0) {
				if (DataStack[i].Func["flat"][0].Num == 1.0f) Element = new UIFlatButton();
				else Element = new UIButton();
			} else {
				Element = new UIButton();
			}

			if (DataStack[i].Func.count("caption") != 0)
				((UIButton*)Element)->Caption = StrToWStr (DataStack[i].Func["caption"][0].Str);
		}
		//
		if (DataStack[i].Type == "edit") {
			Element = new UIEdit();

			if (DataStack[i].Func.count("text") != 0)
				((UIEdit*)Element)->wText = StrToWStr (DataStack[i].Func["text"][0].Str);

			if (DataStack[i].Func.count("before") != 0)
				((UIEdit*)Element)->Before = StrToWStr (DataStack[i].Func["before"][0].Str);

			if (DataStack[i].Func.count("after") != 0)
				((UIEdit*)Element)->After = StrToWStr (DataStack[i].Func["after"][0].Str);

			if (DataStack[i].Func.count("trackbar") != 0)
				((UIEdit*)Element)->TrackBar = (bool) DataStack[i].Func["trackbar"][0].Num;

			if (DataStack[i].Func.count("intfilter") != 0) {
				((UIEdit*)Element)->IntFilter = (bool) DataStack[i].Func["intfilter"][0].Num;

				if (DataStack[i].Func.count("range") != 0) {
					((UIEdit*)Element)->iMinVal = (int) DataStack[i].Func["range"][0].Num;
					((UIEdit*)Element)->iMaxVal = (int) DataStack[i].Func["range"][1].Num;
				}
			}

			if (DataStack[i].Func.count("floatfilter") != 0) {
				((UIEdit*)Element)->IntFilter = (bool) DataStack[i].Func["floatfilter"][0].Num;

				if (DataStack[i].Func.count("range") != 0) {
					((UIEdit*)Element)->fMinVal = DataStack[i].Func["range"][0].Num;
					((UIEdit*)Element)->fMaxVal = DataStack[i].Func["range"][1].Num;
				}
			}

			if (DataStack[i].Func.count("trackstep") != 0)
				((UIEdit*)Element)->TrackStep = (int) DataStack[i].Func["trackstep"][0].Num;
		}
		//
		if (DataStack[i].Type == "checkbutton") {
			//
			Element = new UICheckButton();

			if (DataStack[i].Func.count("caption") != 0)
				((UIButton*)Element)->Caption = StrToWStr (DataStack[i].Func["caption"][0].Str);
			//
			if (DataStack[i].Func.count("checked") != 0)
				((UICheckButton*)Element)->Checked = (bool) DataStack[i].Func["checked"][0].Num;

			if (DataStack[i].Func.count("textalign") != 0) {
				if (DataStack[i].Func["textalign"][0].Str == "left")   ((UICheckButton*)Element)->TextAlign = alLeft;
				if (DataStack[i].Func["textalign"][0].Str == "right")  ((UICheckButton*)Element)->TextAlign = alRight;
				if (DataStack[i].Func["textalign"][0].Str == "center") ((UICheckButton*)Element)->TextAlign = alCenter;
			}
		}
		//
		if (DataStack[i].Type == "checkbox") {
			//
			Element = new UICheckBox();

			if (DataStack[i].Func.count("caption") != 0)
				((UICheckBox*)Element)->Caption = StrToWStr (DataStack[i].Func["caption"][0].Str);
			//
			if (DataStack[i].Func.count("checked") != 0)
				((UICheckBox*)Element)->Checked = (bool) DataStack[i].Func["checked"][0].Num;

			if (DataStack[i].Func.count("textalign") != 0) {
				if (DataStack[i].Func["textalign"][0].Str == "left")   ((UICheckButton*)Element)->TextAlign = alLeft;
				if (DataStack[i].Func["textalign"][0].Str == "right")  ((UICheckButton*)Element)->TextAlign = alRight;
				if (DataStack[i].Func["textalign"][0].Str == "center") ((UICheckButton*)Element)->TextAlign = alCenter;
			}
		}
		//
		if (DataStack[i].Type == "listitem") {
			//
			if (DataStack[i].Func.count("tall") != 0 && DataStack[i].Func["tall"][0].Num == 1.0f) {
				Element = new UITallFlatButton();
				//
				((UITallFlatButton*)Element)->Texture = Textures::GetByName (DataStack[i].Func["texture"][0].Str);
				if (DataStack[i].Func.count("descr") != 0)
					((UITallFlatButton*) Element)->Descr = StrToWStr (DataStack[i].Func["descr"][0].Str);
			} else

			if (DataStack[i].Func.count("flat") != 0) {
				//
				if (DataStack[i].Func["flat"][0].Num == 1.0f) Element = new UIFlatButton (false);
				else Element = new UIFlatButton (true);
			} else Element = new UIFlatButton (true);
			//
			((UIFlatButton*) Element)->Caption = StrToWStr (DataStack[i].Func["caption"][0].Str);
			//
			if (DataStack[i].Func.count("shortkey") != 0)
				((UIFlatButton*) Element)->ShortKey = DataStack[i].Func["shortkey"][0].Str;

			if (DataStack[i].Func.count("checked") != 0)
				((UIFlatButton*) Element)->Checked = (bool) DataStack[i].Func["checked"][0].Num;

			if (DataStack[i].Func.count("menu") != 0) {
				((UIFlatButton*) Element)->Menu = (UIListMenu*) Elements[const_cast<char*>(DataStack[i].Func["menu"][0].Str.c_str())];
			}
		}
		//
		if (DataStack[i].Type == "listmenu") {
			//
			if (DataStack[i].Func.count("transparent") != 0) {
				if (DataStack[i].Func["transparent"][0].Num == 1.0f) Element = new UIListMenu (false);
				else Element = new UIListMenu (true);
			} else Element = new UIListMenu (true);

			//
			if (DataStack[i].Func.count("checklist") != 0) {
				((UIListMenu*) Element)->CheckList = (bool) DataStack[i].Func["transparent"][0].Num;
			}

			for (int j = 0; j < DataStack[i].Func["items"].size(); j++) {
				UIElement *Item = Elements[const_cast<char*>(DataStack[i].Func["items"][j].Str.c_str())];
				Item->inDialog = Element->inDialog;
				((UIListMenu*) Element)->AddItem ((UIFlatButton*) Item);
			}
		}
		//
		if (DataStack[i].Type == "dropmenu") {
			bool Flat = (bool) DataStack[i].Func["flat"][0].Num;
			Element = new UIDropMenu (Flat);
			//
			((UIDropMenu*) Element)->Caption = StrToWStr (DataStack[i].Func["caption"][0].Str);
			((UIDropMenu*) Element)->Menu = (UIListMenu*) Elements[const_cast<char*>(DataStack[i].Func["menu"][0].Str.c_str())];

			if (DataStack[i].Func.count("showtext") != 0)
				((UIDropMenu*) Element)->ShowText = (bool) DataStack[i].Func["showtext"][0].Num;
		}

		if (Element == 0)
			continue;
		//
		if (DataStack[i].Func.count("pos") != 0) {
			Element->Left = DataStack[i].Func["pos"][0].Num;
			Element->Top  = DataStack[i].Func["pos"][1].Num;
		}
		//
		if (DataStack[i].Func.count("size") != 0) {
			//

			if (DataStack[i].Func["size"][0].Str == "auto") {
				//
				if (dynamic_cast <UIButton*> (Element)) {
					// Draw Text
					float bounds[6];
					string Text = wstring2string(((UIButton*)Element)->Caption);
					ftglGetFontBBox(Manager->Theme->Font, Text.c_str(), ((UIButton*)Element)->Caption.size(), bounds);
					Element->Width = bounds[3]-bounds[0]+12;
					((UIButton*)Element)->TextAlign = alLeft;
				}
			} else {
				//
				//
				Element->FixSize = true;
				Element->Width  = DataStack[i].Func["size"][0].Num;
				Element->Height = DataStack[i].Func["size"][1].Num;
			}
		}

		if (DataStack[i].Func.count("visible") != 0) Element->Visible = (bool) DataStack[i].Func["visible"][0].Num;
		if (DataStack[i].Func.count("enabled") != 0) Element->Enabled = (bool) DataStack[i].Func["enabled"][0].Num;
		//
		if (DataStack[i].Func.count("align") != 0){
			//
			if (DataStack[i].Func["align"][0].Str == "top")    Element->Align = alTop;
			if (DataStack[i].Func["align"][0].Str == "bottom") Element->Align = alBottom;
			if (DataStack[i].Func["align"][0].Str == "left")   Element->Align = alLeft;
			if (DataStack[i].Func["align"][0].Str == "right")  Element->Align = alRight;
			if (DataStack[i].Func["align"][0].Str == "center") Element->Align = alCenter;
		}
		//
		if (DataStack[i].Func.count("icon") != 0) {
			Element->IconOffset[0] = DataStack[i].Func["icon"][0].Num-1;
			Element->IconOffset[1] = DataStack[i].Func["icon"][1].Num-1;
			Element->ShowIcon = true;
		}
		//
		if (DataStack[i].Func.count("icon2") != 0) {
			Element->Icon2Offset[0] = DataStack[i].Func["icon2"][0].Num-1;
			Element->Icon2Offset[1] = DataStack[i].Func["icon2"][1].Num-1;
			Element->ShowIcon2 = true;
		}
		//
		if (DataStack[i].Func.count("clickicon") != 0) {
			Element->ClickIconOffset[0] = DataStack[i].Func["clickicon"][0].Num-1;
			Element->ClickIconOffset[1] = DataStack[i].Func["clickicon"][1].Num-1;
			Element->ShowClickIcon = true;
		}

		if (DataStack[i].Func.count ("transparent") != 0)
				Element->Transparent = (bool) DataStack[i].Func["transparent"][0].Num;
		//
		if (Element == 0) continue;
		if (DataStack[i].Func["parent"][0].Str != "root" && DataStack[i].Func["parent"][0].Str != "none") {
			Element->Parent = Elements[const_cast<char*>(DataStack[i].Func["parent"][0].Str.c_str())];
			Element->inDialog = Element->Parent->inDialog;
			//
			if (dynamic_cast<UIPanel*>(Element->Parent)) {
				//
				if (dynamic_cast<UIDialog*>(Element->Parent))
					Element->inDialog = true;
				//
				((UIPanel*) Element->Parent)->AddElement (Element);
			}
			//
			if (dynamic_cast<UITabs*>(Element->Parent))
				((UITabs*) Element->Parent)->AddTab ((UITab*) Element);

			if (dynamic_cast<UITab*>(Element->Parent))
				((UITab*) Element->Parent)->AddElement (Element);
			//
			if (dynamic_cast<UIGroupedElements*>(Element->Parent))
				((UIGroupedElements*) Element->Parent)->AddElement (Element);
			//
		} else if (DataStack[i].Func["parent"][0].Str == "root") {
			Manager->AddUIElement (Element);
		} 
		//
		Element->Theme = Manager->Theme;
		Element->Shader = Manager->Shader;
		Element->ColorShader = Manager->ColorShader;
		Element->Focused = false;
		Element->Manager = Manager;
		Element->Name = DataStack[i].Name;
		//
		Elements[const_cast<char*>(DataStack[i].Name.c_str())] = Element;
	}
	//
}
//
void UIData::LoadFromFile (const char *fn) {
	DataMap::LoadFromFile(fn);
	FillData();
}

void UIData::LoadFromBytecode (const char *fn) {
	DataMap::LoadFromBytecode(fn);
	FillData();
}

#endif
