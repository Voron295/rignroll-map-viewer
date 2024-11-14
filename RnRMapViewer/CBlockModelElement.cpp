#include "CBlockModelElement.h"
#include "CBlockContainer0x6.h"
#include "CBlockContainer0x7.h"

extern PrimitiveRenderData *g_pCurrentPrimitiveDataRendering;
extern PrimitiveRenderData *g_pCurrentPrimitiveDataProcessing;

CBlockModelElement::CBlockModelElement(const char *name)
:CSimpleBlock(name, 1)
{
	m_wFlag = 0;
	m_wMaterialId = 0;
	m_dwFacesCount = 0;
	m_pFaces = 0;
	m_pVertices = 0;
}

CBlockModelElement::~CBlockModelElement()
{

}

int CBlockModelElement::ReadFromFile(FILE *f, void *data1, void *data2)
{
	GameModule::ReadVector4FromFile(f, &m_vPos);
	if (m_wFlag)
	{
		m_wFlag = GameModule::ReadIntFromFile(f);
		if (m_wFlag == 1)
			m_wFlag = 2;
		m_wMaterialId = GameModule::ReadIntFromFile(f);
	}
	m_dwFacesCount = GameModule::ReadIntFromFile(f);
	m_pVertices = (Vertex*)data1;

	m_pFaces = new CFace[m_dwFacesCount];

	for (int i = 0; i < m_dwFacesCount; i++)
	{
		m_pFaces[i].ReadFromFile(f, (Vertex *)data1, (CVector *)data2);
	}
	return 1;
}

extern IDirect3DDevice9 *g_pDirect3DDevice;

void CBlockModelElement::InitPrimitiveData()
{
	m_pPrimitiveRenderData.m_pVertexBuffer = NULL;
	m_pPrimitiveRenderData.m_pIndicesBuffer = NULL;

	if (!m_pFaces)
		return;

	if (m_wFlag != 3)  // составной
	{
		for (int i = 0; i < m_dwFacesCount; i++)
		{
			CFace *face = &m_pFaces[i];

			g_pDirect3DDevice->CreateIndexBuffer(
				face->m_wVertsCount * sizeof(short),	// Размер буфера индексов
				D3DUSAGE_WRITEONLY,	// Используем буфер только  для чтения
				D3DFMT_INDEX16, 	// Формат индексов  (для short - D3DFMT_INDEX16)
				D3DPOOL_MANAGED,	// DirectX сам управляет пересозданием буфера
				&face->m_pPrimitiveRenderData.m_pIndicesBuffer,	// Наш указатель  на буфер
				NULL		// Оставим NULL
				);
			WORD *vertIds;
			face->m_pPrimitiveRenderData.m_pIndicesBuffer->Lock(0, face->m_wVertsCount * sizeof(short), (void**)&vertIds, 0);
			memcpy(vertIds, face->m_pVertsIndices, face->m_wVertsCount * sizeof(short));

			CVertex *vertices = NULL;
			if (face->m_wTexCoordsPerVertexCount && g_pCurrentPrimitiveDataProcessing)
			{
				g_pCurrentPrimitiveDataProcessing->m_pVertexBuffer->Lock(0, 0, (void**)&vertices, 0);
				for (int j = 0; j < face->m_wVertsCount; j++)
				{
					vertices[*vertIds].tx = face->m_pTexCoordsU[j];
					vertices[*vertIds].ty = face->m_pTexCoordsV[j];
					vertIds++;
				}
				g_pCurrentPrimitiveDataProcessing->m_pVertexBuffer->Unlock();
			}
			face->m_pPrimitiveRenderData.m_pIndicesBuffer->Unlock();

		}
	}
	else
	{
		g_pDirect3DDevice->CreateIndexBuffer(
			m_dwFacesCount * 3 * sizeof(short),	// Размер буфера индексов
			D3DUSAGE_WRITEONLY,	// Используем буфер только  для чтения
			D3DFMT_INDEX16, 	// Формат индексов  (для short - D3DFMT_INDEX16)
			D3DPOOL_MANAGED,	// DirectX сам управляет пересозданием буфера
			&m_pPrimitiveRenderData.m_pIndicesBuffer,	// Наш указатель  на буфер
			NULL		// Оставим NULL
			);
		WORD *vertIds;
		m_pPrimitiveRenderData.m_pIndicesBuffer->Lock(0, m_dwFacesCount * 3 * sizeof(short), (void**)&vertIds, 0);

		for (int i = 0; i < m_dwFacesCount; i++)
		{
			CFace *face = &m_pFaces[i];

			CVertex *vertices = NULL;
			if (face->m_wTexCoordsPerVertexCount && g_pCurrentPrimitiveDataProcessing)
			{
				g_pCurrentPrimitiveDataProcessing->m_pVertexBuffer->Lock(0, 0, (void**)&vertices, 0);
			}

			for (int j = 0; j < face->m_wVertsCount; j++)
			{
				*vertIds = face->m_pVertsIndices[j];

				if (face->m_wTexCoordsPerVertexCount && vertices)
				{
					vertices[*vertIds].tx = face->m_pTexCoordsU[j];
					vertices[*vertIds].ty = face->m_pTexCoordsV[j];
				}

				vertIds++;
			}

			if (face->m_wTexCoordsPerVertexCount && g_pCurrentPrimitiveDataProcessing)
			{
				g_pCurrentPrimitiveDataProcessing->m_pVertexBuffer->Unlock();
			}
		}
		m_pPrimitiveRenderData.m_pIndicesBuffer->Unlock();
	}
}

