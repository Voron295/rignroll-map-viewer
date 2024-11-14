#include "CBlockContainer.h"
#include "CBlock0xD.h"

CBlockContainer::CBlockContainer(const char *name, int type, int childCount)
:CSimpleBlock(name, type)
{
	m_dwSubBlocksCount = childCount;
	if (childCount)
		m_pSubBlocks = (CSimpleBlock**)malloc(4 * childCount);
	else
		m_pSubBlocks = 0;

	for (int i = 0; i < childCount; i++)
		m_pSubBlocks[i] = 0;
}

CBlockContainer::~CBlockContainer()
{
	for (int i = 0; i < m_dwSubBlocksCount; i++)
	{
		if (m_pSubBlocks[i])
		{
			while (true)
			{
				if (!m_pSubBlocks[i]->m_pNext)
					break;
				((Node*)m_pSubBlocks[i]->m_pNext)->Remove();
				delete ((Node*)m_pSubBlocks[i]->m_pNext);
			}
			delete m_pSubBlocks[i];
		}
	}
	if (m_pSubBlocks)
		free(m_pSubBlocks);
}

int CBlockContainer::LoadChildren(FILE *f, int expectedCount, int realCount, void *data1, void *data2)
{
	int flag;
	if (realCount != m_dwSubBlocksCount)
	{
		m_dwSubBlocksCount = realCount;
		free(m_pSubBlocks);
		m_pSubBlocks = (CSimpleBlock **)malloc(4 * m_dwSubBlocksCount);
		for (int i = 0; i < m_dwSubBlocksCount; i++)
			m_pSubBlocks[i] = 0;
	}
	for (int i = 0; i < m_dwSubBlocksCount; i++)
	{
		CSimpleBlock *parentBlock = 0;
		if (m_dwType != 7 || i != 1)
		{
			while (true)
			{
				if (!expectedCount && i == m_dwSubBlocksCount - 1)
					return 1;

				flag = GameModule::ReadIntFromFile(f);
				if (flag != 333)
					break;

				int blockType;
				CSimpleBlock *block = GameModule::LoadBlock(f, m_pModule, blockType, data1, data2);
				if (!block)
					return 0;

				--expectedCount;
				if (GameModule::ReadIntFromFile(f) != 555)
					return 0;

				if (block->m_dwType == 9 && block->m_sName && !strncmp(block->m_sName, "way", 3) && ((CBlock0xD *)block)->data.type == 7)
				{
					
					// some code is not copied due to unnecessary
				}
				else
				{
					if (block->m_dwType != 2 || *((CBlockContainer*)block)->m_pSubBlocks)
					{
						if (parentBlock)
						{
							block->m_pParentBlock = parentBlock->m_pParentBlock;
							block->m_pModule = parentBlock->m_pModule;
							block->InsertAfter(parentBlock);
							parentBlock = block;
						}
						else
						{
							AddBlock(i, block);
							parentBlock = block;
						}
					}
					else
					{
						delete block;
					}
				}
			}
			if (flag != 444)
				return 0;
		}
	}
	return expectedCount == 0;
}

int CBlockContainer::AddBlock(int id, CSimpleBlock *block)
{
	if (id >= m_dwSubBlocksCount)
		return 0;
	block->m_pParentBlock = this;
	block->m_pModule = m_pModule;
	if (m_pSubBlocks[id])
	{
		block->m_pNext = m_pSubBlocks[id];
		block->m_pPrev = 0;
		m_pSubBlocks[id]->m_pPrev = block;
		m_pSubBlocks[id] = block;
	}
	else
	{
		m_pSubBlocks[id] = block;
		block->m_pPrev = 0;
		block->m_pNext = 0;
	}
	return 1;
}

void CBlockContainer::ScanFunc(int(__cdecl *callback)(CSimpleBlock *))
{
	int id = callback(this);
	if (id < 0) // scan for all blocks
	{
		for (int i = 0; i < m_dwSubBlocksCount; i++)
		{
			CSimpleBlock *next = 0;
			CSimpleBlock *current = m_pSubBlocks[i];
			if (current)
			{
				do
				{
					next = (CSimpleBlock *)current->m_pNext;
					current->ScanFunc(callback);
					// temporarily remove this part
					/*if (current->flags & 4)
					{
					current->RemoveNode();
					delete current;
					}*/

					current = next;
				} while (next);
			}
		}
	}
	else // scan block by id
	{
		CSimpleBlock *next = 0;
		CSimpleBlock *current = m_pSubBlocks[id];
		if (current)
		{
			do
			{
				next = (CSimpleBlock *)current->m_pNext;
				current->ScanFunc(callback);
				// temporarily remove this part
				/*if (current->flags & 4)
				{
				current->RemoveNode();
				delete current;
				}*/

				current = next;
			} while (next);
		}
	}
}

void CBlockContainer::RenderChildren(int id, int flags)
{
	if (id < m_dwSubBlocksCount && id >= 0)
	{
		CSimpleBlock *block = m_pSubBlocks[id];
		if (block)
		{
			if (flags & 1) // from last to first
			{
				for (auto i = block->m_pNext; i; i = ((Node*)i)->m_pNext)
					block = (CSimpleBlock *)i;

				if (block)
				{
					do
					{
						block->Render();
						block = (CSimpleBlock *)block->m_pPrev;
					} while (block);
				}
			}
			else
			{
				do
				{
					block->Render();
					block = (CSimpleBlock *)block->m_pNext;
				} while (block);
			}
		}
	}
}

void CBlockContainer::Render()
{
	RenderChildren(0, 0);
}