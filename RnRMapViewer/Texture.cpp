#include "GameModule.h"
#include "RGB.h"

extern IDirect3DDevice9 *g_pDirect3DDevice;

IDirect3DTexture9 *CreateDirect3DTextureFromMemory(IDirect3DDevice9 *device, int width, int height, RGBA *memory)
{
	IDirect3DTexture9 *texture;
	D3DLOCKED_RECT lockedRect;
	HRESULT hr = device->CreateTexture(width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture, 0);
	//logFile->WriteLine("Tex Create Err(%d, %d, %d): 0x%X", width, height, depth, hr);
	if (!SUCCEEDED(hr))
		return 0;

	hr = texture->LockRect(0, &lockedRect, 0, 0);
	if (!SUCCEEDED(hr))
	{
		texture->Release();
		return 0;
	}
	for (int i = 0; i < width * height; i++)
	{
		*((BYTE*)lockedRect.pBits + i * 4) = memory[i].B;
		*((BYTE*)lockedRect.pBits + i * 4 + 1) = memory[i].G;
		*((BYTE*)lockedRect.pBits + i * 4 + 2) = memory[i].R;
		*((BYTE*)lockedRect.pBits + i * 4 + 3) = memory[i].A;
	}
	texture->UnlockRect(0);

	return texture;
}

Texture::Texture()
{
	m_pSurface = 0;
}

Texture::~Texture()
{
	if (m_pData)
		free(m_pData);
}

char *GetFileNameFromPath(const char *path, char *buf)
{
	for (int i = strlen(path) - 1; i >= 0; i--)
	{
		if (path[i] == '.')
		{
			int j = i;
			for (j = i; j >= 0; j--)
			{
				if (path[j] == '/' || path[j] == '\\')
					break;
			}
			strncpy(buf, &path[j + 1], i - j - 1);
			buf[i - j - 1] = 0;
			break;
		}
	}
	return buf;
}

