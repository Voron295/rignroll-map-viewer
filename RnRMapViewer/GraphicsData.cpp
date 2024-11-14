#include "GraphicsData.h"

GraphicsPalette *GraphicsData::GetPalette()
{
	GraphicsData *a1 = this;
	for (GraphicsData *i = a1->parent; i; i = i->parent)
		a1 = i;
	return a1->m_pPalette;
}