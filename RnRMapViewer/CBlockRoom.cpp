#include "CBlockRoom.h"

CBlockRoom::CBlockRoom(const char *name)
:CBlockContainer(name, 15, 1)
{
	m_pFaces = 0;
	field_110 = 0;
	someCount = 0;
	field_114[0] = 0;
	field_114[1] = 0;
}

CBlockRoom::~CBlockRoom()
{

}

int CBlockRoom::ReadFromFile(FILE *f, void *data1, void *data2)
{
	int count = GameModule::ReadIntFromFile(f);
	return LoadChildren(f, count, 1, data1, data2);
}

void CBlockRoom::WriteToFile(FILE *f)
{

}