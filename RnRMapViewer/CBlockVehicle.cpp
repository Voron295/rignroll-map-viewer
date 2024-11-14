#include "CBlockVehicle.h"
#include "CPatch.h"

void CBlockVehicle::InitHooks()
{
	CPatch::RedirectJump(0x4D11C0, (void*)&CBlockVehicle::RenderHook);
	CPatch::RedirectJump(0x546FC0, (void*)0x54707D); // CBlockVehicle1::Render remove
}

void __fastcall CBlockVehicle::RenderHook(CBlockVehicle *block, int nop, int a2, D3DRenderData *a3)
{

}
