#ifndef CPP_FINDER
#define CPP_FINDER 1

//
void Finder::UpdateList() {
	//
	// Directories
	vector<string> dirs = vector<string>();
	fs::getDirs(Path.c_str(), dirs);
	FileList->Clear();
	//
	for (int i = 0; i < dirs.size(); i++) {
		UIFlatButton *Item = new UIFlatButton (false);
		Item->Caption = StrToWStr(dirs[i]);
		Item->ShowIcon = true;
		Item->OnMouseDown = OnFileClick;
		Item->OnDblClick = OpenFileEvent;
		//
		Item->IconOffset[0] = 0;
		Item->IconOffset[1] = 5;
		//
		FileList->AddItem (Item);
	}
	//
	// Files
	vector<string> files = vector<string>();
	fs::getFiles(Path.c_str(), files);
	//
	for (int i = 0; i < files.size(); i++) {
		int FileExt = fs::getExt(files[i]);

		switch (Filter) {
			case ftLevel   : if (FileExt != e_lvl)     continue; break;
			case ftProject : if (FileExt != e_prj)     continue; break;
			case ftAudio   : if (FileExt != e_music)   continue; break;
			case ftVideo   : if (FileExt != e_video)   continue; break;
			case ftPicture : if (FileExt != e_picture) continue; break;
			case ftScript  : if (FileExt != e_script)  continue; break;
		}

		UIFlatButton *Item = new UIFlatButton (false);
		Item->Caption = StrToWStr(files[i]);
		Item->ShowIcon = true;
		Item->OnMouseDown = OnFileClick;
		Item->OnDblClick = OpenFileEvent;
		//
		switch (FileExt) {
			case e_video	: Item->IconOffset[0] = 4; Item->IconOffset[1] = 5; break;
			case e_music	: Item->IconOffset[0] = 6; Item->IconOffset[1] = 5; break;
			case e_text		: Item->IconOffset[0] = 8; Item->IconOffset[1] = 5; break;
			case e_script	: Item->IconOffset[0] = 5; Item->IconOffset[1] = 5; break;
			case e_picture	: Item->IconOffset[0] = 3; Item->IconOffset[1] = 5; break;
			case e_font		: Item->IconOffset[0] = 7; Item->IconOffset[1] = 5; break;
			case e_tmx		: Item->IconOffset[0] = 9; Item->IconOffset[1] = 8; break;
			case e_lvl		: Item->IconOffset[0] = 2; Item->IconOffset[1] = 5; break;
			case e_prj		: Item->IconOffset[0] = 8; Item->IconOffset[1] = 8; break;

			//
			default:
				Item->IconOffset[0] = 1;
				Item->IconOffset[1] = 5;
		}

		FileList->AddItem (Item);
	}
	//
	FileList->Init();
}

// Events
void Finder::OnFileClick (UIElement *Sender, int x, int y, int shift) {
	//
	if (!finder->Dialog->isOpened) return;

	finder->FileName = wstring2string (((UIFlatButton*) Sender)->Caption);
	finder->NameField->wText = ((UIFlatButton*) Sender)->Caption;
	//
	if (fs::getExt(finder->FileName) == e_picture) {
		Textures::PreviewFloat->LoadFromFile((finder->Path+DirSlash+"\0"+finder->FileName).c_str());
		Textures::PreviewFloat->Update();

		//finder->PreviewImage->Texture = Textures::PreviewFloat;

		if (Textures::PreviewFloat->Width > 240 || Textures::PreviewFloat->Height > 240) {
			float ratio = (float) Textures::PreviewFloat->Height / (float) Textures::PreviewFloat->Width;

			if (ratio == 1.0f) {
				finder->PreviewImage->Width  = 240;
				finder->PreviewImage->Height = 240;
			} else if (ratio < 1.0f) {
				finder->PreviewImage->Width  = 240;
				finder->PreviewImage->Height = 240*ratio;
			} else {
				finder->PreviewImage->Height = 240;
				finder->PreviewImage->Width  = 240/ratio;
			}
		} else {
			finder->PreviewImage->Width  = Textures::PreviewFloat->Width;
			finder->PreviewImage->Height = Textures::PreviewFloat->Height;
		}

		finder->PreviewImage->Visible = true;
		finder->PreviewGrid ->Visible = true;
	} else {
		//finder->PreviewImage->Texture = Textures::Preview;
		finder->PreviewImage->Width   = 240;
		finder->PreviewImage->Height  = 240;
		finder->PreviewImage->Visible = false;
		finder->PreviewGrid ->Visible = false;
	}

	finder->PlacesList->Deselect();
	finder->DriveList ->Deselect();
	finder->Step();
}

