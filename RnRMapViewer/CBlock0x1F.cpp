#include "CBlock0x1F.h"

CBlock0x1F::CBlock0x1F(const char *name)
:CSimpleBlock(name, 20)
{
	m_dwTransparency = 0;
	m_dwAlignType = 0;
	m_pBlendValues = 0;
	m_pMasksId = 0;
}

CBlock0x1F::~CBlock0x1F()
{

}

int CBlock0x1F::ReadFromFile(FILE *f, void *data1, void *data2)
{
	GameModule::ReadVector4FromFile(f, &m_vPos);
	count = GameModule::ReadIntFromFile(f);
	GameModule::ReadVector4FromFile(f, &field_38);
	int flag = GameModule::ReadIntFromFile(f);
	if (flag & 1)
		m_dwTransparency = 0;
	else
		m_dwTransparency = 1;
	m_dwAlignType = flag >> 1;
	GameModule::ReadVectorFromFile(f, &m_pPosition);
	m_pBlendValues = new float[count];
	m_pMasksId = new int[count];
	for (int i = 0; i < count; i++)
	{
		m_pBlendValues[i] = GameModule::ReadFloatFromFile(f);
		m_pMasksId[i] = GameModule::ReadIntFromFile(f);
	}
	return 1;
}

void CBlock0x1F::WriteToFile(FILE *f)
{

}