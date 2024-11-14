#ifndef CSIMPLEBLOCK_CLASS_H
#define CSIMPLEBLOCK_CLASS_H

#include "Node.h"
#include "CVector4.h"
#include "GameModule.h"
//#include "D3DRenderData.h"

class CSimpleBlock : public Node
{
public:
	char *m_sName;
	int m_dwType;
	CVector4 m_vPos;
	GameModule *m_pModule;
	char flags;
	char field_29;
	char field_2A;
	char field_2B;
	class CBlockContainer *m_pParentBlock;

	CSimpleBlock(const char *name, int type);
	virtual ~CSimpleBlock();

	virtual int AddBlock(int id, CSimpleBlock *block);
	virtual int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
	virtual int RemoveNode();
	virtual void Render();
	virtual void ScanFunc(int(__cdecl *callback)(CSimpleBlock *));

	CSimpleBlock *GetRoot();
};

#endif