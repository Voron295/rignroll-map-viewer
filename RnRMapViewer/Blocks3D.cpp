#include "Blocks3D.h"
#include "CPatch.h"
#include "CBlockVehicle.h"
#include "CBlockContainerEnv.h"
#include "CBlock0x1C.h"
#include "CBlock0x1F.h"
#include "CBlock0x22.h"
#include "CBlockMarineNode.h"
#include "CBlockModelElement.h"
#include "CBlockContainer0x5.h"
#include "CBlockContainer0x6.h"
#include "CBlockContainer0x7.h"
#include "CBlockShadowObj.h"

void Blocks3D::InitHooks()
{
	CBlockContainerEnv::InitHooks();
	CBlockVehicle::InitHooks();
	CBlock0x1C::InitHooks();
	CBlock0x1F::InitHooks();
	CBlock0x22::InitHooks();
	CBlockModelElement::InitHooks();
	CBlockMarineNode::InitHooks();
	CBlockContainer0x5::InitHooks();
	CBlockShadowObj::InitHooks();
	CBlockContainer0x6::InitHooks();
	CBlockContainer0x7::InitHooks();

	CPatch::RedirectJump(0x4BCC00, (void*)0x4BCD04); // CBlockContainerTDTN
	CPatch::RedirectJump(0x4C0DA0, (void*)0x4C0DBC); // CBlockSnow
	CPatch::RedirectJump(0x4C2840, (void*)0x4C285C); // CBlockDrops
	CPatch::RedirectJump(0x4C2A30, (void*)0x4C2A4C); // CBlockDrops
	CPatch::RedirectJump(0x4C8A50, (void*)0x4C8A64); // CBlockReflection
	CPatch::RedirectJump(0x4CCA50, (void*)0x4CCB15); // CBlockPeople
}