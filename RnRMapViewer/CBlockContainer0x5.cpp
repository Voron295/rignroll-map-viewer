#include "CBlockContainer0x5.h"

CBlockContainer0x5::CBlockContainer0x5(const char *name)
:CBlockContainer(name, 5, 1)
{
	field_38 = 0;
	field_3C = 0;
}

CBlockContainer0x5::~CBlockContainer0x5()
{

}

int CBlockContainer0x5::ReadFromFile(FILE *f, void *data1, void *data2)
{
	char buf[32];
	GameModule::ReadVector4FromFile(f, &m_vPos);
	fread(&buf, 32, 1, f);
	int count = GameModule::ReadIntFromFile(f);
	return LoadChildren(f, count, 1, data1, data2);
}

void CBlockContainer0x5::WriteToFile(FILE *f)
{

}