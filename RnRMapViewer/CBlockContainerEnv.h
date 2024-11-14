#ifndef CBLOCKCONTAINERENV_CLASS_H
#define CBLOCKCONTAINERENV_CLASS_H

#include "CBlockContainer.h"

class CBlockContainerEnv : public CBlockContainer
{
public:
	double fogStart;
	double fogEnd;
	double field_48;
	double field_50;
	double field_58;
	int field_60;
	int field_64;

	CBlockContainerEnv(const char *name);
	~CBlockContainerEnv();

	virtual int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
};

#endif