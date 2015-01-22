#ifndef H_UTILS
#define H_UTILS

#define Uint8 unsigned char

inline float min (float a, float b) {
	return a < b ? a : b;
}

inline float max (float a, float b) {
	return a > b ? a : b;
}

inline float min (float a, float b, float c) {
	float m = b;

	if (a < m) m = a;
	if (c < m) m = c;

	return m;
}

inline float max (float a, float b, float c) {
	float m = b;

	if (a > m) m = a;
	if (c > m) m = c;

	return m;
}

inline glm::vec2 min (glm::vec2 a, glm::vec2 b) {
	return glm::vec2(min (a.x, b.x), min (a.y, b.y));
}

inline glm::vec2 max (glm::vec2 a, glm::vec2 b) {
	return glm::vec2(max (a.x, b.x), max (a.y, b.y));
}

inline glm::vec2 rot2D (glm::vec2 a1, glm::vec2 a2, float angle) {

	float x = a1.x + (a2.x-a1.x) * cos (angle) + (a2.y-a1.y) * sin (angle);
	float y = a1.y - (a2.x-a1.x) * sin (angle) + (a2.y-a1.y) * cos (angle);

	return glm::vec2(x, y);
}

// Return true, if x and y in Rect (Xe,Ye,W,H)

inline bool PointInRect (int X, int Y, int Xe, int Ye, int W, int H) {
	//
	return ((X <= Xe+W) && (X >= Xe) &&
			(Y <= Ye+H) && (Y >= Ye));
}

inline bool PointInRect (glm::vec2 point, glm::vec2 *rect) {

	float minx = min (rect[0].x, rect[1].x); float miny = min (rect[0].y, rect[1].y);
	float maxx = max (rect[0].x, rect[1].x); float maxy = max (rect[0].y, rect[1].y);
	//
	return (point.x <= maxx && point.x >= minx &&
			point.y <= maxy && point.y >= miny);

}
//
static string IntToStr (int n) {
	//
	if (n == 0) return "0";
	string res;
	int v = n;
	int len = 0;
	//
	while (v != 0) {
		len++;
		v /= 10;
	}
	//
	res.resize (len);
	//
	while (len != 0) {
		len--;
		v = n % 10;
		res[len] = v+'0';
		n /= 10;
	}

	return res;
}

template <typename T>
inline bool feq (T a, T b, float eps = numeric_limits<T>::epsilon()) {
	return  a >= b-eps &&
			a <= b+eps;
}


inline float deg2rad (float deg) {
	return pi*deg / 180.0f;
}

static wstring IntToWStr (int n) {
	//
	wchar_t m = L'\0';
	if (n < 0) {
		n *= -1;
		m = L'-';
	}
	if (n == 0) return L"0";
	wstring res;
	int v = n;
	int len = 0;
	//
	while (v != 0) {
		len++;
		v /= 10;
	}
	//
	res.resize (len);
	//
	while (len != 0) {
		len--;
		v = n % 10;
		res[len] = wchar_t(v+'0');
		n /= 10;
	}

	if (m != L'\0')
		res = m+res;
	
	return res;
}

inline bool isDigit (const char ch) {
	return ch >= '0' && ch <= '9';
}

inline bool isDigit (const wchar_t ch) {
	return ch >= L'0' && ch <= L'9';
}

static int StrToInt (const string str) {
	int res;
	int stp = 0;
	int end = 0;

	if (str[0] == '-') end = 1;
	for (int i = str.size()-1; i >= end; i--) {
		if (isDigit(str[i])) res += (str[i]-'0')*pow(10, stp);
		else return 0;

		stp++;
	}

	if (end == 1) res *= -1;
	return res;
}

static int WStrToInt (const wstring str) {
	int res = 0;
	int stp = 0;
	int end = 0;

	//
	if (str[0] == L'-') end = 1;
	for (int i = str.size()-1; i >= end; i--) {
		if (isDigit(str[i])) res += (str[i]-L'0')*pow (10, stp);
		else return 0;

		stp++;
	}

	if (end == 1) res *= -1;
	return res;
}

static string FloatToStr (double n) {
	char buff[13];

	n >= 0.f ? sprintf(buff, " %f", n) : sprintf(buff, "%f", n);

	return buff;
}

wstring StrToWStr (const string &s);
static wstring FloatToWStr (double n) {
	return StrToWStr(FloatToStr(n));
}

// Hex To Val
static int HexToInt (wstring val) {
	int Result = 0;

	for (int i = 0; i < val.size(); i++) {
		int Add = 0;
		wchar_t C = val[i];

		if (C >= L'0' && C <= L'9') Add = C-L'0';
		if (C >= L'A' && C <= L'F') Add = C-L'A'+10;
		if (C >= L'a' && C <= L'f') Add = C-L'a'+10;

		Add <<= 4*i;
		Result += Add;
	}
  
	return Result;
}

