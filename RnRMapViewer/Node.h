#ifndef NODE_CLASS_H
#define NODE_CLASS_H

#include <Windows.h>
#include <iostream>

class Node
{
public:
	void *m_pPrev;
	void *m_pNext;

	Node();
	virtual ~Node();
	virtual Node *Copy(Node *node);
	virtual int WriteChainToFile(FILE *f);
	virtual int ReadChainFromFile(FILE *f);
	virtual int Write();
	Node *InsertAfter(Node *node);
	Node *Remove();
};

#endif