void Finder::OnCancelClick (UIElement *Sender) {
	finder->Dialog->Close();
	finder->SendOK = false;
}

void Finder::OnOKClick (UIElement *Sender) {
	finder->Dialog->Close();
	finder->FileName = fs::setSlashes(finder->FileName);

	if (finder->CallBackFunc != 0)
		finder->CallBackFunc();
	//addSpriteForm->Open();
	//finder->SendOK = true;
}

void Finder::OnDriveClick (UIElement *Sender) {
	int n = Sender->Id;
	finder->Path = finder->drives[n].path;
	finder->History.push_back(finder->Path);
	finder->HistoryPos++;

	finder->PrevFolderButton->Enabled = true;
	finder->UpFolderButton  ->Enabled = false;
	finder->PlacesList->Deselect();

	finder->UpdateList();
}

void Finder::OnSpecialDirClick (UIElement *Sender) {
	int n = Sender->Id;
	finder->Path = finder->specialDirs[n];
	finder->History.push_back(finder->Path);
	finder->HistoryPos++;

	finder->PrevFolderButton->Enabled = true;
	finder->UpFolderButton  ->Enabled = false;
	finder->DriveList->Deselect();

	finder->UpdateList();
	//specialDirs
}

// Navigation Events ------------------------------------>

void Finder::OnUpdateClick (UIElement *Sender) {
	finder->UpdateList();
}

void Finder::OnUpFolderClick (UIElement *Sender) {
	finder->Path = fs::upFolder(finder->Path);
	finder->NameField->wText = StrToWStr(finder->Path);
	finder->UpdateList();

	//if (finder->Path == fs::upFolder(finder->Path))
	//	finder->UpFolderButton->Enabled = false;
}

void Finder::OnPrevFolderClick (UIElement *Sender) {
	if (finder->HistoryPos != 0) {
		finder->HistoryPos--;
		finder->Path = finder->History[finder->HistoryPos];
		finder->UpdateList();
		finder->NextFolderButton->Enabled = true;
	}

	if (finder->HistoryPos == 0)
		finder->PrevFolderButton->Enabled = false;
}

void Finder::OnNextFolderClick (UIElement *Sender) {
	if (finder->HistoryPos != finder->History.size()-1) {
		finder->HistoryPos++;
		finder->Path = finder->History[finder->HistoryPos];
		finder->UpdateList();
		finder->PrevFolderButton->Enabled = true;
	}

	if (finder->HistoryPos == finder->History.size()-1)
		finder->NextFolderButton->Enabled = false;
}

// <------------------------------------ Navigation Events

//
void Finder::UpdateDrives() {
	//
	// Mounted Drives
	drives.clear();
	//
	fs::getMountedDrives (drives);
	int dlHeight = 35;
	//
	for (int i = 0; i < drives.size(); i++) {
		UIFlatButton *Item = new UIFlatButton (false);
		Item->Caption = StrToWStr(drives[i].name);
		Item->ShowIcon = true;
		Item->IconOffset[0] = 9;
		Item->IconOffset[1] = 5;
		Item->OnClick = OnDriveClick;
		DriveList->AddItem (Item);
		dlHeight += 22;
	}
	//

	Data->Elements["pdevices"]->Height = dlHeight;
}
//

