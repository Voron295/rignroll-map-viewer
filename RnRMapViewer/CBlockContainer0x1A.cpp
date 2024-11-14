#include "CBlockContainer0x1A.h"

CBlockContainer0x1A::CBlockContainer0x1A(const char *name)
:CBlockContainer(name, 19, 1)
{
	field_38.x = .0f;
	field_38.y = .0f;
	field_38.z = .0f;
	field_44 = .0f;
}

CBlockContainer0x1A::~CBlockContainer0x1A()
{

}

int CBlockContainer0x1A::ReadFromFile(FILE *f, void *data1, void *data2)
{
	CVector a1, a2, a3, a4;
	GameModule::ReadVector4FromFile(f, &m_vPos);
	GameModule::ReadVectorFromFile(f, &a1);
	GameModule::ReadVectorFromFile(f, &a2);
	GameModule::ReadVectorFromFile(f, &a3);
	field_38.x = (a3.x + a2.x) * 0.5;
	field_38.y = (a3.y + a2.y) * 0.5;
	field_38.z = (a3.z + a2.z) * 0.5;
	a4.x = a3.x - a2.x;
	a4.y = a3.y - a2.y;
	a4.z = a3.z - a2.z;

	field_44 = a4.Length() * 0.5;

	int count = GameModule::ReadIntFromFile(f);
	return LoadChildren(f, count, 1, data1, data2);
}

void CBlockContainer0x1A::WriteToFile(FILE *f)
{

}