void Texture::ReadFromFile(FILE *f)
{
	int nop, offset, start, blockOffset;
	char paletteOffset = 0, flag1 = 0, flag2 = 0;
	char hasColor = 0, bitColor = 0;
	WORD paletteCount = 0;
	char sBuf[128];

	m_iWidth = 0;
	m_iHeight = 0;
	m_bDepth = 0;

	m_sFullName = GameModule::ReadStringFromFile(f);
	m_sName = GetFileNameFromPath(m_sFullName.c_str(), sBuf);
	m_dwDataSize = GameModule::ReadIntFromFile(f);

	start = ftell(f);

	fread(&paletteOffset, 1, 1, f);
	fread(&hasColor, 1, 1, f);
	fread(&flag2, 1, 1, f);
	fread(&nop, 1, 2, f);
	fread(&paletteCount, 2, 1, f);
	fread(&bitColor, 1, 1, f);

	fread(&nop, 4, 1, f);

	fread(&m_iWidth, 2, 1, f);
	fread(&m_iHeight, 2, 1, f);
	fread(&m_bDepth, 1, 1, f);

	fread(&flag1, 1, 1, f);

	int endOfHeader = ftell(f);
	int loffStart = 0;
	if (paletteOffset <= 0)
	{
		fseek(f, paletteOffset + endOfHeader, SEEK_SET);
		if (hasColor)
		{
			for (int i = 0; i < paletteCount; i++)
			{
				fread(&m_pPalette[i].b, 1, 1, f);
				fread(&m_pPalette[i].g, 1, 1, f);
				fread(&m_pPalette[i].r, 1, 1, f);
			}
		}


		m_pData = malloc(m_iWidth*m_iHeight);

		if (flag2 == 1)
		{
			for (int i = 0; i < m_iHeight; i++)
			{
				int j = flag1 & 0x20 ? i : m_iHeight - i - 1;
				fread(&((char*)m_pData)[m_iWidth * j], m_iWidth, 1, f);
			}
		}
	}
	else
	{
		fread(&nop, 4, 1, f); // LOFF
		fread(&nop, 4, 1, f); // 4?
		fread(&blockOffset, 4, 1, f); // end of LOFF block
		loffStart = ftell(f);

		fseek(f, start + blockOffset, SEEK_SET); // Seek to other blocks in texture
		while (!feof(f))
		{
			char blockName[4];
			fread(&blockName, 1, 4, f);
			fread(&blockOffset, 4, 1, f); // end of block
			int blockStart = ftell(f);

			if (strncmp(blockName, "LVLS", 4) == 0)
			{
				break;
			}
			else if (strncmp(blockName, "LVLH", 4) == 0)
			{
				break;
			}
			else if (strncmp(blockName, "LEVL", 4) == 0)
			{
				break;
			}
			else if (strncmp(blockName, "LVMP", 4) == 0)
			{
				blockOffset += 2;
			}
			else if (strncmp(blockName, "PFRM", 4) == 0)
			{
				int flag1, flag2, flag3, flag4, flag5, flag6, flag7;
				fread(&flag1, 4, 1, f);
				fread(&flag2, 4, 1, f);
				fread(&flag3, 4, 1, f);
				fread(&flag4, 4, 1, f);
				if (blockOffset > 0x10)
				{
					fread(&flag5, 4, 1, f);
					fread(&flag6, 4, 1, f);
					fread(&flag7, 4, 1, f);
				}

				if (flag3 == 0x1F)
				{
					if (flag2 == 0x3E0)
					{
						if (flag1 == 0x7C00 && !flag4)
							m_bFormat = 3;
					}
					else if (flag2 == 0x7E0 && flag1 == 0xF800 && !flag4)
						m_bFormat = 2;
				}
				else if (flag3 == 0xF)
				{
					if (flag2 == 0xF0 && flag1 == 0xF00 && flag4 == 0xF000)
					{
						m_bFormat = 5;
					}
				}
				else if (flag3 == 0xFF && flag2 == 0xFF00 && flag1 == 0xFF0000)
				{
					if (!flag4)
					{
						m_bFormat = 6;
					}
					else if (flag4 == 0xFF000000)
					{
						m_bFormat = 7;
					}
				}
				else if (flag5 == 0xFF00 && flag6 == 0xFF)
					m_bFormat = 8;
				else
					m_bFormat = 0;
			}
			else if (strncmp(blockName, "ENDR", 4) == 0)
			{
				break;
			}

			fseek(f, blockStart + blockOffset, SEEK_SET);
		}

		fseek(f, loffStart, SEEK_SET);
		//if(m_bFormat == 5)
		//m_pData = new BYTE[m_iWidth*m_iHeight*4];
		//else
		m_pData = malloc(m_iWidth*m_iHeight*(m_bDepth / 8));//new BYTE[m_iWidth*m_iHeight*(m_bDepth/8)];
		memset(m_pData, 0, m_iWidth*m_iHeight*(m_bDepth / 8));
		//fread(m_pData, 1, m_iWidth*m_iHeight*(m_bDepth/8), f);

		WORD data;
		for (int i = 0; i < m_iWidth*m_iHeight; i++)
		{
			fread(&data, 1, m_bDepth / 8, f);
			//if(m_bFormat == 5)
			//((DWORD *)m_pData)[i] = GetRGBAFrom4444(data);
			//else

			/*if (m_bFormat == 5)
			{
				BYTE a, r, g, b;
				a = (data & 0xF000) >> 12;
				b = (data & 0xF00) >> 8;
				g = (data & 0xF0) >> 4;
				r = (data & 0xF);

				data = (a << 12) | (r << 8) | (g << 4) | b;
			}
			else if (m_bFormat == 3)
			{
				data = Get565From555(data);
			}*/
			((WORD *)m_pData)[i] = data;
		}

	}

	fseek(f, start + m_dwDataSize, SEEK_SET);
}

void Texture::InitDirect3D()
{
	RGBA *data = (RGBA*)malloc(m_iWidth * m_iHeight * sizeof(RGBA));
	if (m_bDepth == 8)
	{
		for (int i = 0; i < m_iWidth * m_iHeight; i++)
		{
			data[i].R = m_pPalette[((char*)m_pData)[i]].r;
			data[i].G = m_pPalette[((char*)m_pData)[i]].g;
			data[i].B = m_pPalette[((char*)m_pData)[i]].b;
			if (((char*)m_pData)[i])
				data[i].A = 255;
			else
				data[i].A = 0;
		}
	}
	else
	{
		for (int i = 0; i < m_iWidth * m_iHeight; i++)
		{
			if (m_bFormat == 5) // 4444
			{
				GetRGBAFrom4444(((WORD*)m_pData)[i], data[i].R, data[i].G, data[i].B, data[i].A);
				//GetRGBFrom565(((WORD*)m_pData)[i], data[i].R, data[i].G, data[i].B);
			}
			else if (m_bFormat == 3) // 555
			{
				GetRGBAFrom555(((WORD*)m_pData)[i], data[i].R, data[i].G, data[i].B, data[i].A);
			}
			else
			{
				GetRGBFrom565(((WORD*)m_pData)[i], data[i].B, data[i].G, data[i].R);
			}
		}
	}
	m_pSurface = CreateDirect3DTextureFromMemory(g_pDirect3DDevice, m_iWidth, m_iHeight, data);
	free(data);
}