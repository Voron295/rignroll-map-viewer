#include <malloc.h>
#include "Effect.h"

Effect::Effect()
{
	m_sName1 = 0;
	m_sName2 = 0;
}

Effect::~Effect()
{
	if (m_sName1)
		free(m_sName1);

	if (m_sName2)
		free(m_sName2);
}