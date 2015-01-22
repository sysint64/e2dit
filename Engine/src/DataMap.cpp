//* Author : Kabilin Andrey, 2013 *//

#ifndef CPP_DATAMAP
#define CPP_DATAMAP

//
void DataMap::LoadFromFile (const char *fn) {
	lineno = 1; posno = 1;
	fOffsetStack.push_back(0);
	FileName = fn;
	InFile = fopen(fn, "r");
	Parse();
	fclose(InFile);
}

void DataMap::SaveToFile (const char *fn) {
	InFile = fopen(fn, "w+");
	
	for (DataType::iterator it = Data.begin(); it != Data.end(); it++) {
		fputs(it->first.c_str(), InFile); fputs("\n", InFile);

		struct ThemeFuncs tf = Data[it->first];

		for (ThemeFuncs::DataType::iterator itf = tf.Func.begin(); itf != tf.Func.end(); itf++) {
			fputs("    ", InFile); fputs(itf->first.c_str(), InFile); fputs(" : ", InFile);	

			for (int i = 0; i < tf.Func[itf->first].size(); i++) {
				if (tf.Func[itf->first][i].isStr) {
					char splitc;
					
					if (i < tf.Func[itf->first].size()-1) splitc = ',';
					else splitc = ';';

					fprintf (InFile, "\"%s\"%c ", tf.Func[itf->first][i].Str.c_str(), splitc);
				} else {
					char splitc;
					
					if (i < tf.Func[itf->first].size()-1) splitc = ',';
					else splitc = ';';

					fprintf (InFile, "%f%c ", tf.Func[itf->first][i].Num, splitc);
				}
			}

			fputs("\n", InFile);
		}

		fputs("end;\n\n", InFile);
	}

	fclose(InFile);	
}

//
void DataMap::LoadFromBytecode (const char *fn) {
	//
	InFile = fopen(fn, "r");
	char byte;
	//
	while (!feof(InFile)) {
		//
		byte = GetOp();
		string ElemName;
		string FuncName;
		string ElemType = "";
		int elemid;
		//
		if (byte != op_elem) break;

		byte = GetOp();
		//
		for (int i = 0; i < byte; i++)
			ElemName += GetOp();
		//
		byte = GetOp();
		//
		if (byte == op_type) {
			byte = GetOp();

			for (int i = 0; i < byte; i++)
				ElemType += GetOp();

			byte = GetOp();
		}

		if (FillType == Vector) {
			struct ElData Element;
			Element.Name = ElemName;
			Element.Type = ElemType;
			DataStack.push_back(Element);
			elemid = DataStack.size()-1;
		}
		//
		//
		while (byte == op_def) {
			byte = GetOp();

			for (int i = 0; i < byte; i++)
				FuncName += GetOp();
			//
			vector<ThemeVal> Params;
			byte = GetOp();

			while (byte != op_end) {
				struct ThemeVal Param;
				//
				switch (byte) {
					case op_str :
						byte = GetOp();
						Param.isStr = true;

						for (int i = 0; i < byte; i++)
							Param.Str += GetOp();

						break;

					case op_num :
						//
						union {
							float Num;
							char  Bytes[4];
						} NumByte; // Bytes to Float
						//
						NumByte.Bytes[0] = GetOp();
						NumByte.Bytes[1] = GetOp();
						NumByte.Bytes[2] = GetOp();
						NumByte.Bytes[3] = GetOp();
						//
						Param.Num = NumByte.Num;
						Param.isStr = false;
						//
						break;

					default :
						LOG_ERROR ("failed to open data file %s\n", fn);
						Terminate(EXIT_FAILURE);
						break;
				}
				//
				Params.push_back (Param);
				byte = GetOp();
			}

			if (FillType == Map) {
				Data[ElemName].Func[FuncName] = Params;
				Data[ElemName].Def = true;
			} else {
				DataStack[elemid].Func[FuncName] = Params;
			}

			FuncName = "";
			Params.clear();
			byte = GetOp();
		}
	}
	//
	fclose(InFile);
	//
}

//
void DataMap::SaveToBytecode (const char *fn) {
	//
	FILE *OutFile = fopen (fn, "w+");

	for (int i = 0; i < bytecode.size(); i++) {
		fputc(bytecode[i], OutFile);
	}

	fclose(OutFile);
	//
}

#endif