// Hex To Val
static wstring ByteToHex ( Uint8 val) {
	//val = 15;
	Uint8 hb1 = (val & 0xf0) >> 4;
	//Uint8 hb1 = (val << 1) & 0x0f;
	Uint8 hb2 = val & 0x0f;
	//
	wchar_t wc1, wc2;
	//
	if (hb1 >= 10) wc1 = L'a'+hb1-10;
	else wc1 = '0'+hb1;
	//
	if (hb2 >= 10) wc2 = L'a'+hb2-10;
	else wc2 = '0'+hb2;
	//
	wstring res;
	res += wc1;
	res += wc2;
	//
	return res;
}

wstring StrToWStr (const string &s) {
	//
	wstring ws(s.size(), L' ');
	ws.resize(mbstowcs(&ws[0], s.c_str(), s.size()));
	return ws;
}

inline void clamp (int *val, int min, int max) {
	if (*val <= min) *val = min;
	if (*val >= max) *val = max;
}

inline int clamp (int val, int min, int max) {
	if (val <= min) return min;
	if (val >= max) return max;
	return val;
}

inline void clamp (float *val, float min, float max) {
	if (*val <= min) *val = min;
	if (*val >= max) *val = max;
}

inline float clamp (float val, float min, float max) {
	if (val <= min) return min;
	if (val >= max) return max;
	return val;
}

//#define min (a, b) (((a) < (b)) ? (a):(b))
//#define max (a, b) (((a) > (b)) ? (a):(b))

/*inline int Max (int a, int b) {
	if (a > b) return a;
	else return b;
}

inline int Min (int a, int b) {
	if (a < b) return a;
	else return b;
}*/

// UTF Utility
string DecodeUTF8 (wchar_t uc) {
	string ret;
	Uint8 b1, b2, b3, b4;//, b5, b6;
	
	if (uc == 0x0040) return "";
	if ((uc >= 0xD800 & uc <= 0xDFFF) == 1) return "";
	
	//
	if ((uc > 0x1 & uc <= 0x007F) == 1) {
		ret += uc;
		//
	} else if ((uc > 0x007F & uc <= 0x07FF) == 1) {
		b1 = 0xC0 | (uc & 0x7C0) >> 6;
		b2 = 0x80 | (uc & 0x3F);
		//
		ret  = b1; ret += b2;
		//
	} else if ((uc > 0x07FF & uc <= 0xFFFF) == 1) {
		b1 = 0xE0 | (uc & 0xF000) >> 12;
		b2 = 0x80 | (uc & 0xFC0) >> 6;
		b3 = 0x80 | (uc & 0x3F);
		//
		ret  = b1; ret += b2; ret += b3;
		//
	} else if ((uc > 0xFFFF & uc <= 0x10FFFF) == 1) {
		b1 = 0xF0 | (uc & 0x1C00000) >> 18;
		b2 = 0x80 | (uc & 0x3F000) >> 12;
		b3 = 0x80 | (uc & 0xFC0) >> 6;
		b4 = 0x80 | (uc & 0x3F);
		//
		ret  = b1; ret += b2; ret += b3;
		ret += b4;
		//
	}/* else if ((uc > 0x1FFFFF & uc <= 0x3FFFFFF) == 1) {
		b1 = 0xF8 | (uc & 0x3000000) >> 24;
		b2 = 0x80 | (uc & 0xFC0000) >> 18;
		b3 = 0x80 | (uc & 0xF000) >> 12;
		b4 = 0x80 | (uc & 0xFC0) >> 6;
		b5 = 0x80 | (uc & 0x3F);
		//
		ret  = b1; ret += b2; ret += b3;
		ret += b4; ret += b5;
		//
	} else if ((uc > 0x3FFFFFF & uc <= 0x7FFFFFFF) == 1) {
		b1 = 0xFC | (uc & 0x40000000) >> 30;
		b2 = 0x80 | (uc & 0x3F000000) >> 24;
		b3 = 0x80 | (uc & 0xFC0000) >> 18;
		b4 = 0x80 | (uc & 0xF000) >> 12;
		b5 = 0x80 | (uc & 0xFC0) >> 6;
		b5 = 0x80 | (uc & 0x3F);
		//
		ret  = b1; ret += b2; ret += b3;
		ret += b4; ret += b5; ret += b6;
	}*/
	
	return ret;
}

// Перевод символа в нижний регистр
char LowerCase (const char c) {
	//
	if (c >= 'A' && c <= 'Z') {
		return c+32;
	} else return c;
}
	
// Перевод строки в нижний регистр
string LowerCaseS (const string str) {
	string s;
	
	for (int i = 0; i < str.length(); i++)
		s += LowerCase(str.c_str()[i]);
	
	return s;
}

string wstring2string (wstring ws) {
	string s;
	
	for (int i = 0; i < ws.size(); i++) {
		s += DecodeUTF8 (ws[i]);
	}
	
	return s;
}


inline void glBegin2D () {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, ScreenWidth, 0, ScreenHeight);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

inline void glEnd2D () {
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void DrawRect (int x, int y, int w, int h) {
	/*glBegin2D();
		glColor3f (1.0f, 0, 0);
		glBegin (GL_LINE_LOOP);
			glVertex2f (x, y);
			glVertex2f (x+w, y);
			glVertex2f (x+w, y+h);
			glVertex2f (x, y+h);
		glEnd();
	glEnd2D();*/
}

#endif
