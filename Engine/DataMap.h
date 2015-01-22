//* Author : Kabilin Andrey, 2013 *//

#ifndef H_DATAMAP
#define H_DATAMAP
//
static string FileName;
//
struct ThemeVal {
	float	Num;
	string	Str;
	wstring wStr;
	bool	isStr;
} typedef ThemeVal;

//
struct ThemeFuncs {
	map <string, vector<ThemeVal> > typedef DataType;
	DataType Func;
	bool Def;
} typedef ThemeFunc;

struct ElFunc {
	string Name;
	vector<ThemeVal> Params;
} typedef ElFunc;

struct ElThemeFuncs {
	//vector <ElFunc> Func;
	map <string, vector<ThemeVal> > Func;
} typedef ElThemeFuncs;

//
class mData {
public:
	map<string, struct ThemeFuncs> Data;
	//
	inline struct ThemeFuncs operator[](const string idx) {
		//
		if (!Data[idx].Def) {
			LOG_ERROR("%s: can't get function %s", FileName.c_str(), idx.c_str());
			Terminate(EXIT_FAILURE);
		}

		return Data[idx];
	}
};

struct ElData {
	string Name, Type;
	//vector <ElFunc> Funcs;
	map <string, vector<ThemeVal> > Func;
	//struct ElThemeFuncs;
};
//

// Tokens
enum Token {
	tok_eof = -1,
	//
	tok_id = -2, tok_number = -3, tok_string = -4, 
	tok_begin = -5, tok_end = -6, tok_include = -7
};

enum Operand {
	op_eof  = 0x00, op_elem = 0x01, op_def = 0x02,
	op_str  = 0x03, op_num  = 0x04, op_end = 0x05,
	op_type = 0x06,
};

//*
class DataMap {
private:
	FILE		*InFile;
	char		 CurToken;
	string		 IdentifierStr;
	float		 NumVal;
	vector<int> bytecode;
	vector<int>	fOffsetStack;
	int LastChar;
	//
	int posno, lineno; // Line & Pos
	
	// Lexer
	char GetChar();
	char GetToken();
	char GetNextToken();
	char GetStrLen();
	
	// Parser
	//
	void ParseElement();
	void ParseElements();
	vector<ThemeVal> ParseParams(string Name);
	struct ThemeVal ParseParam();
	void ParseFuncs(string Name);
	void ParseFunc(string Name);
	void ParseVal();
	void Parse();
	void ParseInclude();
	void OpenFile (const char *fn);
	void ReOpenFile (const char *fn);
	//
	char GetOp() {
		char byte;
		fread(&byte, sizeof(char), 1, InFile);
		return byte;
	}
	
public:
	//
	//mData Data;
	bool HaltIfErr;
	enum {Map, Vector} FillType;
	map<string, struct ThemeFuncs> typedef DataType;
	DataType Data;
	vector<ElData> DataStack;
	DataMap () { HaltIfErr = false; FillType = Map; LastChar = ' '; }
	//
	virtual void LoadFromFile     (const char *fn);
	virtual void LoadFromBytecode (const char *fn);
	void SaveToBytecode   (const char *fn);
	void SaveToFile		  (const char *fn);
	void Update();
};
//

#include "src/DataMapParser.cpp"
#include "src/DataMap.cpp"
#endif
