#include "Node.h"

Node::Node()
{
	m_pNext = 0;
	m_pPrev = 0;
}

Node::~Node()
{

}

Node *Node::Copy(Node *node)
{
	return node;
	//return ((Node *(__thiscall*)(Node *, Node *))0x5D2230)(this, node);
}

int Node::WriteChainToFile(FILE *f)
{
	return 1;
	//return ((int(__thiscall*)(Node *, FILE *))0x5D2150)(this, f);
}

int Node::ReadChainFromFile(FILE *f)
{
	return 1;
	//return ((int(__thiscall*)(Node *, FILE *))0x5D21A0)(this, f);
}

int Node::Write()
{
	return 1;
	//return ((int(__thiscall*)(Node *))0x5D2270)(this);
}

Node *Node::InsertAfter(Node *next)
{
	m_pPrev = next;
	Node *result = (Node*)next->m_pNext;
	m_pNext = result;
	if (result)
		result->m_pPrev = this;
	next->m_pNext = this;
	return result;
}

Node *Node::Remove()
{
	Node *v1 = (Node*)m_pPrev;
	if (m_pPrev)
		v1->m_pNext = m_pNext;
	Node *result = (Node*)m_pNext;
	if (result)
		result->m_pPrev = m_pPrev;
	m_pPrev = 0;
	m_pNext = 0;
	return result;
}