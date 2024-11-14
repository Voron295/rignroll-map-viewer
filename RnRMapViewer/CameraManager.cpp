#include "CameraManager.h"
#include <Windows.h>

extern HWND g_hWnd;

#include <d3d9.h>
#include <d3dx9.h>
#include "CVertex.h"

CameraManager::CameraManager(IDirect3DDevice9 *pDevice)
{
	m_pDevice = pDevice;
	SetPos(0, -5.0f, 3.0f);
}

CameraManager::~CameraManager()
{

}

extern int g_iWindowWidth;
extern int g_iWindowHeight;

void CameraManager::Process()
{
	if (GetForegroundWindow() != g_hWnd)
		return;

	int offsetX = 0;
	int offsetY = 0;
	POINT pt;

	if (isClicked)
	{
		if ((GetKeyState(VK_LBUTTON) & 0x8000) == 0)
		{
			isClicked = false;
			ShowCursor(1);
		}

	}
	else
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			GetCursorPos(&pt);
			ScreenToClient(g_hWnd, &pt);
			if (pt.x >= 0 && pt.y >= 0 && pt.x <= g_iWindowWidth && pt.y <= g_iWindowHeight)
			{
				isClicked = true;
				pt.x = g_iWindowWidth / 2;
				pt.y = g_iWindowHeight / 2;
				ClientToScreen(g_hWnd, &pt);
				SetCursorPos(pt.x, pt.y);
				SetFocus(g_hWnd);
				ShowCursor(0);
			}
		}
	}

	if (isClicked)
	{
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);
		offsetX = pt.x - g_iWindowWidth / 2;
		offsetY = pt.y - g_iWindowHeight / 2;
		pt.x = g_iWindowWidth / 2;
		pt.y = g_iWindowHeight / 2;
		ClientToScreen(g_hWnd, &pt);
		SetCursorPos(pt.x, pt.y);
	}

	if (isClicked)
	{

		float x = 0;
		float y = 0;
		float z = 0;
		if (GetKeyState('W') & 0x8000)
		{
			y += 1;
		}
		if (GetKeyState('S') & 0x8000)
		{
			y -= 1;
		}
		if (GetKeyState('A') & 0x8000)
		{
			x -= 1;
		}
		if (GetKeyState('D') & 0x8000)
		{
			x += 1;
		}
		if (GetKeyState('E') & 0x8000)
		{
			z += 1;
		}
		if (GetKeyState('Q') & 0x8000)
		{
			z -= 1;
		}
		if (GetKeyState(VK_SHIFT) & 0x8000)
		{
			x *= 5;
			y *= 5;
			z *= 5;
		}
		if (GetKeyState(VK_CONTROL) & 0x8000)
		{
			x /= 20;
			y /= 20;
			z /= 20;
		}

		m_fAngleZ -= offsetX / 4.f;
		for (; m_fAngleZ < 0.f; m_fAngleZ += 360.f);
		for (; m_fAngleZ > 360.f; m_fAngleZ -= 360.f);

		m_fAngleX -= offsetY / 4.f;
		for (; m_fAngleX < 0.f; m_fAngleX += 360.f);
		for (; m_fAngleX > 360.f; m_fAngleX -= 360.f);

		SetRot(m_fAngleX, 0.0, m_fAngleZ);
		SetPos(m_fX + x * m_pMatrix.m_fTopY + y * m_pMatrix.m_fTopX, m_fY - x * m_pMatrix.m_fTopX + y * m_pMatrix.m_fTopY, m_fZ + z);
	}
	SetDirect3DView();
}

void CameraManager::SetPos(float x, float y, float z)
{
	m_fX = x;
	m_fY = y;
	m_fZ = z;
}

void CameraManager::SetRot(float x, float y, float z)
{
	/*x = x * PI / 180.f;
	y = y * PI / 180.f;
	z = z * PI / 180.f;

	m_fTopX = cosf(z);
	m_fTopY = sinf(z);
	m_fTopZ = 0;

	m_fAtX = 0;
	m_fAtY = 0;
	m_fAtZ = 1;*/
	m_pMatrix.Rotate(x, y, z);
	//m_fTopX = m_pMatrix->m_fTopX;
}

void CameraManager::SetDirect3DView()
{
	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	D3DXMatrixLookAtRH(&matView,
		&D3DXVECTOR3(m_fX, m_fY, m_fZ),  // Точка  из которой мы смотрим
		&D3DXVECTOR3(m_fX + m_pMatrix.m_fTopX, m_fY + m_pMatrix.m_fTopY, m_fZ + m_pMatrix.m_fTopZ),  // Точка  в которую смотрим
		&D3DXVECTOR3(m_pMatrix.m_fAtX, m_pMatrix.m_fAtY, m_pMatrix.m_fAtZ)); // Где верх?
	m_pDevice->SetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixPerspectiveFovRH(&matProj, (float)D3DX_PI/2, (float)g_iWindowWidth / (float)g_iWindowHeight, 0.5f, 5000.0f);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	/*RenderLine3D(m_pDevice, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(matView._11, matView._12, matView._13), D3DCOLOR_XRGB(255, 0, 0));
	RenderLine3D(m_pDevice, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(matView._21, matView._22, matView._23), D3DCOLOR_XRGB(0, 255, 0));
	RenderLine3D(m_pDevice, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(matView._21, matView._22, matView._23), D3DCOLOR_XRGB(0, 0, 255));*/
}