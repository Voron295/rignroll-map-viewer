#ifndef CBLOCKREF_CLASS_H
#define CBLOCKREF_CLASS_H

#include "CBlockSpace.h"

class CBlockRef : public CBlockContainer
{
public:
	int someCount;
	CBlockSpace *field_3C;

	CBlockRef(const char *name);
	~CBlockRef();

	virtual int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
};

#endif