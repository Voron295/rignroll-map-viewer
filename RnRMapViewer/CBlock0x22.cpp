#include "CBlock0x22.h"
#include "CPatch.h"

void CBlock0x22::InitHooks()
{
	CPatch::RedirectJump(0x5FA960, (void*)&CBlock0x22::RenderHook);
}

void __fastcall CBlock0x22::RenderHook(CBlock0x22 *block, int nop, int a2, D3DRenderData *a3)
{

}