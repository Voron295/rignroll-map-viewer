#include "CBlockContainerStatic.h"

CBlockContainerStatic::CBlockContainerStatic(const char *name)
:CBlockContainer(name, 4, 1)
{
	field_38 = 0;
	m_pAttachedTo = 0;
	field_40 = 0;
}

CBlockContainerStatic::~CBlockContainerStatic()
{

}

int CBlockContainerStatic::ReadFromFile(FILE *f, void *data1, void *data2)
{
	char buf[32];
	GameModule::ReadVector4FromFile(f, &m_vPos);
	fread(buf, 32, 1, f);
	fread(buf, 32, 1, f);
	int count = GameModule::ReadIntFromFile(f);
	return LoadChildren(f, count, 1, data1, data2);
}

void CBlockContainerStatic::WriteToFile(FILE *f)
{

}