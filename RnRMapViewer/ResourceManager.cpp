#include "ResourceManager.h"
#include "GameModule.h"
#include "GraphicsFuncs.h"

std::map<std::string, CSimpleBlock *> ResourceManager::m_pBlocksPool;
LPD3DXFONT ResourceManager::m_pFont;

void ResourceManager::DrawTextString(int x, int y, DWORD color, const char *str)
{
	RECT TextRect = { x, y, 0, 0 };
	m_pFont->DrawText(NULL, str, -1, &TextRect, DT_CALCRECT, color);
	m_pFont->DrawText(NULL, str, -1, &TextRect, DT_LEFT, color);
}

IDirect3DTexture9 *g_tex1;
IDirect3DTexture9 *g_tex2;
IDirect3DTexture9 *g_tex3;
IDirect3DTexture9 *g_tex4;

ResourceManager::ResourceManager(IDirect3DDevice9 *pDevice)
{
	m_pDevice = pDevice;
	m_dwLastTick = GetTickCount();

	char red[3] = { 255, 0, 0 };
	char blue[3] = { 0, 0, 255};
	char green[3] = { 0, 255, 0 };
	char gray[3] = { 100, 100, 100 };
	g_tex1 = CreateDirect3DTextureFromMemory(pDevice, 1, 1, 24, red, 0);
	g_tex2 = CreateDirect3DTextureFromMemory(pDevice, 1, 1, 24, blue, 0);
	g_tex3 = CreateDirect3DTextureFromMemory(pDevice, 1, 1, 24, green, 0);
	g_tex4 = CreateDirect3DTextureFromMemory(pDevice, 1, 1, 24, gray, 0);

	HRESULT r = 0;
	// init debug font
	r = D3DXCreateFont(m_pDevice, 16,
		0,
		0,
		1,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		"Tahoma",
		&m_pFont);

	if (FAILED(r))
		return;

	if (m_pDevice->CreateVertexBuffer(
		8 * sizeof(TestVertex),	// Размер буфера  (в нашем случае 8 вершин
		// каждая размером  в структуру MyVertex)
		D3DUSAGE_WRITEONLY,	// Используем буфер только  для чтения
		D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE,	// Формат наших вершин
		D3DPOOL_MANAGED,	// DirectX сам управляет пересозданием буфера
		&MyVertexBuffer,	// Наш указатель  на буфер
		NULL		// Оставим NULL
		))
		return;		// Если произошла ошибка сообщить об этом

	if (m_pDevice->CreateIndexBuffer(
		36 * sizeof(short),	// Размер буфера индексов  (в нашем случае
		// 36 индексов каждый размером  в short)
		D3DUSAGE_WRITEONLY,	// Используем буфер только  для чтения
		D3DFMT_INDEX16, 	// Формат индексов  (для short - D3DFMT_INDEX16
		//  для DWORD - D3DFMT_INDEX32)
		D3DPOOL_MANAGED,	// DirectX сам управляет пересозданием буфера
		&MyIndexBuffer,	// Наш указатель  на буфер
		NULL		// Оставим NULL
		))
		return;

	TestVertex v[] =
	{
		{ { -1.0f, -1.0f, -1.0f }, { -1.0f, -1.0f, -1.0f }, D3DCOLOR_XRGB(255, 0, 0), },
		{ { -1.0f, 1.0f, -1.0f }, { -1.0f, 1.0f, -1.0f }, D3DCOLOR_XRGB(0, 255, 0), },
		{ { 1.0f, -1.0f, -1.0f }, { 1.0f, -1.0f, -1.0f }, D3DCOLOR_XRGB(0, 0, 255), },
		{ { 1.0f, 1.0f, -1.0f }, { 1.0f, 1.0f, -1.0f }, D3DCOLOR_XRGB(255, 255, 0), },
		{ { 1.0f, -1.0f, 1.0f }, { 1.0f, -1.0f, 1.0f }, D3DCOLOR_XRGB(255, 0, 255), },
		{ { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, D3DCOLOR_XRGB(0, 255, 255), },
		{ { -1.0f, -1.0f, 1.0f }, { -1.0f, -1.0f, 1.0f }, D3DCOLOR_XRGB(0, 0, 0), },
		{ { -1.0f, 1.0f, 1.0f }, { -1.0f, 1.0f, 1.0f }, D3DCOLOR_XRGB(255, 255, 255), }
	};

	void * pBuf;
	// заполненяем вершинный буфер
	MyVertexBuffer->Lock(0, 8 * sizeof(TestVertex), &pBuf, 0);
	memcpy(pBuf, v, 8 * sizeof(TestVertex));
	MyVertexBuffer->Unlock();

	short inds[36] =
	{
		0, 1, 2,
		2, 1, 3,
		2, 3, 4,
		4, 3, 5,
		4, 5, 6,
		6, 5, 7,
		6, 7, 0,
		0, 7, 1,
		1, 7, 3,
		3, 7, 5,
		0, 2, 6,
		2, 4, 6 };

	// заполненяем индексный буфер
	MyIndexBuffer->Lock(0, 36 * sizeof(short), &pBuf, 0);
	memcpy(pBuf, inds, 36 * sizeof(short));
	MyIndexBuffer->Unlock();

	char Path[500];
	char tmp[500];
	int count = 0;
	
	WIN32_FIND_DATA ffd;
	memset(&ffd, 0, sizeof(ffd));
	GetCurrentDirectory(500, Path);
	strcpy(tmp, Path);
	strcat(tmp, "\\ENV\\*.b3d");
	HANDLE handle = FindFirstFile(tmp, &ffd);
	if (handle != INVALID_HANDLE_VALUE)
	{
		ffd.cFileName[strlen(ffd.cFileName) - 4] = 0;
		GameModule *module = new GameModule();
		module->Load("ENV\\", ffd.cFileName);
		m_pGameModule.push_back(module);
	}
	while (FindNextFile(handle, &ffd))
	{
		ffd.cFileName[strlen(ffd.cFileName) - 4] = 0;
		GameModule *module = new GameModule();
		module->Load("ENV\\", ffd.cFileName);
		m_pGameModule.push_back(module);
	}
	FindClose(handle);
}

ResourceManager::~ResourceManager()
{
	m_pFont->Release();
}

struct CVertexRHW
{
	float	x, y, z, rhw;
	DWORD	color;
	float	tx, ty;
};

HRESULT RenderQuadPrimitive(IDirect3DDevice9 *device, IDirect3DTexture9 *texture, LPD3DXVECTOR2 posStart, LPD3DXVECTOR2 posEnd, LPD3DXVECTOR2 uvStart, LPD3DXVECTOR2 uvEnd, D3DCOLOR color, bool useAlpha)
{
	static CVertexRHW spriteV[] =
	{
		{ 0.0f, 0.0f, 1.0f, 1.0f, color, 0.0f, 0.0f, },
		{ 1.0f, 0.0f, 1.0f, 1.0f, color, 1.0f, 0.0f, },
		{ 0.0f, 1.0f, 1.0f, 1.0f, color, 0.0f, 1.0f, },
		{ 1.0f, 1.0f, 1.0f, 1.0f, color, 1.0f, 1.0f, },
	};

	static LPDIRECT3DVERTEXBUFFER9 MyVertexBuffer = 0;
	static LPDIRECT3DINDEXBUFFER9 MyIndexBuffer = 0;

	short inds[6] =
	{
		0, 1, 2,
		0, 2, 3
	};
	spriteV[0].x = posStart->x;
	spriteV[0].y = posStart->y;
	spriteV[1].x = posEnd->x;
	spriteV[1].y = posStart->y;
	spriteV[3].x = posEnd->x;
	spriteV[3].y = posEnd->y;
	spriteV[2].x = posStart->x;
	spriteV[2].y = posEnd->y;

	spriteV[0].tx = uvStart->x;
	spriteV[0].ty = uvStart->y;
	spriteV[1].tx = uvEnd->x;
	spriteV[1].ty = uvStart->y;
	spriteV[3].tx = uvEnd->x;
	spriteV[3].ty = uvEnd->y;
	spriteV[2].tx = uvStart->x;
	spriteV[2].ty = uvEnd->y;
	// устанавливаем формат вершин
	device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	device->SetTexture(0, texture);

	//device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	//device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	//device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	if (useAlpha)
	{
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
	}
	else {
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTCOLOR);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
	}

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
	DWORD culling;
	device->GetRenderState(D3DRS_CULLMODE, &culling);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	HRESULT result = device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, spriteV, sizeof(CVertexRHW));

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
	device->SetRenderState(D3DRS_CULLMODE, culling);
	return result;
}

