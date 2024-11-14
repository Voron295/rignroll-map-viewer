#include "CBlockContainerLamp.h"

CBlockContainerLamp::CBlockContainerLamp(const char *name)
:CBlockContainer(name, 21, 1)
{
	field_88 = 1;
	field_8C = 0;
}

CBlockContainerLamp::~CBlockContainerLamp()
{

}

int CBlockContainerLamp::ReadFromFile(FILE *f, void *data1, void *data2)
{
	GameModule::ReadVector4FromFile(f, &m_vPos);
	field_88 = GameModule::ReadIntFromFile(f);
	if (GameModule::ReadIntFromFile(f))
		field_8C = 1;
	else
		field_8C = 0;
	
	switch (GameModule::ReadIntFromFile(f))
	{
	case 0:
		field_38.field_0 = 0;
		break;

	case 1:
		field_38.field_0 = 1;
		field_38.field_48 = 210;
		break;

	case 2:
		field_38.field_0 = 2;
		break;

	case 3:
		field_38.field_0 = 3;
		field_38.field_48 = 130;
		break;

	default:
		break;
	}

	GameModule::ReadVectorFromFile(f, &field_38.field_4);
	GameModule::ReadVectorFromFile(f, &field_38.field_10);
	field_38.field_1C = GameModule::ReadFloatFromFile(f);
	field_38.field_20 = GameModule::ReadFloatFromFile(f);
	field_38.field_24 = GameModule::ReadFloatFromFile(f);
	field_38.field_28 = GameModule::ReadFloatFromFile(f);
	field_38.someAngle = GameModule::ReadFloatFromFile(f);
	if (field_38.someAngle > 3.1415926)
		field_38.someAngle = 3.1415925;
	field_38.field_30 = GameModule::ReadFloatFromFile(f);
	if (field_38.field_30 + field_38.someAngle > 3.1415926)
		field_38.field_30 = 3.1415926 - field_38.someAngle;
	field_38.field_34 = GameModule::ReadFloatFromFile(f);
	field_38.field_38 = GameModule::ReadFloatFromFile(f);
	field_38.field_3C = GameModule::ReadFloatFromFile(f);
	int count = GameModule::ReadIntFromFile(f);
	return LoadChildren(f, count, 1, data1, data2);
}

void CBlockContainerLamp::WriteToFile(FILE *f)
{

}