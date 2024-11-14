#ifndef CBLOCK0XD_CLASS_H
#define CBLOCK0XD_CLASS_H

#include "CBlockContainerLOD.h"

class CBlock0xDSub
{
public:
	int flags;
	int field_4;
	int count;
	int *data;

	int ReadFromFile(FILE *f);
	void WriteToFile(FILE *f);
};


struct CBlock0xD : public CSimpleBlock
{
public:
	CBlock0xDSub field_30;
	CBlockContainer0x9Sub data;

	CBlock0xD(const char *name);
	~CBlock0xD();

	int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
};



#endif