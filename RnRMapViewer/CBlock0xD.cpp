#include "CBlock0xD.h"

CBlock0xD::CBlock0xD(const char *name)
:CSimpleBlock(name, 9)
{
	field_30.data = 0;
}

CBlock0xD::~CBlock0xD()
{
	
}

int CBlock0xD::ReadFromFile(FILE *f, void *data1, void *data2)
{
	if (data.type == 6)
	{
		field_30.ReadFromFile(f);
		data.ReadFromFile(f);
		return 1;
	}
	if (data.type == 7)
	{
		GameModule::ReadVector4FromFile(f, &m_vPos);
		data.count = GameModule::ReadIntFromFile(f);
		field_30.ReadFromFile(f);
		data.ReadFromFile(f);
		return 1;
	}
	GameModule::ReadVector4FromFile(f, &m_vPos);
	data.ReadFromFile(f);
	field_30.ReadFromFile(f);
	return 1;
}

int CBlock0xDSub::ReadFromFile(FILE *f)
{
	flags = GameModule::ReadIntFromFile(f);
	field_4 = GameModule::ReadIntFromFile(f);
	count = GameModule::ReadIntFromFile(f);
	if (count)
	{
		data = new int[count];
		fread(data, count, 4, f);
	}
	return count;
}

void CBlock0xD::WriteToFile(FILE *f)
{

}