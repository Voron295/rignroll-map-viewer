#include "CBlockContainer0x1D.h"

CBlockContainer0x1D::CBlockContainer0x1D(const char *name)
:CBlockContainer(name, 13, 0)
{
	field_38 = 0;
	field_50 = 0;
	field_3C = 0;
}

CBlockContainer0x1D::~CBlockContainer0x1D()
{

}

int CBlockContainer0x1D::ReadFromFile(FILE *f, void *data1, void *data2)
{
	GameModule::ReadVector4FromFile(f, &m_vPos);
	int someCount = GameModule::ReadIntFromFile(f);
	int flag = GameModule::ReadIntFromFile(f);
	if (flag & 3)
	{
		if ((flag & 3) == 1)
		{
			field_38 = 1;
		}
		else if ((flag & 3) == 2)
		{
			field_38 = 2;
		}
	}
	else
	{
		field_38 = 0;
	}
	if (flag & 8)
		field_3C |= 1;

	GameModule::ReadVector4FromFile(f, &field_40);
	field_50 = new float[someCount];
	for (int i = 0; i < someCount; i++)
	{
		field_50[i] = GameModule::ReadFloatFromFile(f);
	}
	int count = GameModule::ReadIntFromFile(f);
	return LoadChildren(f, count, someCount, data1, data2);
}

void CBlockContainer0x1D::WriteToFile(FILE *f)
{

}