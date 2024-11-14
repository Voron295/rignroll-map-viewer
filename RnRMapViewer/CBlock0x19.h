#ifndef CBLOCK0X19_CLASS_H
#define CBLOCK0X19_CLASS_H

#include "CSimpleBlock.h"

class CBlock0x19 : public CSimpleBlock
{
public:
	int field_30;
	int field_34;
	int field_38;
	float field_3C;
	float field_40;
	float field_44;
	float field_48;
	float field_4C;
	int field_50;
	CVector field_54;
	CVector field_60;

	CBlock0x19(const char *name);
	~CBlock0x19();

	virtual int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
};

#endif