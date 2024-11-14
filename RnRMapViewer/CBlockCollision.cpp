#include "CBlockCollision.h"

CBlockCollision::CBlockCollision(const char *name)
:CSimpleBlock(name, 22)
{
	m_dwCount = 0;
	m_pVertices = 0;
	m_pIndices = 0;
}

CBlockCollision::~CBlockCollision()
{

}

int CBlockCollision::ReadFromFile(FILE *f, void *data1, void *data2)
{
	GameModule::ReadVector4FromFile(f, &m_vPos);
	GameModule::ReadIntFromFile(f);
	m_dwCount = GameModule::ReadIntFromFile(f);
	m_pVertices = new CVector[m_dwCount];
	m_pIndices = new int[m_dwCount];
	for (int i = 0; i < m_dwCount; i++)
	{
		GameModule::ReadVectorFromFile(f, &m_pVertices[i]);
		m_pIndices[i] = GameModule::ReadIntFromFile(f);
	}
	return 1;
}

void CBlockCollision::WriteToFile(FILE *f)
{

}

void CBlockCollision::InitPrimitiveData()
{

}