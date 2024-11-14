#ifndef CBLOCKROOM_CLASS_H
#define CBLOCKROOM_CLASS_H

#include "CBlockContainer.h"
#include "CFace.h"

class CBlockRoom : public CBlockContainer
{
public:
	CFace *m_pFaces;
	PrimitiveRenderData field_3C;
	__int16 field_60;
	__int16 materialId;
	int field_64;
	int field_68;
	int field_6C;
	int field_70;
	int field_74;
	int field_78;
	int field_7C;
	CBlockRoom *m_pChildren_;
	char moduleName[32];
	char field_A4[32];
	int field_C4;
	int field_C8;
	int field_CC;
	int field_D0;
	int field_D4;
	int field_D8;
	int field_DC;
	int field_E0;
	int field_E4;
	int field_E8;
	int field_EC;
	int field_F0;
	int field_F4;
	int field_F8;
	int field_FC;
	int field_100;
	int field_104;
	int field_108;
	int field_10C;
	int field_110;
	int field_114[2];
	int someCount;
	double field_120;
	double field_128;
	double field_130;
	double field_138;

	CBlockRoom(const char *name);
	~CBlockRoom();

	virtual int ReadFromFile(FILE *f, void *data1, void *data2);
	virtual void WriteToFile(FILE *f);
};

#endif