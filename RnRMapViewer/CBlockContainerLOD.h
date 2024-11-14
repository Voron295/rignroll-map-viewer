#ifndef CBLOCKCONTAINER0X9_CLASS_H
#define CBLOCKCONTAINER0X9_CLASS_H

#include "CBlockContainer.h"

class CBlockContainer0x9Sub
{
public:
	int type;
	int count;
	void *data;

	int ReadFromFile(FILE *f);
};


class CBlockContainerLOD : public CBlockContainer
{
public:
	CBlockContainer0x9Sub data;

	CBlockContainerLOD(const char *name);
	~CBlockContainerLOD();

	virtual int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);

	virtual void Render();
};

#endif