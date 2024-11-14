#include "CBlockRoomDesc.h"
#include "GraphicsFuncs.h"

extern IDirect3DDevice9 *g_pDirect3DDevice;

CBlockRoomDesc::CBlockRoomDesc(const char *name)
:CSimpleBlock(name, 14)
{
	field_80 = 0;
	field_6C = 0;
	field_84[0] = 0;
	m_sRoomName[0] = 0;
}

CBlockRoomDesc::~CBlockRoomDesc()
{

}

int CBlockRoomDesc::ReadFromFile(FILE *f, void *data1, void *data2)
{
	char buf[64];
	CVector v1;
	CVector v2;
	GameModule::ReadVector4FromFile(f, &field_70);
	fread(buf, 32, 1, f);
	GameModule::ReadVectorFromFile(f, &m_vStartPos);
	GameModule::ReadVectorFromFile(f, &m_vEndPos);

	float _y2;
	float _z2;

	if (fabs(m_vStartPos.z - m_vEndPos.z) >= 0.1)
	{
		_y2 = v1.y;
		_z2 = v1.z;
		v1.z = m_vEndPos.z - m_vStartPos.z;
		v1.x = m_vEndPos.x - m_vStartPos.x;
		v1.y = m_vStartPos.y - m_vEndPos.y;

		v2.x = v1.z * v1.y;
		v2.y = -v1.z * v1.x;
		v2.z = 0;
		v2.Normalize();
		field_30 = v2;
	}
	else
	{
		_y2 = v2.y;
		_z2 = v2.z;
		v1.z = m_vStartPos.z - m_vEndPos.z;
		v1.y = m_vEndPos.y - m_vStartPos.y;
		v1.x = m_vStartPos.x - m_vEndPos.y;
		v2.x = -v1.y * v1.z;
		v2.y = 0;
		v2.z = v1.y * v1.x;
	}
	
	m_pPoints[0] = m_vStartPos;
	m_pPoints[1].x = m_vEndPos.x;
	m_pPoints[1].y = _y2;
	m_pPoints[1].z = _z2;
	m_pPoints[2] = m_vEndPos;
	m_pPoints[3].x = m_vStartPos.x;
	m_pPoints[3].y = _y2;
	m_pPoints[3].z = m_vEndPos.z;

	return 1;
}

void CBlockRoomDesc::WriteToFile(FILE *f)
{

}

void CBlockRoomDesc::Render()
{
	RenderLine3D(g_pDirect3DDevice, &D3DXVECTOR3(m_vStartPos.x, m_vStartPos.y, m_vStartPos.z), &D3DXVECTOR3(m_vEndPos.x, m_vEndPos.y, m_vEndPos.z), D3DCOLOR_XRGB(255, 0, 0), D3DCOLOR_XRGB(0, 255, 0));
	//RenderLine3D(g_pDirect3DDevice, &D3DXVECTOR3(m_pPoints[0].x, m_pPoints[0].y, m_pPoints[0].z), &D3DXVECTOR3(m_pPoints[2].x, m_pPoints[2].y, m_pPoints[2].z), D3DCOLOR_XRGB(255, 0, 0));
	//RenderLine3D(g_pDirect3DDevice, &D3DXVECTOR3(m_pPoints[1].x, m_pPoints[1].y, m_pPoints[1].z), &D3DXVECTOR3(m_pPoints[2].x, m_pPoints[2].y, m_pPoints[2].z), D3DCOLOR_XRGB(255, 0, 0));
	//RenderLine3D(g_pDirect3DDevice, &D3DXVECTOR3(m_pPoints[2].x, m_pPoints[2].y, m_pPoints[2].z), &D3DXVECTOR3(m_pPoints[3].x, m_pPoints[3].y, m_pPoints[3].z), D3DCOLOR_XRGB(255, 0, 0));
	//RenderLine3D(g_pDirect3DDevice, &D3DXVECTOR3(m_pPoints[3].x, m_pPoints[3].y, m_pPoints[3].z), &D3DXVECTOR3(m_pPoints[0].x, m_pPoints[0].y, m_pPoints[0].z), D3DCOLOR_XRGB(255, 0, 0));
}