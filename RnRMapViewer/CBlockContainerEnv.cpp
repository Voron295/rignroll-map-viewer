#include "CBlockContainerEnv.h"

CBlockContainerEnv::CBlockContainerEnv(const char *name)
:CBlockContainer(name, 0x17, 1)
{
	fogStart = 100;
	fogEnd = 1000;
	field_48 = 0.3;
	field_58 = -1;
	field_50 = -1;
	field_60 = 8;
}

CBlockContainerEnv::~CBlockContainerEnv()
{

}

int CBlockContainerEnv::ReadFromFile(FILE *f, void *data1, void *data2)
{
	GameModule::ReadVector4FromFile(f, &m_vPos);
	field_48 = GameModule::ReadFloatFromFile(f);
	GameModule::ReadIntFromFile(f);
	fogStart = GameModule::ReadFloatFromFile(f);
	fogEnd = GameModule::ReadFloatFromFile(f);
	field_60 = GameModule::ReadIntFromFile(f);
	int count = GameModule::ReadIntFromFile(f);
	return LoadChildren(f, count, 1, data1, data2);
}

void CBlockContainerEnv::WriteToFile(FILE *f)
{

}