HRESULT RenderLinePrimitive(IDirect3DDevice9 *device, LPD3DXVECTOR2 posStart, LPD3DXVECTOR2 posEnd, D3DCOLOR color)
{
	static CVertexRHW spriteV[] =
	{
		{ 0.0f, 0.0f, 1.0f, 1.0f, D3DCOLOR_XRGB(255, 255, 255) },
		{ 1.0f, 1.0f, 1.0f, 1.0f, D3DCOLOR_XRGB(255, 255, 255) }
	};

	spriteV[0].x = posStart->x;
	spriteV[0].y = posStart->y;
	spriteV[1].x = posEnd->x;
	spriteV[1].y = posEnd->y;
	spriteV[0].color = color;
	spriteV[1].color = color;

	// устанавливаем формат вершин
	device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	device->SetTexture(0, 0);

	// Set the source blend state.
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	// Set the destination blend state.

	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);

	HRESULT result = device->DrawPrimitiveUP(D3DPT_LINELIST, 1, spriteV, sizeof(CVertexRHW));

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
	return result;
}

void ResourceManager::RenderTestCube()
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_pDevice->SetStreamSource(0, MyVertexBuffer, 0, sizeof(TestVertex));
	m_pDevice->SetIndices(MyIndexBuffer);
	// устанавливаем формат вершин
	m_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	// Производим отрисовку
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}

