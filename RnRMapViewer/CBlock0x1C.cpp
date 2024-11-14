#include "CBlock0x1C.h"

CBlock0x1C::CBlock0x1C(const char *name)
:CSimpleBlock(name, 11)
{
	count = 0;
	data = 0;
}

CBlock0x1C::~CBlock0x1C()
{

}

int CBlock0x1C::ReadFromFile(FILE *f, void *data1, void *data2)
{
	GameModule::ReadVector4FromFile(f, &m_vPos);
	GameModule::ReadVectorFromFile(f, &field_38);
	count = GameModule::ReadIntFromFile(f);
	data = new CBlock0x1CSub[count];
	for (int i = 0; i < count; i++)
	{
		data[i].ReadFromFile(f);
	}
	return 1;
}

int CBlock0x1CSub::ReadFromFile(FILE *f)
{
	field_0 = GameModule::ReadIntFromFile(f);
	field_6 = HIBYTE(field_0);
	GameModule::ReadIntFromFile(f);
	field_22 = GameModule::ReadIntFromFile(f);
	field_20 = GameModule::ReadIntFromFile(f);
	field_4 = GameModule::ReadIntFromFile(f);
	if (field_0 & 2)
		field_6++;

	for (int i = 0; i < field_4; i++)
	{
		GameModule::ReadFloatFromFile(f);
		GameModule::ReadFloatFromFile(f);
		if (field_0 & 2)
		{
			if (field_6)
			{
				for (int j = 0; j < field_6; j++)
				{
					GameModule::ReadFloatFromFile(f);
					GameModule::ReadFloatFromFile(f);
				}
			}
		}
	}
	return 1;
}

void CBlock0x1CSub::WriteToFile(FILE *f)
{
	GameModule::WriteIntToFile(f, field_0);
	GameModule::WriteIntToFile(f, 0);
	GameModule::WriteIntToFile(f, field_22);
	GameModule::WriteIntToFile(f, field_20);
	GameModule::WriteIntToFile(f, field_4);

	for (int i = 0; i < field_4; i++)
	{

	}
}

void CBlock0x1C::WriteToFile(FILE *f)
{

}