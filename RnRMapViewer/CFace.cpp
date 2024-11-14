#include "CFace.h"
#include "GameModule.h"

int gFacesCount = 0;

int CFace::ReadFromFile(FILE *f, Vertex *vertices, CVector *normals)
{
	internalFormat = GameModule::ReadIntFromFile(f) ^ 1;
	m_wTexCoordsPerVertexCount = HIBYTE(internalFormat);
	field_1C = GameModule::ReadFloatFromFile(f);
	field_32 = GameModule::ReadIntFromFile(f);
	m_wMaterialId = GameModule::ReadIntFromFile(f);
	m_wVertsCount = GameModule::ReadIntFromFile(f);
	m_dwFaceGlobalId = gFacesCount++;
	m_pVertsIndices = new WORD[m_wVertsCount];
	m_pVertices = 0;

	if (internalFormat & 2)
	{
		++m_wTexCoordsPerVertexCount;
		m_pTexCoordsU = new float[m_wVertsCount * m_wTexCoordsPerVertexCount];
		m_pTexCoordsV = new float[m_wVertsCount * m_wTexCoordsPerVertexCount];
	}
	if (internalFormat & 0x10)
	{
		if (internalFormat & 1)
		{
			m_pVertices = new CVector[m_wVertsCount];
			for (int i = 0; i < m_wVertsCount; i++)
			{
				m_pVertices[i].x = 0;
				m_pVertices[i].y = 0;
				m_pVertices[i].z = 0;
			}
		}
		else
		{
			m_pIntencityValues = new float[m_wVertsCount];
		}
	}

	for (int i = 0; i < m_wVertsCount; i++)
	{
		m_pVertsIndices[i] = GameModule::ReadIntFromFile(f);
		if (internalFormat & 2 && m_wTexCoordsPerVertexCount)
		{
			int offset = i;
			for (int j = 0; j < m_wTexCoordsPerVertexCount; j++)
			{
				m_pTexCoordsU[offset] = GameModule::ReadFloatFromFile(f);
				m_pTexCoordsV[offset] = GameModule::ReadFloatFromFile(f);
				offset += 2;
			}
		}
		
		CVector intencityVector;
		if (internalFormat & 0x10)
		{
			if (internalFormat & 1)
			{
				if (internalFormat & 0x20)
				{
					GameModule::ReadVectorFromFile(f, &intencityVector);
				}
				else 
				{
					intencityVector.x = normals[m_pVertsIndices[i]].x;
					intencityVector.y = normals[m_pVertsIndices[i]].y;
					intencityVector.z = normals[m_pVertsIndices[i]].z;
				}
				m_pVertices[i] = intencityVector;
			}
			else if (internalFormat & 0x20)
			{
				m_pIntencityValues[i] = GameModule::ReadFloatFromFile(f);
			}
		}
	}
	return 1;
}