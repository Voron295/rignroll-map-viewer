#ifndef CBLOCK0X28_CLASS_H
#define CBLOCK0X28_CLASS_H

#include "CBlock0xD.h"

class CBlock0x28 : public CSimpleBlock
{
public:
	int(__cdecl *callback)(DWORD, DWORD, DWORD);
	int field_34;
	int field_38;
	CBlock0xDSub field_3C;
};

#endif