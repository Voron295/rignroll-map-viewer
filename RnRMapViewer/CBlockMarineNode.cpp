#include "CBlockMarineNode.h"
#include "CPatch.h"

void CBlockMarineNode::InitHooks()
{
	CPatch::RedirectJump(0x60E6D0, (void*)&CBlockMarineNode::RenderHook);
}

void __fastcall CBlockMarineNode::RenderHook(CBlockMarineNode *block, int nop, int a2, D3DRenderData *a3)
{

}