#include "CBlock0x19.h"

CBlock0x19::CBlock0x19(const char *name)
:CSimpleBlock(name, 16)
{
	field_50 = 0;
	field_54.x = .0f;
	field_54.y = .0f;
	field_54.z = .0f;
	field_60.x = .0f;
	field_60.y = .0f;
	field_60.z = .0f;
}

CBlock0x19::~CBlock0x19()
{

}

int CBlock0x19::ReadFromFile(FILE *f, void *data1, void *data2)
{
	char buf[32];
	field_34 = GameModule::ReadIntFromFile(f);
	field_30 = GameModule::ReadIntFromFile(f);
	field_38 = GameModule::ReadIntFromFile(f);
	fread(buf, 32, 1, f);
	GameModule::ReadVectorFromFile(f, &field_54);
	GameModule::ReadVectorFromFile(f, &field_60);
	field_40 = GameModule::ReadIntFromFile(f);
	field_44 = GameModule::ReadIntFromFile(f);
	field_48 = GameModule::ReadIntFromFile(f);
	field_4C = GameModule::ReadIntFromFile(f);
	field_3C = GameModule::ReadIntFromFile(f);
	return 1;
}

void CBlock0x19::WriteToFile(FILE *f)
{

}