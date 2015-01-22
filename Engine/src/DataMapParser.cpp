//* Author : Kabilin Andrey, 2013 *//

#ifndef CPP_DATAMAP_PARSER
#define CPP_DATAMAP_PARSER

// Lexer

//===----------------------------------------------------------------------===//
// Lexer - Lexical Analizer
//===----------------------------------------------------------------------===//

// GetChar - Return next char from stream
char DataMap::GetChar() {
	static int buf;

	if (feof(InFile)) { return EOF; }
	
	size_t a = fread(&buf, sizeof(char), 1, InFile);
	fOffsetStack[fOffsetStack.size()-1]++;
	posno++;

	if (buf == '\r') {
		size_t a = fread(&buf, sizeof(char), 1, InFile);
		fOffsetStack[fOffsetStack.size()-1]++;
		
		if (buf == '\n') {
			lineno++;
			posno = 1;
		}
	} else if (buf == '\n') {
		lineno++;
		posno = 1;
	}
	
	return buf;
}

void DataMap::OpenFile (const char *fn) {
	fclose(InFile);
	InFile = fopen(fn, "r");
	fOffsetStack.push_back(0);
}

void DataMap::ReOpenFile (const char *fn) {
	fclose(InFile);
	InFile = fopen(fn, "r");
	fOffsetStack.pop_back();
	fseek (InFile, fOffsetStack[fOffsetStack.size()-1], SEEK_CUR);
	//char *buf = new char[fOffsetStack[fOffsetStack.size()-1]+1];
	//char *buf[1000];
	//fread(&buf, sizeof(char), fOffsetStack[fOffsetStack.size()-1], InFile);
	//delete buf;
}

//
char DataMap::GetNextToken() {
	CurToken = GetToken();
	return CurToken;
}

// GetToken - Return next token from stream
char DataMap::GetToken() {
	//static int LastChar = ' ';
	
	// Skip space
	while (isspace(LastChar))
		LastChar = GetChar();

	if (feof(InFile)) {
		return tok_eof; }

	char ThisChar2 = LastChar;
	bool isWideStr;

	/*if (LastChar == 'L') {
		ThisChar2 = GetChar();

		if (ThisChar2 == '"') {
			LastChar = ThisChar2;
			isWideStr = true;
		}
	}*/

	// Символ | Строка
	if (LastChar == '\"') {
		//StrLen = 0;
		string StrVal;
		IdentifierStr = "";
		int State = 0;
		//
		while (State != 2) {			
			switch (State) {
				case 0 :
					do {
						LastChar = GetChar();
						if (LastChar != '\"') {
							IdentifierStr += LastChar;
							//StrLen++;
						}
					} while (LastChar != '\"' && LastChar != EOF);
		
					if (LastChar == EOF) {
						LOG_ERROR("%s:%d:%d: Parse error", FileName.c_str(), lineno, posno);
						Terminate(EXIT_FAILURE);
					} else LastChar = GetChar();

					if (LastChar == '#')  { State = 1; break; }
					if (LastChar == '\"') { IdentifierStr += '\"'; State = 0; break; }

					State = 2;
					break;

				case 1 :
					//StrLen++;
					string NumStr = "";
					LastChar = GetChar();

					while (isdigit(LastChar)) {
						NumStr += LastChar;//
						LastChar = GetChar();
					} //

					//LastChar = GetChar();
					int c = strtod(NumStr.c_str(), 0);
					char buff[3];

					IdentifierStr += buff;

					if (LastChar == '\"') { State = 0; break; }
					if (LastChar == '#')  { State = 1; break; }

					State = 2;
					break;
			} // switch
		} // while

		return tok_string;
	}
	
	// Identifier: [a-zA-Z_][a-zA-Z0-9_]*
	if (isalpha(LastChar) || LastChar == '_') {
		IdentifierStr = LastChar;
		
		while (1) {
			LastChar = GetChar();
			
			if (isdigit(LastChar) || isalpha(LastChar) || LastChar == '_') {
				IdentifierStr += LastChar;
				continue;
			}
			
			break;
		}
		
		IdentifierStr = LowerCaseS (IdentifierStr);
		
		// Words
		if (IdentifierStr == "begin")		return tok_begin;
		if (IdentifierStr == "end")			return tok_end;
		if (IdentifierStr == "include")		return tok_include;
		if (IdentifierStr == "false")		{ NumVal = 0; return tok_number; }
		if (IdentifierStr == "true")		{ NumVal = 1; return tok_number; }
		
		// Простой идентификатор
		return tok_id;
	}
	
	// Число Float | Integer: [0-9] (.[0-9]+)? (E[+-]? [0-9]+)?
	if (isdigit(LastChar) || LastChar == '-') {
		int stat = 2;
		
		bool neg = false;
		string NumStr;
		string ScaleStr;
		int ScaleFactor = 0;
		
		//
		while (stat < 8) { // Строим конечный автормат
			switch (stat) {
				case 2 :
					NumStr += LastChar;
					LastChar = GetChar();
						
					if (isdigit(LastChar)) {
						stat = 2;
					} else if (LastChar == '.') {
						stat = 3;
					} else if (LastChar == 'E' || LastChar == 'e') {
						stat = 5;
					} else stat = 9; // Завершающее состояние
					
					break;
					
				case 3 :
					NumStr += LastChar;
					LastChar = GetChar();
					
					if (isdigit(LastChar)) {
						stat = 4;
					} else stat = 9; // Ошибка
					
					break;
					
				case 4 :
					NumStr += LastChar;
					LastChar = GetChar();
						
					if (isdigit(LastChar)) {
						stat = 4;
					} else if (LastChar == 'E' || LastChar == 'e') {
						stat = 5;
					} else stat = 10; // Завершающее состояние
					
					break;
					
				case 5 :
					LastChar = GetChar();
					
					if (LastChar == '+' || LastChar == '-') {
						stat = 6;
					} else if (isdigit(LastChar)) {
						stat = 7;
					} else ; // Ошибка
					
					break;
					
				case 6 :
					ScaleStr += LastChar;
					LastChar = GetChar();
					
					if (isdigit(LastChar)) {
						stat = 7;
					} else ; // Ошибка
						
					break;
					
				case 7 :
					ScaleStr += LastChar;
					LastChar = GetChar();
					
					if (isdigit(LastChar)) {
						stat = 7;
					} else stat = 8; // Завершающее состояние
					
					break;
					
				default : ;
			}
		} // Конечный Автомат
			
		NumVal = strtod (NumStr.c_str(), 0);
		
		if (ScaleStr != "") ScaleFactor = strtod(ScaleStr.c_str(), 0);
		
		NumVal = NumVal*pow(10, ScaleFactor);
		if (neg) NumVal = -NumVal;
		//
		return tok_number;
	}
	
	// End of File
	//if (LastChar == EOF)
	//	return tok_eof;
	
	// Comment
	if (LastChar == '/') {
		int ThisChar = LastChar;
		LastChar = GetChar();

		if (LastChar == '/') { // Delete comment
			while (LastChar != EOF && LastChar != '\n' && LastChar != '\r')
				LastChar = GetChar();
			
			if (LastChar != EOF)
				return GetToken();
		}
		
		return ThisChar;
	}
	
	// Comment
	if (LastChar == '{') { // Delete comment
		LastChar = GetChar();
		
		while (LastChar != EOF && LastChar != '}') {
			LastChar = GetChar();
		}
		
		LastChar = GetChar();
		
		if (LastChar != EOF)
			return GetToken();
	}
	
	// Else, return as ASCII
	int ThisChar = LastChar;
	LastChar = GetChar();
	
	return ThisChar;
}