//
void Finder::Init() {
	FileList	   = (UIListMenu*) Data->Elements["lmfiles"];
	PlacesList	   = (UIListMenu*) Data->Elements["lmplaces"];
	DriveList	   = (UIListMenu*) Data->Elements["lmdrives"];
	Dialog		   =   (UIDialog*) Data->Elements["finderdialog"];
	OKButton	   =   (UIButton*) Data->Elements["bok"];
	PreviewImage   =    (UIImage*) Data->Elements["ipreview"];
	PreviewPanel   =    (UIPanel*) Data->Elements["ppreview"];
	PreviewBorder  =    (UIImage*) Data->Elements["ipreviewborder"];
	PreviewGrid    =    (UIImage*) Data->Elements["igrid"];
	NameField	   =     (UIEdit*) Data->Elements["efilename"];

	// Navigation buttons
	NextFolderButton = (UIButton*) Data->Elements["bnext"];
	PrevFolderButton = (UIButton*) Data->Elements["bprev"];
	UpFolderButton   = (UIButton*) Data->Elements["bup"];
	//
	NextFolderButton->Enabled = false;
	PrevFolderButton->Enabled = false;

	Data->Elements["bupd"] ->OnClick = OnUpdateClick;
	Data->Elements["bup"]  ->OnClick = OnUpFolderClick;
	Data->Elements["bprev"]->OnClick = OnPrevFolderClick;
	Data->Elements["bnext"]->OnClick = OnNextFolderClick;
	//
	Data->Elements["bcancel"]->OnClick = OnCancelClick;
	OKButton->OnClick = OnOKClick;
	Path = ExeDir;

	// Filter
	Data->Elements["bftge"]      ->OnClick = SetProjectFilter;
	Data->Elements["bftsound"]   ->OnClick = SetAudioFilter;
	Data->Elements["bftvideo"]   ->OnClick = SetVideoFilter;
	Data->Elements["bftpicture"] ->OnClick = SetPictureFilter;
	Data->Elements["bftgescript"]->OnClick = SetScriptFilter;
	//
	int n = 0;

	string sdir;

	if (!fs::getRecentDir(&sdir)) PlacesList->Items[0]->Visible = false;
	specialDirs.push_back(sdir);

	if (!fs::getHomeDir(&sdir)) PlacesList->Items[1]->Visible = false;
	specialDirs.push_back(sdir);

	if (!fs::getDesktopDir(&sdir)) PlacesList->Items[2]->Visible = false;
	specialDirs.push_back(sdir);

	if (!fs::getVideosDir(&sdir)) PlacesList->Items[3]->Visible = false;
	specialDirs.push_back(sdir);

	if (!fs::getDocumentsDir(&sdir)) PlacesList->Items[4]->Visible = false;
	specialDirs.push_back(sdir);

	if (!fs::getDownloadsDir(&sdir)) PlacesList->Items[5]->Visible = false;
	specialDirs.push_back(sdir);

	if (!fs::getPicturesDir(&sdir)) PlacesList->Items[6]->Visible = false;
	specialDirs.push_back(sdir);

	if (!fs::getMusicsDir(&sdir)) PlacesList->Items[7]->Visible = false;
	specialDirs.push_back(sdir);
	//
	for (int i = 0; i < PlacesList->Items.size(); i++) {
		if (!PlacesList->Items[i]->Visible) continue;
		PlacesList->Items[i]->OnClick = OnSpecialDirClick;
	}
	//
	for (int i = 0; i < OKButton->Caption.size(); i++) {
		//
		if (OKButton->Caption[i] == L'|') {
			n++;
			continue;
		}
		//
		DialogNames[n] += OKButton->Caption[i];
	}
	//
	OKButton->Caption = L"";
	//
	Update();
}

void Finder::Step() {
	PreviewImage->Top   = (PreviewPanel->Height >> 1) - (PreviewImage->Height >> 1);
	PreviewImage->Left  = (PreviewPanel->Width  >> 1) - (PreviewImage->Width  >> 1);
	//
	PreviewBorder->Top  = (PreviewPanel->Height >> 1) - (PreviewBorder->Height >> 1);
	PreviewBorder->Left = (PreviewPanel->Width  >> 1) - (PreviewBorder->Width  >> 1);
	//
	PreviewGrid->Top    = PreviewBorder->Top;
	PreviewGrid->Left   = PreviewBorder->Left;
}

