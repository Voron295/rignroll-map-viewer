#include "CBlockContainer.h"
#include "LightData.h"

class CBlockContainerLamp : public CBlockContainer
{
public:
	LightData field_38;
	int field_88;
	int field_8C;

	CBlockContainerLamp(const char *name);
	~CBlockContainerLamp();

	virtual int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
};