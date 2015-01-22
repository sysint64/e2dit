#ifndef H_FINDER
#define H_FINDER 1

//
#define ftSaveLevel			0
#define ftSaveProject		1
#define ftOpenLevel			2
#define ftOpenProject		3
#define ftOpenAudio			4
#define ftOpenVideo			5
#define ftOpenPicture		6
#define ftOpenScript		7
#define ftSaveScript		8
//
#define ftLevel				0
#define ftProject			1
#define ftAudio				2
#define ftVideo				3
#define ftPicture			4
#define ftScript			5
//
class Finder {
private:
	UIListMenu *FileList;
	UIListMenu *DriveList;
	UIListMenu *PlacesList;
	UIButton   *OKButton;
	UIButton   *NextFolderButton;
	UIButton   *PrevFolderButton;
	UIButton   *UpFolderButton;
	UIImage	   *PreviewBorder;
	UIImage	   *PreviewImage;
	UIImage	   *PreviewGrid;
	UIPanel    *PreviewPanel;
	int			Filter;
	//
	vector<fs::drive> drives;
	vector<string> specialDirs;
	int Type;
	wstring DialogNames[9];
	//
	void UpdateList();
	void UpdateDrives();
	//
	static void OnFileClick       (UIElement *Sender, int x, int y, int shift);
	static void OnCancelClick     (UIElement *Sender);
	static void OnOKClick         (UIElement *Sender);
	static void OpenFileEvent     (UIElement *Sender, int x, int y, int shift);
	static void OnDriveClick      (UIElement *Sender);
	static void OnSpecialDirClick (UIElement *Sender);

	// Navigation
	static void OnUpdateClick     (UIElement *Sender);
	static void OnUpFolderClick   (UIElement *Sender);
	static void OnPrevFolderClick (UIElement *Sender);
	static void OnNextFolderClick (UIElement *Sender);

	// Filer
	static void SetLevelFilter    (UIElement *Sender);
	static void SetProjectFilter  (UIElement *Sender);
	static void SetVideoFilter    (UIElement *Sender);
	static void SetAudioFilter    (UIElement *Sender);
	static void SetPictureFilter  (UIElement *Sender);
	static void SetScriptFilter   (UIElement *Sender);
	//
public:
	UIEdit	 *NameField;
	UIDialog *Dialog;
	vector <string> History;
	int	HistoryPos;
	string FileName;
	string Path;
	UIData *Data;
	bool SendOK;
	VoidEvent CallBackFunc;
	//
	Finder() { Data = new UIData(); SendOK = false; CallBackFunc = 0; }
	~Finder() { delete Data; }
	//
	void KeyPressed (Uint16 key);
	void Update() { UpdateList(); UpdateDrives(); }
	void Init();
	void Step();
	void Open (const int type, VoidEvent callback = 0);
};

Finder *finder;
#endif
