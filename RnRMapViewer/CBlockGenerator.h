#ifndef CBLOCKGENERATOR_CLASS_H
#define CBLOCKGENERATOR_CLASS_H

#include "CBlock0xD.h"

class CBlockGenerator : public CSimpleBlock
{
public:
	int(__cdecl *callback)(DWORD, DWORD, DWORD);
	int field_34;
	int field_38;
	CBlock0xDSub field_3C;

	CBlockGenerator(const char *name);
	~CBlockGenerator();

	virtual int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
};

#endif