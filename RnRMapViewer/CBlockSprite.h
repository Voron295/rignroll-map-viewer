#ifndef CBLOCKSPRITE_CLASS_H
#define CBLOCKSPRITE_CLASS_H

#include "CSimpleBlock.h"

class CBlockSprite : public CSimpleBlock
{
public:
	int field_30;
	int m_dwAlignType;
	CVector m_pPosition;
	int m_dwMaterialId;

	CBlockSprite(const char *name);
	~CBlockSprite();

	virtual int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
};

#endif