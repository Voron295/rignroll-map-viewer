#include "GameModule.h"

Palette::Palette()
{

}

Palette::~Palette()
{

}

bool Palette::ReadFromFile(FILE *f)
{
	m_sName = GameModule::ReadStringFromFile(f);
	int dataSize = GameModule::ReadIntFromFile(f);
	int curPos = ftell(f);
	fseek(f, 12, SEEK_CUR);

	for (int i = 0; i < 256; i++)
	{
		fread(&m_pPalette[i].R, 1, 1, f);
		fread(&m_pPalette[i].G, 1, 1, f);
		fread(&m_pPalette[i].B, 1, 1, f);
	}

	fseek(f, curPos + dataSize, SEEK_SET);
	return true;
}