//===----------------------------------------------------------------------===//
// Parser - Sintax Analizer
//===----------------------------------------------------------------------===//

// Main Parse
void DataMap::Parse() {
	GetNextToken();
	
	while (1) {
		switch (CurToken) {
			case tok_eof	:  bytecode.push_back(op_eof); return;
			case ';'	: GetNextToken(); break;
			case tok_include :  ParseInclude(); break;
			case tok_id	: ParseElement(); break;
			default		:
				//SetCurLine();
				//Error<OperAST*>(expected_expr);
				LOG_ERROR("%s:%d:%d: Parse error", FileName.c_str(), lineno, posno);
				Terminate(EXIT_FAILURE);
				return;
				GetNextToken();
		}
	}
}

void DataMap::ParseInclude() {
	GetNextToken();

	if (CurToken != tok_string) {
		LOG_ERROR("%s:%d:%d: Parse error", FileName.c_str(), lineno, posno);
		Terminate(EXIT_FAILURE);
	}

	string FN = IdentifierStr;
	FN = fs::extractFileDir(FileName)+FN;
	LOG_DEBUG("%s\n", FN.c_str());
	GetNextToken();

	if (CurToken != ';') {
		LOG_ERROR("%s:%d:%d: Parse error", FileName.c_str(), lineno, posno);
		Terminate(EXIT_FAILURE);
	}

	string LastFN = FileName;
	FileName = FN;

	int lastPos  = posno;
	int lastLine = lineno;

	OpenFile(FileName.c_str());
	posno = 1; lineno = 1;

	Parse();

	FileName = LastFN;
	LOG_DEBUG("%s\n", FileName.c_str());
	//Terminate(0);

	ReOpenFile(FileName.c_str());
	LOG_DEBUG("!!!\n");
	int Tok = GetToken();

	Parse();
	//
	posno  = lastPos;
	lineno = lastLine;
}

// PARAM ::= NUM | STR
struct ThemeVal DataMap::ParseParam() {
	struct ThemeVal Val;
	union {
		float Num;
		char  Bytes[4];
	} NumByte;
	
