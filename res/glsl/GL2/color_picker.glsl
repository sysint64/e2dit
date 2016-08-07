#vertex shader

uniform mat4 MVP;
varying vec3 Pos;

void main() {
	gl_Position = MVP*gl_Vertex;
	Pos = gl_Vertex;
}

#fragment shader

varying vec3 Pos;
uniform vec3 Color;
uniform vec3 HSB;
uniform vec3 RGB;
uniform vec3 Lab;
uniform int  Pallete;

void HSB2RGB (vec3 iHSB, inout vec3 ioRGB) {
	float H  = iHSB[0];
	float S  = iHSB[1];
	float B  = iHSB[2];
	float Hi = H/60.f;
	float Bmin = ((100.f-S)*B)/100.f;
	float a = (B-Bmin)*(mod(H, 60.f)/60.f);
	float Binc = Bmin+a;
	float Bdec = B-a;

	B /= 100.f; Binc /= 100.f; Bmin /= 100.f; Bdec /= 100.f;

	switch (int(Hi)) {
		case 0 : ioRGB = vec3 (B   , Binc, Bmin); break;
		case 1 : ioRGB = vec3 (Bdec, B   , Bmin); break;
		case 2 : ioRGB = vec3 (Bmin, B   , Binc); break;
		case 3 : ioRGB = vec3 (Bmin, Bdec, B   ); break;
		case 4 : ioRGB = vec3 (Binc, Bmin, B   ); break;
		case 5 : ioRGB = vec3 (B   , Bmin, Bdec); break;
	}
}

// HSB
void HSB_HParam() {
	vec3 c;
	HSB2RGB(vec3(HSB[0], Pos.x*100.f, Pos.y*100.f), c);
	gl_FragColor = vec4 (c, 1);
}

void HSB_SParam() {
	vec3 c;
	HSB2RGB(vec3(Pos.x*360.f, HSB[1], Pos.y*100.f), c);
	gl_FragColor = vec4 (c, 1);
}

void HSB_BParam() {
	vec3 c;
	HSB2RGB(vec3(Pos.x*360.f, Pos.y*100.f, HSB[2]), c);
	gl_FragColor = vec4 (c, 1);
}

// RGB
void RGB_RParam() { gl_FragColor = vec4 (vec3(RGB[0], Pos.y , Pos.x ), 1); }
void RGB_GParam() { gl_FragColor = vec4 (vec3(Pos.y , RGB[1], Pos.x ), 1); }
void RGB_BParam() { gl_FragColor = vec4 (vec3(Pos.x , Pos.y , RGB[2]), 1); }

// Lab
void Lab_LParam() {
}

void Lab_aParam() {
}

void Lab_bParam() {
}

void main() {
	switch (Pallete) {
		// HSB
		case 0 : HSB_HParam(); break;
		case 1 : HSB_SParam(); break;
		case 2 : HSB_BParam(); break;

		// RGB
		case 3 : RGB_RParam(); break;
		case 4 : RGB_GParam(); break;
		case 5 : RGB_BParam(); break;

		// Lab
		case 6 : Lab_LParam(); break;
		case 7 : Lab_aParam(); break;
		case 8 : Lab_bParam(); break;
	}
}
