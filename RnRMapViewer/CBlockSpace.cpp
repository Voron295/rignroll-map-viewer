#include "CBlockSpace.h"

CBlockSpace::CBlockSpace(const char *name)
:CBlockContainer(name, 6, 1)
{

}

CBlockSpace::~CBlockSpace()
{

}

int CBlockSpace::ReadFromFile(FILE *f, void *data1, void *data2)
{
	GameModule::ReadMatrixFromFile(f, &m_pMatrix);
	int flag = GameModule::ReadIntFromFile(f);
	if (flag)
	{
		if (flag == 1)
			field_38 = 1;
	}
	else 
	{
		field_38 = 0;
	}
	int count = GameModule::ReadIntFromFile(f);
	return LoadChildren(f, count, 1, data1, data2);
}

void CBlockSpace::WriteToFile(FILE *f)
{

}