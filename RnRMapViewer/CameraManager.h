#include <d3d9.h>
#include <d3dx9.h>
#include "CMatrix.h"

class CameraManager 
{
public:
	CameraManager(IDirect3DDevice9 *pDevice);
	~CameraManager();

	void Process();
	void SetDirect3DView();

	void SetPos(float x, float y, float z);
	void SetRot(float x, float y, float z);

private:
	IDirect3DDevice9 *m_pDevice;

	CMatrix m_pMatrix;
	float m_fX;
	float m_fY;
	float m_fZ;

	float m_fTopX;
	float m_fTopY;
	float m_fTopZ;

	float m_fAtX;
	float m_fAtY;
	float m_fAtZ;

	float m_fAngleX;
	float m_fAngleZ;

	bool isClicked;
};