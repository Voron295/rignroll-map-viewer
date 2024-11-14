#ifndef RGB_CLASS_H
#define RGB_CLASS_H

#include <Windows.h>
struct RGB
{
	BYTE R;
	BYTE G;
	BYTE B;
};

struct RGBA
{
	BYTE R;
	BYTE G;
	BYTE B;
	BYTE A;
};

void GetRGBFrom565(WORD color, BYTE &r, BYTE &g, BYTE &b);
void GetRGBAFrom555(WORD color, BYTE &r, BYTE &g, BYTE &b, BYTE &a);
WORD Get565FromRGB(BYTE r, BYTE g, BYTE b);
WORD Get565From555(WORD color);
DWORD GetRGBAFrom4444(WORD color);
void GetRGBAFrom4444(WORD color, BYTE &r, BYTE &g, BYTE &b, BYTE &a);

#endif