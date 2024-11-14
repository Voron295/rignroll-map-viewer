#ifndef CBLOCKCONTAINER0X6_CLASS_H
#define CBLOCKCONTAINER0X6_CLASS_H

#include "CBlockSpace.h"
#include "CFace.h"

class CBlockContainer0x6 : public CBlockContainer
{
public:
	int field_38;
	int m_dwVerticesCount;
	Vertex *m_pVertices;
	CVector *m_pVertexNormals;
	PrimitiveRenderData m_pPrimitiveRenderData;
	int flag_0x6_or_0x24;
	CBlockSpace *field_70;
	int field_74;

	CBlockContainer0x6(const char *name);
	~CBlockContainer0x6();

	virtual int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
	virtual void Render();
	void InitPrimitiveData();
};

#endif