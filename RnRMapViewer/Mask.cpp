#include "GameModule.h"

Mask::Mask()
{
	m_pData = 0;
	m_dwDataSize = 0;
	m_wWidth = 0;
	m_wHeight = 0;
	memset(m_pPalette, 0, 3 * 256);
}

Mask::~Mask()
{
	if (m_pData)
		free(m_pData);
}

bool Mask::ReadFromFile(FILE *f)
{
	m_sName = GameModule::ReadStringFromFile(f);
	m_dwDataSize = GameModule::ReadIntFromFile(f);

	DWORD startFile = ftell(f);

	char buf[5];
	fread(buf, 1, 4, f);
	buf[4] = 0;

	fread(&m_wWidth, 2, 1, f);
	fread(&m_wHeight, 2, 1, f);

	m_pData = malloc(3 * m_wWidth * m_wHeight);
	memset(m_pData, 0, 3 * m_wWidth * m_wHeight);

	for (int i = 0; i < 256; i++)
	{
		fread(&m_pPalette[i].R, 1, 1, f);
		fread(&m_pPalette[i].G, 1, 1, f);
		fread(&m_pPalette[i].B, 1, 1, f);
	}

	char *data = (char*)m_pData;
	if (!strcmp(buf, "MASK"))
	{
		m_eInternalType = MASK_8BIT;
		BYTE offset = 0;
		for (int i = 0; i < m_wHeight; i++)
		{
			for (int j = 0; j < m_wWidth; j += offset)
			{
				unsigned char id;
				fread(&id, 1, 1, f);

				//*(BYTE*)data = id;
				offset = id & 0x7F;

				if (id & 0x80)
				{
					memset(data, 0, offset * 3);
					data += offset * 3;
				}
				else
				{
					for (int k = 0; k < offset; k++)
					{
						fread(&id, 1, 1, f);
						//id = id & 0x7F;
						((RGB*)data)->R = m_pPalette[id].R;
						((RGB*)data)->G = m_pPalette[id].G;
						((RGB*)data)->B = m_pPalette[id].B;
						data += 3;
					}
				}
			}
		}
	}
	else if (!strcmp(buf, "MS16"))
	{
		m_eInternalType = MASK_16BIT;
		for (int i = 0; i < m_wWidth * m_wHeight; i++)
		{
			WORD buf;
			BYTE r, g, b;
			fread(&buf, 2, 1, f);
			GetRGBFrom565(buf, r, g, b);
			((RGB*)data)->R = r;
			((RGB*)data)->G = g;
			((RGB*)data)->B = b;
			data += 3;
		}
	}

	fseek(f, startFile + m_dwDataSize, SEEK_SET);
	return true;
}