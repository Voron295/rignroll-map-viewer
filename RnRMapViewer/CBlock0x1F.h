#ifndef CBLOCK0X1F_CLASS_H
#define CBLOCK0X1F_CLASS_H

#include "CSimpleBlock.h"

class CBlock0x1F : public CSimpleBlock
{
public:
	int m_dwTransparency;
	int m_dwAlignType;
	CVector4 field_38;
	int count;
	float *m_pBlendValues;
	int *m_pMasksId;
	CVector m_pPosition;

	CBlock0x1F(const char *name);
	~CBlock0x1F();

	virtual int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
};

#endif