#ifndef CBLOCKANMOBJ_CLASS_H
#define CBLOCKANMOBJ_CLASS_H

#include "CBlockSpace.h"

class CBlockAnmObj : public CSimpleBlock
{
public:
	int field_30;
	int field_34;
	float field_38;
	float field_3C;
	CSimpleBlock *m_pBlock;
	CBlockSpace *m_pSpace;
	int field_48;
	int field_4C;
	float field_50;
	float field_54;

	CBlockAnmObj(const char *name);
	~CBlockAnmObj();

	int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
	virtual void Render();
};

#endif