#ifndef MASK_CLASS_H
#define MASK_CLASS_H

#include <d3dx9.h>
#include <iostream>
#include "RGB.h"
#include <iostream>

enum MaskType
{
	MASK_8BIT,
	MASK_16BIT
};

class Mask
{
public:
	std::string m_sName;
	void *m_pData;
	DWORD m_dwDataSize;
	RGB m_pPalette[256];
	unsigned short m_wWidth;
	unsigned short m_wHeight;
	MaskType m_eInternalType;
	LPDIRECT3DTEXTURE9 m_pSurface;

	Mask();
	~Mask();
	bool ReadFromFile(FILE *f);
};

#endif