#ifndef CBLOCK0X1C_CLASS_H
#define CBLOCK0X1C_CLASS_H

#include "CSimpleBlock.h"

class CBlock0x1CSub
{
public:
	WORD field_0;
	__int16 field_2;
	WORD field_4;
	__int16 field_6;
	float *field_8;
	float *field_C;
	float *field_10;
	float *field_14;
	int field_18;
	float field_1C;
	WORD field_20;
	__int16 field_22;
	int field_24;

	int ReadFromFile(FILE *f);
	void WriteToFile(FILE *f);
};


class CBlock0x1C : public CSimpleBlock
{
public:
	int count;
	CBlock0x1CSub *data;
	CVector field_38;

	CBlock0x1C(const char *name);
	~CBlock0x1C();

	virtual int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
};

#endif