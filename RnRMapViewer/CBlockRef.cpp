#include "CBlockRef.h"

CBlockRef::CBlockRef(const char *name)
:CBlockContainer(name, 18, 0)
{
	someCount = 0;
	field_3C = 0;
}

CBlockRef::~CBlockRef()
{

}

int CBlockRef::ReadFromFile(FILE *f, void *data1, void *data2)
{
	GameModule::ReadVector4FromFile(f, &m_vPos);
	int realCount = GameModule::ReadIntFromFile(f);
	someCount = GameModule::ReadIntFromFile(f);

	int count = GameModule::ReadIntFromFile(f);
	return LoadChildren(f, count, realCount, data1, data2);
}

void CBlockRef::WriteToFile(FILE *f)
{

}