void Finder::Open (const int type,  VoidEvent callback) {
	CallBackFunc = callback;
	History.clear();
	History.push_back(ExeDir);
	HistoryPos = 0;

	//Path = ExeDir;
	Dialog->Open();
	Type = type;
	//
	// Set Caption -->
	switch (type) {
		case ftSaveLevel	  : Dialog->Caption = DialogNames[0]; OKButton->Caption = DialogNames[0]; Filter = ftLevel;   break;
		case ftSaveProject 	  : Dialog->Caption = DialogNames[1]; OKButton->Caption = DialogNames[1]; Filter = ftProject; break;
		case ftOpenLevel	  : Dialog->Caption = DialogNames[2]; OKButton->Caption = DialogNames[2]; Filter = ftLevel;   break;
		case ftOpenProject	  : Dialog->Caption = DialogNames[3]; OKButton->Caption = DialogNames[3]; Filter = ftProject; break;
		case ftOpenAudio	  : Dialog->Caption = DialogNames[4]; OKButton->Caption = DialogNames[4]; Filter = ftAudio;   break;
		case ftOpenVideo	  : Dialog->Caption = DialogNames[5]; OKButton->Caption = DialogNames[5]; Filter = ftVideo;   break;
		case ftOpenPicture	  : Dialog->Caption = DialogNames[6]; OKButton->Caption = DialogNames[6]; Filter = ftPicture; break;
		case ftOpenScript	  : Dialog->Caption = DialogNames[7]; OKButton->Caption = DialogNames[7]; Filter = ftScript;  break;
		case ftSaveScript	  : Dialog->Caption = DialogNames[8]; OKButton->Caption = DialogNames[8]; Filter = ftScript;  break;
	}

	//
	for (int i = 0; i < ((UIGroupedElements*) Data->Elements["gbft"])->Elements.size(); i++)
		((UIGroupedElements*) Data->Elements["gbft"])->Elements[i]->Checked = false;

	// <--
	switch (Filter) {
		case ftProject : Data->Elements["bftge"]      ->Checked = true; break;
		case ftAudio   : Data->Elements["bftsound"]   ->Checked = true; break;
		case ftVideo   : Data->Elements["bftvideo"]   ->Checked = true; break;
		case ftPicture : Data->Elements["bftpicture"] ->Checked = true; break;
		case ftScript  : Data->Elements["bftgescript"]->Checked = true; break;
	}
	//
	UpdateList();
	SendOK = false;
}

void Finder::OpenFileEvent (UIElement *Sender, int x, int y, int shift) {
	string path = finder->Path+"/"+finder->FileName;
	
	if (access (finder->Path.c_str(), X_OK) != 0)
		return;
	//
	struct stat buf;
	stat (path.c_str(), &buf);
	//
	if ((buf.st_mode & S_IFMT) == S_IFDIR) {
		finder->History.push_back(path);
		finder->HistoryPos++;
		finder->Path = path;
		finder->UpdateList();
		finder->PrevFolderButton->Enabled = true;
		finder->UpFolderButton  ->Enabled = true;
	} else OnOKClick (0);
}

//
void Finder::KeyPressed (Uint16 key) {
	if (!Dialog->isOpened) return;
	if (key == keyEscape) OnCancelClick (0);
	if (key != keyEnter) return;
	//
	OpenFileEvent (NULL, 0, 0, 0);
}

// Filter
void Finder::SetLevelFilter   (UIElement *Sender) { finder->Filter = ftLevel;   finder->UpdateList(); }
void Finder::SetProjectFilter (UIElement *Sender) { finder->Filter = ftProject; finder->UpdateList(); }
void Finder::SetVideoFilter   (UIElement *Sender) { finder->Filter = ftVideo;   finder->UpdateList(); }
void Finder::SetAudioFilter   (UIElement *Sender) { finder->Filter = ftAudio;   finder->UpdateList(); }
void Finder::SetPictureFilter (UIElement *Sender) { finder->Filter = ftPicture; finder->UpdateList(); }
void Finder::SetScriptFilter  (UIElement *Sender) { finder->Filter = ftScript;  finder->UpdateList(); }

#endif
