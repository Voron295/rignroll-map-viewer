
#include "CBlockSpace.h"

class CBlockContainerStatic : public CBlockContainer
{
public:
	int field_38;
	CBlockSpace *m_pAttachedTo;
	int field_40;

	CBlockContainerStatic(const char *name);
	~CBlockContainerStatic();

	virtual int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
};