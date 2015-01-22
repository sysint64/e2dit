#ifndef H_FS
#define H_FS

namespace fs {
//
#define e_unknown	0
#define e_video		1
#define e_music		2
#define e_text		3
#define e_script	4
#define e_picture	5
#define e_font		6
#define e_tmx		7
#define e_lvl		8
#define e_prj		9
//
#define d_drive		0
#define d_disc		1
//
//
struct drive {
	string path;
	string name;
	int	   type;
} typedef drive;

string extractFileName (const string &fn) {
	//
	if (fn == "/" || fn == "\\") return fn;
	string str;
	//
	for (int i = fn.size()-1; i >= 0; i--) {
		if (fn[i] == '/' || fn[i] == '\\') break;
		str += fn[i];
	}

	reverse (str.begin(), str.end());
	return str;
}

string extractFileExt (const string &fn) {
	//
	string str;
	//
	for (int i = fn.size()-1; i >= 0; i--) {
		if (fn[i] == '.') break;
		str += fn[i];
	}

	reverse (str.begin(), str.end());
	return str;
}

string extractFileDir (const string &fn) {
	//
	if (fn == "/" || fn == "\\") return fn;
	string str;
	int pos = 0;
	//
	for (int i = fn.size()-1; i >= 0; i--) {
		if (fn[i] == '/' || fn[i] == '\\') {
			pos = i;
			break;
		}
	}

	for (int i = pos; i >= 0; i--) {
		str += fn[i];
	}

	reverse (str.begin(), str.end());
	return str;
}

string upFolder (const string &fn) {
	string str;
	bool upped = 0;
	int start = fn.size()-1;

	if (fn[start] == '/' || fn[start] == '\\')
		start--;
	//
	for (int i = start; i >= 1; i--) {
		if (upped < 1)
			if (fn[i] == '/' || fn[i] == '\\') {
				upped++;
			}

		if (upped >= 1) str += fn[i-1];
	}

	reverse (str.begin(), str.end());

	if (upped) return str;
	else return fn;
}

bool exists (const string &fn) {
	if (access(fn.c_str(), X_OK) != 0)
		return false;

	return true;
}

//
int getExt (const string &fn) {
	//
	string ext;
	for (int i = fn.size()-1; i >= 0; i--) {
		if (fn[i] == '.') break;
		ext += fn[i];
	}

	reverse (ext.begin(), ext.end());
	ext = LowerCaseS (ext);

	// EXTS
	if (ext == "cpp")	return e_text;
	if (ext == "h") 	return e_text;
	if (ext == "txt") 	return e_text;
	if (ext == "jpg") 	return e_picture;
	if (ext == "png") 	return e_picture;
	//if (ext == "dds") 	return e_picture;
	if (ext == "avi") 	return e_video;
	if (ext == "mpg") 	return e_video;
	if (ext == "mpeg") 	return e_video;
	if (ext == "wmv") 	return e_video;
	if (ext == "mp3") 	return e_music;
	if (ext == "ogg") 	return e_music;
	if (ext == "wav") 	return e_music;
	if (ext == "ttf") 	return e_font;
	if (ext == "otf") 	return e_font;
	if (ext == "tmx") 	return e_tmx;

	//
	if (ext == "gh") 	return e_script;
	if (ext == "gs") 	return e_script;
	if (ext == "glv") 	return e_lvl;
	if (ext == "gprj") 	return e_prj;

	//
	return e_unknown;
}

#ifdef _linux_
	
	inline string setSlashes (string fn) {
		for (int i = 0; i < fn.size(); i++)
			if (fn[i] == '\\') fn[i] = '/';

		return fn;
	}

	void AddTextToClipboard (string text) {
		//

	}

	string GetTextFromClipboard() {
		return "";
	}

	inline string getExeDir() {
		char buffer[PATH_MAX];
		size_t len = sizeof(buffer);
		char *path_end;

		if (readlink ("/proc/self/exe", buffer, len) <= 0)
			return ".";
		
		path_end = strrchr (buffer, '/');
		
		if (path_end == NULL)
			return ".";
		
		//++path_end;
		*path_end = '\0';
		
		return string(buffer);
	}

	// Get Special Directories ----------------------->
	inline bool getRecentDir (string *res) {


		return false;
	}

	inline bool getHomeDir (string *res) {


		return false;
	}

	inline bool getDesktopDir (string *res) {


		return false;
	}

	inline bool getVideosDir (string *res) {


		return false;
	}

	inline bool getDocumentsDir (string *res) {


		return false;
	}

	inline bool getDownloadsDir (string *res) {

		return false;
	}

	inline bool getPicturesDir (string *res) {

		return false;
	}

	inline bool getMusicsDir (string *res) {

		return false;
	}

	// <----------------------- Get Special Directories


	// File system
	inline int getDirs (string dir, vector<string> &dirs) {
		DIR *dp = opendir(dir.c_str());
		struct dirent *dirp;
		//
		while ((dirp = readdir(dp)) != NULL) {
			if (dirp->d_type == DT_DIR && string(dirp->d_name) != "." && string(dirp->d_name) != "..")
			dirs.push_back(string(dirp->d_name));
		}

		closedir(dp);
		sort (dirs.begin(), dirs.end());
		return 0;
	}

	//
	inline int getFiles (string dir, vector<string> &files) {
		DIR *dp = opendir(dir.c_str());
		struct dirent *dirp;
		//
		while ((dirp = readdir(dp)) != NULL) {
			if (dirp->d_type != DT_DIR)
				files.push_back(string(dirp->d_name));
		}

		closedir(dp);
		sort (files.begin(), files.end());
		return 0;
	}

	// Mounted Drives