	if (CurToken == tok_string || CurToken == tok_id) {
		Val.isStr = true;
		Val.Str = IdentifierStr;
		//
		bytecode.push_back (op_str);
		bytecode.push_back (IdentifierStr.size());
		//
		for (int i = 0; i < IdentifierStr.size(); i++)
			bytecode.push_back(IdentifierStr[i]);
		//
	} else {
		Val.isStr = false;
		Val.Num = NumVal;
		//
		NumByte.Num = NumVal;
		//
		bytecode.push_back (op_num);
		bytecode.push_back (NumByte.Bytes[0]);
		bytecode.push_back (NumByte.Bytes[1]);
		bytecode.push_back (NumByte.Bytes[2]);
		bytecode.push_back (NumByte.Bytes[3]);
	}
	
	GetNextToken();
	return Val;
}

// PARAMS ::= PARAM | PARAM PARAMS
vector<ThemeVal> DataMap::ParseParams(string Name) {
	vector<ThemeVal> Params;
	//
	while ((CurToken != ';' & CurToken != tok_eof) == 1) {
		struct ThemeVal Param = ParseParam();
		Params.push_back(Param);
		
		if (CurToken == ';') break;
		if (CurToken != ',') {
			LOG_ERROR("%s:%d:%d: Parse error", FileName.c_str(), lineno, posno);
			Terminate(EXIT_FAILURE);

			GetNextToken();
		}
		
		GetNextToken();
	}
	
	return Params;
}

// FUNC ::= ID '(' PARAMS ')'
void DataMap::ParseFunc (string Name) {
	string FuncName = IdentifierStr;

	bytecode.push_back (op_def);
	bytecode.push_back (FuncName.size());
	//
	for (int i = 0; i < FuncName.size(); i++)
		bytecode.push_back(FuncName[i]);

	GetNextToken();

	//
	if (CurToken != ':') {
		LOG_ERROR("%s:%d:%d: Parse error", FileName.c_str(), lineno, posno);
		Terminate(EXIT_FAILURE);
		//
		GetNextToken();
	}
	
	GetNextToken();
	vector<ThemeVal> Params = ParseParams(FuncName);
	
	//

	if (CurToken != ';') {
		LOG_ERROR("%s:%d:%d: Parse error", FileName.c_str(), lineno, posno);
		Terminate(EXIT_FAILURE);
		//
		GetNextToken();
	}

	bytecode.push_back (op_end);
	GetNextToken();
	
	// Fill

	if (FillType == Map) {
		Data[Name].Func[FuncName] = Params;
		Data[Name].Def = true;
	}  else {
		//
		for (int i = 0; i < DataStack.size(); i++) {
			//
			if (DataStack[i].Name == Name) {
				DataStack[i].Func[FuncName] = Params;
				return;
			}
		}
		//
	}
	//
}

// FUNCS ::= FUNC | FUNC FUNCS
void DataMap::ParseFuncs(string Name) {
	//
	while ((CurToken != tok_end & CurToken != tok_eof) == 1) {
		ParseFunc(Name);
		if (CurToken == tok_end) break;
	}
}

// ELEMENT ::= ID BEGIN FUNCS END
void DataMap::ParseElement() {
	string Name = IdentifierStr;
	string Type = "";
	//
	bytecode.push_back (op_elem);
	bytecode.push_back (Name.size());
	//
	for (int i = 0; i < Name.size(); i++)
		bytecode.push_back(Name[i]);
	//
	//
	GetNextToken();

	if (CurToken == '(') {
		GetNextToken();
		Type = IdentifierStr;

		bytecode.push_back (op_type);
		bytecode.push_back (Type.size());
		//
		for (int i = 0; i < Type.size(); i++)
			bytecode.push_back(Type[i]);

		GetNextToken();
		//
		if (CurToken != ')') {
			LOG_ERROR("%s:%d:%d: Parse error", FileName.c_str(), lineno, posno);
			Terminate(EXIT_FAILURE);
			//
			GetNextToken();
		}

		GetNextToken();
	}

	//
	if (FillType == Vector) {
		struct ElData Element;
		Element.Name = Name;
		Element.Type = Type;
		DataStack.push_back(Element);
	}

	ParseFuncs(Name);
	
	//
	if (CurToken != tok_end) {
		LOG_ERROR("%s:%d:%d: Parse error", FileName.c_str(), lineno, posno);
		Terminate(EXIT_FAILURE);
		//
		GetNextToken();
	}

	GetNextToken();

	if (CurToken != ';') {
		LOG_ERROR("%s:%d:%d: Parse error", FileName.c_str(), lineno, posno);
		Terminate(EXIT_FAILURE);
		//
		GetNextToken();
	}

	bytecode.push_back (op_end);
	GetNextToken();
	
	// Fill
	
}

// ELEMENTS ::= ELEMENT | ELEMENT ELEMENTS
void DataMap::ParseElements() {
	//
	while (CurToken != tok_eof) {
		ParseElement();
	}
}

#endif
