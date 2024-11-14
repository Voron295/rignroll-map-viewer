#include "CBlockContainer.h"
#include "CFace.h"

class CBlockModelElement : public CSimpleBlock
{
public:
	Vertex *m_pVertices;
	__int16 m_dwFacesCount;
	__int16 field_36;
	CFace *m_pFaces;
	PrimitiveRenderData m_pPrimitiveRenderData;
	__int16 m_wFlag;
	__int16 m_wMaterialId;

	CBlockModelElement(const char *name);
	~CBlockModelElement();

	virtual int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
	virtual void Render();
	void RenderFace(CFace *face);

	void InitPrimitiveData();
};