#include "CBlockContainer0x6.h"

extern PrimitiveRenderData *g_pCurrentPrimitiveDataRendering;
extern PrimitiveRenderData *g_pCurrentPrimitiveDataProcessing;

CBlockContainer0x6::CBlockContainer0x6(const char *name)
:CBlockContainer(name, 2, 1)
{
	field_38 = 0;
	field_70 = 0;
	field_74 = 0;
	m_pVertexNormals = 0;
	flag_0x6_or_0x24 = 0;
}

CBlockContainer0x6::~CBlockContainer0x6()
{

}

int CBlockContainer0x6::ReadFromFile(FILE *f, void *data1, void *data2)
{
	char buf[32];
	GameModule::ReadVector4FromFile(f, &m_vPos);
	fread(buf, 32, 1, f);
	fread(buf, 32, 1, f);

	if (flag_0x6_or_0x24)
		flag_0x6_or_0x24 = GameModule::ReadIntFromFile(f);

	int uvCount = flag_0x6_or_0x24 >> 8;
	flag_0x6_or_0x24 = (unsigned __int8)flag_0x6_or_0x24;
	m_dwVerticesCount = GameModule::ReadIntFromFile(f);
	if (m_dwVerticesCount)
	{
		m_pVertices = new Vertex[m_dwVerticesCount];
	}
	else
	{
		m_pVertices = 0;
	}

	if (flag_0x6_or_0x24 > 0)
	{
		if (m_dwVerticesCount > 0)
		{
			m_pVertexNormals = new CVector[m_dwVerticesCount];
			if (m_pVertexNormals && (m_dwVerticesCount & 0x80000000) == 0)
			{
				for (int i = 0; i < m_dwVerticesCount; i++)
				{
					m_pVertexNormals[i].x = 0;
					m_pVertexNormals[i].y = 0;
					m_pVertexNormals[i].z = 0;
				}
			}
		}
	}

	for (int i = 0; i < m_dwVerticesCount; i++)
	{
		GameModule::ReadVertexFromFile(f, &m_pVertices[i]);
		if (uvCount > 0)
		{
			m_pVertices[i].uvCount = uvCount;
			m_pVertices[i].m_pUVs = new UV[uvCount];
			for (int j = 0; j < uvCount; j++)
			{
				m_pVertices[i].m_pUVs[j].u = GameModule::ReadFloatFromFile(f);
				m_pVertices[i].m_pUVs[j].v = GameModule::ReadFloatFromFile(f);
			}
		}

		if (flag_0x6_or_0x24 == 2 || flag_0x6_or_0x24 == 1)
		{
			GameModule::ReadVectorFromFile(f, &m_pVertexNormals[i]);
		}
		else if (flag_0x6_or_0x24 == 3)
		{
			m_pVertexNormals[i].x = GameModule::ReadFloatFromFile(f);
		}
	}
	int count = GameModule::ReadIntFromFile(f);
	return LoadChildren(f, count, 1, m_pVertices, m_pVertexNormals);
}

void CBlockContainer0x6::WriteToFile(FILE *f)
{

}

extern IDirect3DDevice9 *g_pDirect3DDevice;

void CBlockContainer0x6::InitPrimitiveData()
{
	CVertex *vertices;
	m_pPrimitiveRenderData.m_pVertexBuffer = NULL;
	m_pPrimitiveRenderData.m_pIndicesBuffer = NULL;
	WORD *verticesInd;
	if (!m_dwVerticesCount)
		return;

	if (!FAILED(g_pDirect3DDevice->CreateVertexBuffer(
		m_dwVerticesCount * sizeof(CVertex),	// Размер буфера
		D3DUSAGE_WRITEONLY,	// Используем буфер только  для чтения
		CVERTEX_TYPE,	// Формат наших вершин
		D3DPOOL_MANAGED,	// DirectX сам управляет пересозданием буфера
		&m_pPrimitiveRenderData.m_pVertexBuffer,	// Наш указатель  на буфер
		NULL		// Оставим NULL
		)))
	{
		m_pPrimitiveRenderData.m_pVertexBuffer->Lock(0, m_dwVerticesCount * sizeof(CVertex), (void**)&vertices, 0);
		for (int i = 0; i < m_dwVerticesCount; i++)
		{
			vertices[i].x = m_pVertices[i].pos.x;
			vertices[i].y = m_pVertices[i].pos.y;
			vertices[i].z = m_pVertices[i].pos.z;

			if (m_pVertexNormals)
			{
				vertices[i].nx = m_pVertexNormals[i].x;
				vertices[i].ny = m_pVertexNormals[i].y;
				vertices[i].nz = m_pVertexNormals[i].z;
			}
			else
			{
				vertices[i].nx = .0f;
				vertices[i].ny = .0f;
				vertices[i].nz = 1.0f;
			}

			vertices[i].tx = m_pVertices[i].u;
			vertices[i].ty = m_pVertices[i].v;

			//vertices[i].color = D3DCOLOR_XRGB(255, 255, 255);
		}
		m_pPrimitiveRenderData.m_pVertexBuffer->Unlock();
		g_pCurrentPrimitiveDataProcessing = &m_pPrimitiveRenderData;
	}
}

void CBlockContainer0x6::Render()
{
	g_pCurrentPrimitiveDataRendering = &m_pPrimitiveRenderData;
	RenderChildren(0, 0);
	g_pCurrentPrimitiveDataRendering = 0;
}