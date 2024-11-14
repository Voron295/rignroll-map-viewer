#ifndef CBLOCKCONTAINER_CLASS_H
#define CBLOCKCONTAINER_CLASS_H

#include "CSimpleBlock.h"

class CBlockContainer : public CSimpleBlock
{
public:
	class CSimpleBlock **m_pSubBlocks;
	int m_dwSubBlocksCount;

	CBlockContainer(const char *name, int type, int childCount);

	virtual ~CBlockContainer();
	virtual int AddBlock(int id, class CSimpleBlock *block);

	int LoadChildren(FILE *f, int expectedCount, int realCount, void *data1, void *data2);
	void WriteChildren(FILE *f);
	virtual void ScanFunc(int(__cdecl *callback)(CSimpleBlock *));
	void RenderChildren(int id, int flags);
	virtual void Render();
};

#endif