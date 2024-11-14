#include "CBlockContainer0x2.h"

CBlockContainer0x2::CBlockContainer0x2(const char *name)
:CBlockContainer(name, 7, 3)
{
	field_38.pos.x = .0f;
	field_38.pos.y = .0f;
	field_38.pos.z = .0f;
	field_38.len = .0f;
}

CBlockContainer0x2::~CBlockContainer0x2()
{

}

int CBlockContainer0x2::ReadFromFile(FILE *f, void *data1, void *data2)
{
	GameModule::ReadVector4FromFile(f, &m_vPos);
	GameModule::ReadVector4FromFile(f, &field_38);
	int count = GameModule::ReadIntFromFile(f);
	return LoadChildren(f, count, 3, data1, data2);
}

void CBlockContainer0x2::WriteToFile(FILE *f)
{

}