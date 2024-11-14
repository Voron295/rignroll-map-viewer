#include <d3d9.h>
#include <d3dx9.h>

IDirect3DTexture9 *CreateDirect3DTextureFromMemory(IDirect3DDevice9 *device, int width, int height, int depth, char *memory, bool useTransparency);
HRESULT RenderLine3D(IDirect3DDevice9 *device, LPD3DXVECTOR3 posStart, LPD3DXVECTOR3 posEnd, D3DCOLOR color);
HRESULT RenderLine3D(IDirect3DDevice9 *device, LPD3DXVECTOR3 posStart, LPD3DXVECTOR3 posEnd, D3DCOLOR from, D3DCOLOR to);