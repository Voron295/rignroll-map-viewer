#ifndef CBLOCKCONTAINER0x7_CLASS_H
#define CBLOCKCONTAINER0x7_CLASS_H

#include "CBlockContainer.h"
#include "CFace.h"

class CBlockContainer0x7 : public CBlockContainer
{
public:
	int m_dwVerticesCount;
	Vertex *m_pVertices;
	CVector *m_pVertexNormals;
	PrimitiveRenderData m_pPrimitiveRenderData;
	int field_68;
	int flag_0x7_or_0x25;
	int field_70;

	CBlockContainer0x7(const char *name);
	~CBlockContainer0x7();

	virtual int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
	virtual void Render();
	void InitPrimitiveData();
};

#endif