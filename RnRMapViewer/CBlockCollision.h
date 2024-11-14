#ifndef CBLOCKCOLLISION_CLASS_H
#define CBLOCKCOLLISION_CLASS_H

#include "CSimpleBlock.h"

class CBlockCollision : public CSimpleBlock
{
public:
	int m_dwCount;
	CVector *m_pVertices;
	int *m_pIndices;

	CBlockCollision(const char *name);
	~CBlockCollision();

	virtual int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
	void InitPrimitiveData();
};

#endif