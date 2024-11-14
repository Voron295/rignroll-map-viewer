#include "CBlockAnmObj.h"
#include "ResourceManager.h"

extern IDirect3DDevice9 *g_pDirect3DDevice;

CBlockAnmObj::CBlockAnmObj(const char *name)
:CSimpleBlock(name, 17)
{
	field_30 = 0;
	field_34 = 0;
	field_38 = .0f;
	field_3C = .0f;
	m_pBlock = 0;
	m_pSpace = 0;
	field_48 = 0;
	field_4C = 0;
	field_50 = 0;
	field_54 = -1;
}

CBlockAnmObj::~CBlockAnmObj()
{

}

int CBlockAnmObj::ReadFromFile(FILE *f, void *data1, void *data2)
{
	char buf[32];
	GameModule::ReadVector4FromFile(f, &m_vPos);
	fread(buf, 32, 1, f);
	m_pSpace = (CBlockSpace*)ResourceManager::GetBlockFromPool(buf);
	fread(buf, 32, 1, f);
	m_pBlock = ResourceManager::GetBlockFromPool(buf);
	return 1;
}

void CBlockAnmObj::WriteToFile(FILE *f)
{

}

void CBlockAnmObj::Render()
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	if (m_pSpace)
	{
		matWorld.m[0][0] = m_pSpace->m_pMatrix.m_fRightX;
		matWorld.m[0][1] = m_pSpace->m_pMatrix.m_fRightY;
		matWorld.m[0][2] = m_pSpace->m_pMatrix.m_fRightZ;
		matWorld.m[0][3] = 0;

		matWorld.m[1][0] = m_pSpace->m_pMatrix.m_fTopX;
		matWorld.m[1][1] = m_pSpace->m_pMatrix.m_fTopY;
		matWorld.m[1][2] = m_pSpace->m_pMatrix.m_fTopZ;
		matWorld.m[1][3] = 0;

		matWorld.m[2][0] = m_pSpace->m_pMatrix.m_fAtX;
		matWorld.m[2][1] = m_pSpace->m_pMatrix.m_fAtY;
		matWorld.m[2][2] = m_pSpace->m_pMatrix.m_fAtZ;
		matWorld.m[2][3] = 0;

		matWorld.m[3][0] = m_pSpace->m_pMatrix.m_fPosX;
		matWorld.m[3][1] = m_pSpace->m_pMatrix.m_fPosY;
		matWorld.m[3][2] = m_pSpace->m_pMatrix.m_fPosZ;
		matWorld.m[3][3] = 1.0f;

		g_pDirect3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	}

	if (m_pBlock)
		m_pBlock->Render();

	if (m_pSpace)
	{
		D3DXMatrixIdentity(&matWorld);
		g_pDirect3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	}
}