extern int gFacesRendered;

void ResourceManager::Render()
{
	//RenderTestCube();
	gFacesRendered = 0;
	for (auto it = m_pGameModule.begin(); it != m_pGameModule.end(); it++)
		(*it)->Render();
	/*
	Texture *tex = m_pGameModule[0]->m_pTextures[0];
	RenderQuadPrimitive(m_pDevice, tex->m_pSurface, &D3DXVECTOR2(0, 0), &D3DXVECTOR2(tex->m_iWidth, tex->m_iHeight), &D3DXVECTOR2(0, 0), &D3DXVECTOR2(1, 1), D3DCOLOR_XRGB(255, 255, 255), true);


	sprintf(debugStr, "Width: %d, Height: %d, Depth: %d, Type: %d", tex->m_iWidth, tex->m_iHeight, tex->m_bDepth, tex->m_bFormat);
	DrawTextString(0, 100, D3DCOLOR_XRGB(255, 255, 255), debugStr);
	*/
	// debug info
	char debugStr[512];
	int tick = GetTickCount() - m_dwLastTick;
	if (tick <= 0)
		tick = 1;
	sprintf(debugStr, "FPS: %d\r\nFaces: %d", 1000/tick, gFacesRendered);
	DrawTextString(0, 0, D3DCOLOR_XRGB(255, 255, 255), debugStr);
	m_dwLastTick = GetTickCount();
}

void ResourceManager::AddBlockToPool(char *name, CSimpleBlock *block)
{
	m_pBlocksPool[name] = block;
}

CSimpleBlock *ResourceManager::GetBlockFromPool(char *name)
{
	auto block = m_pBlocksPool.find(name);
	if (block != m_pBlocksPool.end())
		return block->second;
	
	return 0;
}