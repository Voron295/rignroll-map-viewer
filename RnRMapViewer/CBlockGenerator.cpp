#include "CBlockGenerator.h"

CBlockGenerator::CBlockGenerator(const char *name)
:CSimpleBlock(name, 24)
{
	field_3C.data = 0;
	field_38 = 0;
	callback = 0;
	field_34 = 0;
}

CBlockGenerator::~CBlockGenerator()
{

}

int CBlockGenerator::ReadFromFile(FILE *f, void *data1, void *data2)
{
	char buf[32];
	GameModule::ReadVector4FromFile(f, &m_vPos);
	fread(buf, 32, 1, f);
	fread(buf, 32, 1, f);
	field_3C.ReadFromFile(f);
	return 1;
}

void CBlockGenerator::WriteToFile(FILE *f)
{

}