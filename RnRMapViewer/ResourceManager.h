#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <string>
#include <map>

struct TestVertex
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DCOLOR    color;
};

class ResourceManager 
{
public:
	ResourceManager(IDirect3DDevice9 *pDevice);
	~ResourceManager();

	void Render();
	static void DrawTextString(int x, int y, DWORD color, const char *str);
	void RenderTestCube();

	static void AddBlockToPool(char *name, class CSimpleBlock *block);
	static class CSimpleBlock *GetBlockFromPool(char *name);

private:
	IDirect3DDevice9 *m_pDevice;
	static LPD3DXFONT m_pFont;
	DWORD m_dwLastTick;

	std::vector<class GameModule *> m_pGameModule;
	static std::map<std::string, class CSimpleBlock *> m_pBlocksPool;

	// test cube
	LPDIRECT3DVERTEXBUFFER9 MyVertexBuffer;	// Вертекс буфер  для отрисовки кубика
	LPDIRECT3DINDEXBUFFER9 MyIndexBuffer;	// Индекс  буфер  для отрисовки кубика
};