#ifndef CBLOCKCONTAINER0X1A_CLASS_H
#define CBLOCKCONTAINER0X1A_CLASS_H

#include "CBlockContainer.h"

class CBlockContainer0x1A : public CBlockContainer
{
public:
	CVector field_38;
	float field_44;

	CBlockContainer0x1A(const char *name);
	~CBlockContainer0x1A();

	virtual int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
};

#endif