#ifndef CBLOCKSPACE_CLASS_H
#define CBLOCKSPACE_CLASS_H

#include "CBlockContainer.h"
#include "CMatrix.h"

class CBlockSpace : public CBlockContainer
{
public:
	int field_38;
	int field_3C;
	CMatrix m_pMatrix;

	CBlockSpace(const char *name);
	~CBlockSpace();

	virtual int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
};

#endif