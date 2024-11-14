#ifndef VERTEX_STRUCT_H
#define VERTEX_STRUCT_H

#include "CVector.h"

struct UV
{
	float u;
	float v;
};

struct Vertex
{
	CVector pos;
	float u;
	float v;
	UV *m_pUVs;
	char uvCount;
};


#endif