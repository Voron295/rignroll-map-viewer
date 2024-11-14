#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H
#include <iostream>
#include <d3dx9.h>

IDirect3DTexture9 *CreateDirect3DTextureFromMemory(IDirect3DDevice9 *device, int width, int height, RGBA *memory);

class Texture
{
public:
	std::string m_sFullName;
	std::string m_sName;
	int m_iWidth;
	int m_iHeight;
	BYTE m_bDepth;
	BYTE m_bFormat;
	void *m_pData;
	DWORD m_dwDataSize;
	bool m_bLoaded;

	LPDIRECT3DTEXTURE9 m_pSurface;

	struct {
		BYTE r;
		BYTE g;
		BYTE b;
	} m_pPalette[256];

	Texture();
	~Texture();

	void ReadFromFile(FILE *f);
	void InitDirect3D();
};

#endif