#include "GraphicsFuncs.h"
#include "CVertex.h"

IDirect3DTexture9 *CreateDirect3DTextureFromMemory(IDirect3DDevice9 *device, int width, int height, int depth, char *memory, bool useTransparency)
{
	IDirect3DTexture9 *texture;
	D3DLOCKED_RECT lockedRect;
	HRESULT hr = device->CreateTexture(width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture, 0);
	//logFile->WriteLine("Tex Create Err(%d, %d, %d): 0x%X", width, height, depth, hr);
	if (!SUCCEEDED(hr))
		return 0;

	hr = texture->LockRect(0, &lockedRect, 0, 0);
	if (!SUCCEEDED(hr))
	{
		texture->Release();
		return 0;
	}
	for (int i = 0; i < width * height; i++)
	{
		bool isTransparent = *((BYTE*)memory + i * 3 + 2) == 0 && *((BYTE*)memory + i * 3 + 1) == 0 && *((BYTE*)memory + i * 3) == 0;
		*((BYTE*)lockedRect.pBits + i * 4) = *((BYTE*)memory + i * 3 + 2);
		*((BYTE*)lockedRect.pBits + i * 4 + 1) = *((BYTE*)memory + i * 3 + 1);
		*((BYTE*)lockedRect.pBits + i * 4 + 2) = *((BYTE*)memory + i * 3);
		*((BYTE*)lockedRect.pBits + i * 4 + 3) = isTransparent && useTransparency ? 0 : 255;
	}
	texture->UnlockRect(0);

	return texture;
}


HRESULT RenderLine3D(IDirect3DDevice9 *device, LPD3DXVECTOR3 posStart, LPD3DXVECTOR3 posEnd, D3DCOLOR color)
{
	static CVertex spriteV[] =
	{
		{ 0.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(255, 255, 255), .0f, .0f },
		{ 1.0f, 1.0f, 1.0f, D3DCOLOR_XRGB(255, 255, 255), .0f, .0f }
	};

	spriteV[0].x = posStart->x;
	spriteV[0].y = posStart->y;
	spriteV[0].z = posStart->z;
	spriteV[1].x = posEnd->x;
	spriteV[1].y = posEnd->y;
	spriteV[1].z = posEnd->z;
	spriteV[0].color = color;
	spriteV[1].color = color;

	// устанавливаем формат вершин
	device->SetFVF(CVERTEX_TYPE);
	device->SetTexture(0, 0);

	// Set the source blend state.
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	// Set the destination blend state.

	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);

	HRESULT result = device->DrawPrimitiveUP(D3DPT_LINELIST, 1, spriteV, sizeof(CVertex));

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
	return result;
}


HRESULT RenderLine3D(IDirect3DDevice9 *device, LPD3DXVECTOR3 posStart, LPD3DXVECTOR3 posEnd, D3DCOLOR from, D3DCOLOR to)
{
	static CVertex spriteV[] =
	{
		{ 0.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(255, 255, 255), .0f, .0f },
		{ 1.0f, 1.0f, 1.0f, D3DCOLOR_XRGB(255, 255, 255), .0f, .0f }
	};

	spriteV[0].x = posStart->x;
	spriteV[0].y = posStart->y;
	spriteV[0].z = posStart->z;
	spriteV[1].x = posEnd->x;
	spriteV[1].y = posEnd->y;
	spriteV[1].z = posEnd->z;
	spriteV[0].color = from;
	spriteV[1].color = to;

	// устанавливаем формат вершин
	device->SetFVF(CVERTEX_TYPE);
	device->SetTexture(0, 0);

	// Set the source blend state.
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	// Set the destination blend state.

	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);

	HRESULT result = device->DrawPrimitiveUP(D3DPT_LINELIST, 1, spriteV, sizeof(CVertex));

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
	return result;
}
