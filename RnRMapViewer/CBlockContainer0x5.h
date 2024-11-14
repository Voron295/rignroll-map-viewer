#include "CBlockContainer.h"

class CBlockContainer0x5 : public CBlockContainer
{
public:
	int field_38;
	int field_3C;

	CBlockContainer0x5(const char *name);
	~CBlockContainer0x5();

	virtual int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
};
