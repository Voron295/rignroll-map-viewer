
#include "RGB.h"

//RGB565 -> RGB888 using tables
BYTE Table5[] = { 0, 8, 16, 25, 33, 41, 49, 58, 66, 74, 82, 90, 99, 107, 115, 123, 132,
140, 148, 156, 165, 173, 181, 189, 197, 206, 214, 222, 230, 239, 247, 255 };

BYTE Table6[] = { 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 45, 49, 53, 57, 61, 65, 69,
73, 77, 81, 85, 89, 93, 97, 101, 105, 109, 113, 117, 121, 125, 130, 134, 138,
142, 146, 150, 154, 158, 162, 166, 170, 174, 178, 182, 186, 190, 194, 198,
202, 206, 210, 215, 219, 223, 227, 231, 235, 239, 243, 247, 251, 255 };

void GetRGBFrom565(WORD color, BYTE &r, BYTE &g, BYTE &b)
{
	b = (color & 0xF800) >> 11;
	g = (color & 0x7E0) >> 5;
	r = (color & 0x1F);
	/*
	r = r * 0xFF / 0x1F;
	g = g * 0xFF / 0x3F;
	b = b * 0xFF / 0x1F;*/

	r = Table5[r];
	g = Table6[g];
	b = Table5[b];
}


void GetRGBAFrom555(WORD color, BYTE &r, BYTE &g, BYTE &b, BYTE &a)
{
	a = (color & 0x8000) >> 15;
	r = (color & 0x7C00) >> 10;
	g = (color & 0x3E0) >> 5;
	b = (color & 0x1F);
	a *= 255;
	r = Table5[r];
	g = Table5[g];
	b = Table5[b];
}

WORD Get565FromRGB(BYTE r, BYTE g, BYTE b)
{
	r = r * 0x1F / 0xFF;
	g = g * 0x3F / 0xFF;
	b = b * 0x1F / 0xFF;
	return (r << 11) | (g << 5) | b;
}

WORD Get565From555(WORD color)
{
	BYTE r, g, b;
	r = (color & 0x7C00) >> 10;
	g = (color & 0x3E0) >> 5;
	b = (color & 0x1F);
	b = b * 255 / 31;
	g = g * 255 / 31;
	r = r * 255 / 31;
	return Get565FromRGB(r, g, b);
}

DWORD GetRGBAFrom4444(WORD color)
{
	BYTE out[4];
	out[0] = ((color & 0xF000) >> 12) * 17;
	out[3] = ((color & 0xF00) >> 8) * 17;
	out[2] = ((color & 0xF0) >> 4) * 17;
	out[1] = (color & 0xF) * 17;
	return *(DWORD *)&out;
}

void GetRGBAFrom4444(WORD color, BYTE &r, BYTE &g, BYTE &b, BYTE &a)
{
	a = ((color & 0xF000) >> 12) * 17;
	r = ((color & 0xF00) >> 8) * 17;
	g = ((color & 0xF0) >> 4) * 17;
	b = (color & 0xF) * 17;
}