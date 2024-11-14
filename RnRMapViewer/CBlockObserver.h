#ifndef CBLOCKOBSERVER_CLASS_H
#define CBLOCKOBSERVER_CLASS_H

#include "Node.h"
#include "CBlockSpace.h"
#include "CBlockRoom.h"

class CBlockObserver : public Node
{
public:
	char m_sName[32];
	char m_sRoom[32];
	class CBlockRoom *m_pRoomBlock;
	class CBlockSpace *m_pRootBlock;
	float field_54;
	float field_58;
	int field_5C;
	int field_60;
	int m_dwCullDirection;

	virtual int ReadFromFile(FILE *f);
	virtual void WriteToFile(FILE *f);
};

#endif