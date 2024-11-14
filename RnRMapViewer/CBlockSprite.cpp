#include "CBlockSprite.h"

CBlockSprite::CBlockSprite(const char *name)
:CSimpleBlock(name, 12)
{
	field_30 = 0;
	m_dwAlignType = 0;
}

CBlockSprite::~CBlockSprite()
{

}

int CBlockSprite::ReadFromFile(FILE *f, void *data1, void *data2)
{
	GameModule::ReadVector4FromFile(f, &m_vPos);
	int flag = GameModule::ReadIntFromFile(f);
	if (flag & 1)
		field_30 = 0;
	else
		field_30 = 1;
	m_dwAlignType = flag >> 1;
	m_dwMaterialId = GameModule::ReadIntFromFile(f);
	return 1;
}

void CBlockSprite::WriteToFile(FILE *f)
{

}