	void getMountedDrives (vector<struct drive> &drives) {
		//
		FILE *mounts = setmntent("/etc/mtab", "r");
		struct mntent *ent;
		//
		while ((ent = getmntent(mounts)) != NULL){
			//
			struct drive dr;
			dr.path = ent->mnt_dir;
			dr.name = extractFileName(ent->mnt_dir);
			dr.type = d_drive;
			//
			if (strcmp(ent->mnt_type, "ext4") == 0)
				drives.push_back(dr);

			if (strcmp(ent->mnt_type, "ext3") == 0)
				drives.push_back(dr);

			if (strcmp(ent->mnt_type, "fuseblk") == 0)
				drives.push_back(dr);
		} 
		//
		endmntent (mounts);
	}
#endif

#ifdef _win_
	#include "shlobj.h"

	inline string setSlashes (string fn) {
		for (int i = 0; i < fn.size(); i++)
			if (fn[i] == '/') fn[i] = '\\';

		return fn;
	}

	void AddTextToClipboard (string text) {
		//
		if (OpenClipboard(0)) {
		    HGLOBAL clipbuffer;
		    char * buffer;
		    EmptyClipboard();
		    clipbuffer = GlobalAlloc(GMEM_DDESHARE, text.size()+1);
		    buffer = (char*) GlobalLock(clipbuffer);
		    strcpy(buffer, text.c_str());
		    GlobalUnlock(clipbuffer);
		    SetClipboardData(CF_TEXT,clipbuffer);
		    CloseClipboard();
		}
	}

	string GetTextFromClipboard() {
		char * buffer;
		string res = "";

		if (OpenClipboard(0)) {
		    buffer = (char*) GetClipboardData(CF_TEXT);
		    res = string(buffer);
		}

		CloseClipboard(); 
		return res;
	}

	inline string getExeDir() {
		char dir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, &dir[0]);
		return string(dir);
	}

	// Get Special Directories ----------------------->
	inline bool getRecentDir (string *res) {
		char path[MAX_PATH];

		/*if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_RECENT, NULL, 0, &path[0]))) {
			*res = string(path);
			return true;
		}*/

		return false;
	}

	inline bool getHomeDir (string *res) {
		char path[MAX_PATH];

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, &path[0]))) {
			*res = string(path);
			return true;
		}

		return false;
	}

	inline bool getDesktopDir (string *res) {
		char path[MAX_PATH];

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, &path[0]))) {
			*res = string(path);
			return true;
		}

		return false;
	}

	inline bool getVideosDir (string *res) {
		char path[MAX_PATH];

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYVIDEO, NULL, 0, &path[0]))) {
			*res = string(path);
			return true;
		}

		return false;
	}

	inline bool getDocumentsDir (string *res) {
		char path[MAX_PATH];

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, &path[0]))) {
			*res = string(path);
			return true;
		}

		return false;
	}

	inline bool getDownloadsDir (string *res) {
		char path[MAX_PATH];

		/*if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, 0, &path[0]))) {
			*res = string(path);
			return true;
		}*/

		return false;
	}

	inline bool getPicturesDir (string *res) {
		char path[MAX_PATH];

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYPICTURES, NULL, 0, &path[0]))) {
			*res = string(path);
			return true;
		}

		return false;
	}

	inline bool getMusicsDir (string *res) {
		char path[MAX_PATH];

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYMUSIC, NULL, 0, &path[0]))) {
			*res = string(path);
			return true;
		}

		return false;
	}

	// <----------------------- Get Special Directories

	// File system
	inline int getDirs (string dir, vector<string> &dirs) {
		WIN32_FIND_DATA f;
		HANDLE h = FindFirstFile((dir+"\*").c_str(), &f);

		if(h != INVALID_HANDLE_VALUE) {
			do {
				struct stat buf;
				stat ((dir+"\\"+f.cFileName).c_str(), &buf);

				if ((buf.st_mode & S_IFMT) == S_IFDIR && string(f.cFileName) != "." && string(f.cFileName) != "..")
					dirs.push_back(string(f.cFileName));
			} while(FindNextFile(h, &f));
		} else {
			return -1;
		}

		return 0;
	}

	//
	inline int getFiles (string dir, vector<string> &files) {
		WIN32_FIND_DATA f;
		HANDLE h = FindFirstFile((dir+"\\*").c_str(), &f);

		if (h != INVALID_HANDLE_VALUE) {
			do {
				struct stat buf;
				stat ((dir+"\\"+f.cFileName).c_str(), &buf);

				if ((buf.st_mode & S_IFMT) != S_IFDIR)
					files.push_back(string(f.cFileName));
			} while(FindNextFile(h, &f));
		} else {
			return -1;
		}

		return 0;
	}

	// Mounted Drives

	void getMountedDrives (vector<struct drive> &drives) {
		bool Flag;
		DWORD drivesMask = GetLogicalDrives();

		for (int i = 0; i < 26; i++) {
			if (!((drivesMask >> i) & 1)) continue;

			char letter = 65+i;
			char dl[3] = {letter, ':', '\0'};
			char path[MAX_PATH] = {letter, ':', '\\', '\0'};

			if (!exists(path)) continue;
			UINT drive_type = GetDriveType(path);
			if (drive_type != DRIVE_FIXED) continue;

            char drive_label[30];
            char drive_fat[30];
            DWORD drive_sn;
            DWORD drive_name_size = sizeof(drive_label);

            Flag = GetVolumeInformation(path, drive_label, sizeof(drive_label), &drive_sn, &drive_name_size, NULL, drive_fat, sizeof(drive_fat));
            if (!Flag) continue;

			struct drive dr;
			dr.path = string(path);
			dr.name = "("+string(dl)+") "+drive_label;
			dr.type = d_drive;
			drives.push_back(dr);
		}
	}

#endif

}

#endif
