#include "CBlockContainer.h"

CSimpleBlock::CSimpleBlock(const char *name, int type)
{
	m_pPrev = 0;
	m_pNext = 0;
	m_vPos.pos.x = .0f;
	m_vPos.pos.y = .0f;
	m_vPos.pos.z = .0f;
	m_vPos.len = .0f;
	if (name && *name)
	{
		m_sName = (char*)malloc(strlen(name) + 1);
		strcpy(m_sName, name);
	}
	else
	{
		m_sName = 0;
	}
	flags = 0;
	m_dwType = type;
	m_pParentBlock = 0;
	m_pModule = 0;
}

CSimpleBlock::~CSimpleBlock()
{
	if (m_sName)
		free(m_sName);
}

CSimpleBlock *CSimpleBlock::GetRoot()
{
	CSimpleBlock *result;
	CSimpleBlock *i;

	result = this;
	for (i = (CSimpleBlock*)this->m_pParentBlock; i; i = (CSimpleBlock*)i->m_pParentBlock)
		result = i;
	return result;
}

int CSimpleBlock::AddBlock(int id, CSimpleBlock *block)
{
	return 1;
}

int CSimpleBlock::ReadFromFile(FILE *f, void *data1, void *data2)
{
	return 1;
}

void CSimpleBlock::WriteToFile(FILE *f)
{

}

void CSimpleBlock::Render()
{
	
}

int CSimpleBlock::RemoveNode()
{
	if (m_pPrev)
	{
		Remove();
		m_pParentBlock = 0;
		m_pModule = 0;
		return 1;
	}
	else
	{
		if (m_pParentBlock)
		{
			int i = 0;
			CBlockContainer *parent = m_pParentBlock;
			for (; i < parent->m_dwSubBlocksCount; i++)
			{
				if (parent->m_pSubBlocks[i] == this)
					break;
			}
			if (i == parent->m_dwSubBlocksCount)
			{
				return 0;
			}
			else
			{
				parent->m_pSubBlocks[i] = (CSimpleBlock *)m_pNext;
				if (m_pNext)
					((Node*)m_pNext)->m_pPrev = 0;
				m_pPrev = 0;
				m_pNext = 0;
				m_pParentBlock = 0;
				return 1;
			}
		}
	}
	return 0;
}

void CSimpleBlock::ScanFunc(int(__cdecl *callback)(CSimpleBlock *))
{
	callback(this);
}