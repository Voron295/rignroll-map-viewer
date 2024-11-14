#ifndef CBlockRoomDesc_CLASS_H
#define CBlockRoomDesc_CLASS_H

#include "CSimpleBlock.h"
#include "CMatrix.h"

class CBlockRoomDesc : public CSimpleBlock
{
public:
	CVector field_30;
	CVector m_pPoints[4];
	int field_6C;
	CVector4 field_70;
	int field_80;
	char field_84[20];
	int field_98;
	int field_9C;
	int field_A0;
	char m_sRoomName[20];
	int field_B8;
	int field_BC;
	int field_C0;

	CVector m_vStartPos;
	CVector m_vEndPos;

	CBlockRoomDesc(const char *name);
	~CBlockRoomDesc();

	virtual int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
	virtual void Render();
};

#endif