#include "CBlockContainer.h"

class CBlockContainer0x2 : public CBlockContainer
{
public:
	CVector4 field_38;

	CBlockContainer0x2(const char *name);
	~CBlockContainer0x2();

	virtual int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
};
