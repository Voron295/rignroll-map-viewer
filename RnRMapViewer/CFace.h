#ifndef CFACE_CLASS_H
#define CFACE_CLASS_H

#include <Windows.h>
#include <iostream>
#include "CVertex.h"
#include "Vertex.h"
#include "CVector4.h"
#include <d3d9.h>
#include <d3dx9.h>

struct PrimitiveRenderData
{
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9 m_pIndicesBuffer;
};

class CFace
{
public:
	__int16 internalFormat;
	__int16 m_dwFaceGlobalId;
	__int16 m_wVertsCount;
	__int16 m_wTexCoordsPerVertexCount;
	WORD *m_pVertsIndices;
	float *m_pTexCoordsU;
	float *m_pTexCoordsV;
	float *m_pIntencityValues;
	CVector *m_pVertices;
	float field_1C;
	CVector4 m_FaceNormal;
	__int16 m_wMaterialId;
	__int16 field_32;
	PrimitiveRenderData m_pPrimitiveRenderData;

	int ReadFromFile(FILE *f, Vertex *vertices, CVector *normals);
};

#endif