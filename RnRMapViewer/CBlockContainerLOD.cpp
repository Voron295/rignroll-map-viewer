#include "CBlockContainerLOD.h"

CBlockContainerLOD::CBlockContainerLOD(const char *name)
:CBlockContainer(name, 8, 2)
{
	
}

CBlockContainerLOD::~CBlockContainerLOD()
{

}

int CBlockContainerLOD::ReadFromFile(FILE *f, void *data1, void *data2)
{
	GameModule::ReadVector4FromFile(f, &m_vPos);
	data.ReadFromFile(f);
	int count = GameModule::ReadIntFromFile(f);
	return LoadChildren(f, count, 2, data1, data2);
}

int CBlockContainer0x9Sub::ReadFromFile(FILE *f)
{
	switch (type)
	{
	case 1:
	case 2:
		data = new CVector4;
		GameModule::ReadVector4FromFile(f, (CVector4 *)data);
		break;

	case 3:
	case 4:
		data = malloc(0x20);
		fread(data, 0x20, 1, f);
		break;

	case 5: // collision header
		data = new CVector4;
		GameModule::ReadVector4FromFile(f, (CVector4 *)data);
		break;

	case 6:
		count = GameModule::ReadIntFromFile(f);
		for (int i = 0; i < count; i++)
		{
			CVector v;
			int cnt = GameModule::ReadIntFromFile(f);
			for (int j = 0; j < cnt; j++)
			{
				GameModule::ReadVectorFromFile(f, &v);
			}
		}
		break;

	case 7:
		for (int i = 0; i < count; i++)
		{
			CVector v;
			GameModule::ReadVectorFromFile(f, &v);
		}
		break;

	default:
		return 1;
	}

	return 1;
}

void CBlockContainerLOD::WriteToFile(FILE *f)
{

}

void CBlockContainerLOD::Render()
{
	/*if (GetTickCount() % 10000 > 5000)
	{
		RenderChildren(0, 0);
	}
	else
	{
		RenderChildren(1, 0);
	}*/

	RenderChildren(0, 0);
}