extern IDirect3DTexture9 *g_tex1;
extern IDirect3DTexture9 *g_tex2;
extern IDirect3DTexture9 *g_tex3;
extern IDirect3DTexture9 *g_tex4;

void CBlockModelElement::RenderFace(CFace *face)
{
	if (!g_pCurrentPrimitiveDataRendering->m_pVertexBuffer || !face->m_pPrimitiveRenderData.m_pIndicesBuffer)
		return;

	LPDIRECT3DTEXTURE9 d3dtexture = g_tex1;

	if (face->m_wMaterialId >= 0 && face->m_wMaterialId < m_pModule->m_pMaterials.size())
	{
		auto texture = m_pModule->m_pMaterials[face->m_wMaterialId]->m_pTexture;
		if (texture && texture->m_bFormat == 5)
		{
			g_pDirect3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			g_pDirect3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			g_pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
		}
		else
		{
			g_pDirect3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
			g_pDirect3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTCOLOR);
			g_pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
		}
		if (texture && texture->m_pSurface)
			d3dtexture = texture->m_pSurface;

		g_pDirect3DDevice->SetMaterial(&m_pModule->m_pMaterials[face->m_wMaterialId]->m_pMaterial3D);
	}

	if (face->internalFormat & 128)
		d3dtexture = g_tex1;
	g_pDirect3DDevice->SetTexture(0, d3dtexture);

		

	g_pDirect3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_pDirect3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pDirect3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	g_pDirect3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pDirect3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pDirect3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	g_pDirect3DDevice->SetStreamSource(0, g_pCurrentPrimitiveDataRendering->m_pVertexBuffer, 0, sizeof(CVertex));
	g_pDirect3DDevice->SetIndices(face->m_pPrimitiveRenderData.m_pIndicesBuffer);
	g_pDirect3DDevice->SetFVF(CVERTEX_TYPE);

	g_pDirect3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	if (face->internalFormat & 4)
		g_pDirect3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//int type = *(int *)(((DWORD)&a1) + 0x21AC);
	int type = 6 - (*((__int8*)&(face->internalFormat)) < 0);

	g_pDirect3DDevice->DrawIndexedPrimitive((D3DPRIMITIVETYPE)type, 0, 0, face->m_wVertsCount, 0, face->m_wVertsCount - 2);
	g_pDirect3DDevice->SetTexture(0, 0);
	if (face->internalFormat & 4)
		g_pDirect3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
}

int gFacesRendered = 0;

void CBlockModelElement::Render()
{
	int flags = 0;
	if (!g_pCurrentPrimitiveDataRendering)
		return;

	//g_pDirect3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//logFile->WriteLine("RenderModelElement, faces count: %d", block->m_dwFacesCount);

	if (m_wFlag == 3)
	{
		LPDIRECT3DTEXTURE9 d3dtexture = g_tex2;
		if (m_pPrimitiveRenderData.m_pIndicesBuffer)
		{
			if (m_wMaterialId >= 0 && m_wMaterialId < m_pModule->m_pMaterials.size())
			{
				auto texture = m_pModule->m_pMaterials[m_wMaterialId]->m_pTexture;
				if (texture && texture->m_bFormat == 5)
				{
					g_pDirect3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
					g_pDirect3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
					g_pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
				}
				else
				{
					g_pDirect3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
					g_pDirect3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTCOLOR);
					g_pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
				}
				if (texture && texture->m_pSurface)
					d3dtexture = texture->m_pSurface;

				g_pDirect3DDevice->SetMaterial(&m_pModule->m_pMaterials[m_wMaterialId]->m_pMaterial3D);
			}
			
			g_pDirect3DDevice->SetTexture(0, d3dtexture);
			g_pDirect3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
			
			g_pDirect3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			g_pDirect3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			g_pDirect3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

			g_pDirect3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			g_pDirect3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			g_pDirect3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

			g_pDirect3DDevice->SetStreamSource(0, g_pCurrentPrimitiveDataRendering->m_pVertexBuffer, 0, sizeof(CVertex));
			g_pDirect3DDevice->SetIndices(m_pPrimitiveRenderData.m_pIndicesBuffer);
			g_pDirect3DDevice->SetFVF(CVERTEX_TYPE);
			g_pDirect3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
			g_pDirect3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwFacesCount * 3, 0, m_dwFacesCount);
			g_pDirect3DDevice->SetTexture(0, 0);
			gFacesRendered += m_dwFacesCount;
		}
		return;
	}

	if (!m_pFaces)
		return;

	gFacesRendered++;

	if (flags & 1) // render direction
	{
		for (int i = m_dwFacesCount - 1; i >= 0; i--)
		{
			RenderFace(&m_pFaces[i]);
		}
	}
	else
	{
		for (int i = 0; i < m_dwFacesCount; i++)
		{
			RenderFace(&m_pFaces[i]);
		}
	}
}

void CBlockModelElement::WriteToFile(FILE *f)
{

}