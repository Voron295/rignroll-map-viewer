#ifndef CBLOCKCONTAINER0X1D_CLASS_H
#define CBLOCKCONTAINER0X1D_CLASS_H

#include "CBlockContainer.h"

class CBlockContainer0x1D : public CBlockContainer
{
public:
	int field_38;
	int field_3C;
	CVector4 field_40;
	float *field_50;

	CBlockContainer0x1D(const char *name);
	~CBlockContainer0x1D();

	virtual int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
};

#endif