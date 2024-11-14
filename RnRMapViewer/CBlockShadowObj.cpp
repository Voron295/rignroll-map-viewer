#include "CBlockShadowObj.h"
#include "CPatch.h"

void CBlockShadowObj::InitHooks()
{
	CPatch::RedirectJump(0x57C360, (void*)&CBlockShadowObj::RenderHook);
	CPatch::RedirectJump(0x5CDF10, (void*)0x5CDF43); // CBlockShadow::Render disable
}

void __fastcall CBlockShadowObj::RenderHook(CBlockShadowObj *block, int nop, int a2, D3DRenderData *a3)
{

}