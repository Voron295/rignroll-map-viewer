#pragma comment(lib, "d3d9.lib")	// ������  �� d3d9.lib
#pragma comment(lib, "d3dx9.lib")	// ������  �� d3dx9.lib
#pragma comment(lib, "winmm.lib")	// ������  �� winmm.lib

#include <windows.h>		// ������������ ����  ��� Windows

#include "CameraManager.h"
#include "ResourceManager.h"

HINSTANCE g_hInstance = NULL;      //���������� ����������
HWND g_hWnd = NULL;            //���������� ����
int g_iWindowWidth = 1280;        //������ ����
int g_iWindowHeight = 768;        //������ ����
bool g_bApplicationState = true;    //��������� ���������� (true - ��������/false - �� ��������)
IDirect3D9 *g_pDirect3D = NULL;      //��������� ��� �������� ���������� ����������
IDirect3DDevice9 *g_pDirect3DDevice = NULL;  //��������� ���������� ����������

CameraManager *g_pCamera = NULL;
ResourceManager *g_pResourceManager = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow); //����� ������ ����������
long WINAPI WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);//���������� ���������
bool InitDirect3D(D3DFORMAT ColorFormat, D3DFORMAT DepthFormat);    //������������� Direct3D
void DrawFrame();                          //������ ����
void Shutdown();                          //����������� ������
void SetupLight();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	g_hInstance = GetModuleHandle(NULL);

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);        //������ ���������
	wc.style = CS_HREDRAW | CS_VREDRAW;      //����� ������ ����
	wc.lpfnWndProc = WndProc;              //������� ��������� ���������
	wc.cbClsExtra = 0;                //���������� ���������� ������ ��� �������� ����������
	wc.cbWndExtra = 0;                //���������� ���������� ������ ��� �������� ����������
	wc.hInstance = g_hInstance;            //���������� ����������
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);  //��������� ����������� ������
	wc.hCursor = LoadCursor(0, IDC_ARROW);      //��������� ����������� ������
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//���� ����� ��������� � ����� ����
	wc.lpszMenuName = 0;                //�� ���������� ����
	wc.lpszClassName = "RnRMapViewer";            //�������� ������
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);  //��������� ����������� ������

	if (!RegisterClassEx(&wc))                //������������ ����� � Windows
	{
		Shutdown();                    //����������� ������
		MessageBox(NULL, "Can`t register window class", "Error", MB_OK | MB_ICONERROR); //������� ���������
		return 0;                    //��������� ������ ����������
	}

	RECT theRect;
	theRect.top = 50;
	theRect.left = 50;
	theRect.right = g_iWindowWidth + 50;
	theRect.bottom = g_iWindowHeight + 50;
	AdjustWindowRect(&theRect, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0);


	g_hWnd = CreateWindowEx(              //������� ����
		WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,        //����������� ����� ����
		"RnRMapViewer",                    //�������� ������ ����
		"RnR Map Viewer",    //�������� ����
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,//����� ����
		theRect.left,
		theRect.top,
		theRect.right - theRect.left,
		theRect.bottom - theRect.top,
		NULL,                    //��� ���� ������� ����
		NULL,                    //���� ����
		g_hInstance,                //���������� ����������
		NULL);                    //�������������� �������� �� ����������

	if (g_hWnd == NULL)                //���� �� ������� ����
	{
		Shutdown();
		MessageBox(NULL, "Can`t create window", "Error", MB_OK | MB_ICONERROR);//������� ���������
		return 0;                  //��������� ������ ����������
	}

	if (!InitDirect3D(D3DFMT_R8G8B8, D3DFMT_D24S8))    //���� �� ������ ���������������� Direct3D
	{
		Shutdown();
		MessageBox(NULL, "Can`t create direct3d", "Error", MB_OK | MB_ICONERROR);//������� ���������
		return 0;                  //��������� ������ ����������
	}

	g_pCamera = new CameraManager(g_pDirect3DDevice);
	g_pResourceManager = new ResourceManager(g_pDirect3DDevice);

	SetupLight();

	ShowWindow(g_hWnd, SW_SHOW);            //���������� ����  
	UpdateWindow(g_hWnd);              //��������� ����
	SetFocus(g_hWnd);                //������������� ����� �� ���� ����
	SetForegroundWindow(g_hWnd);          //������������� ��������� ���� ���� ��������

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (g_bApplicationState)            //�������� ����������� ���� ��������� ���������
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))//�������� ���������
		{
			TranslateMessage(&msg);          //������������ ���������
			DispatchMessage(&msg);          //������������ ���������
		}
		else
			DrawFrame();              //���� ��������� ���� ������ �����
	}

	Shutdown();                    //����������� ������
	return 0;                    //��������� ������ ����������
}

long WINAPI WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
		case WM_DESTROY:              //���� �������� ��������� � ���������� ����
		{
			g_bApplicationState = false;      //������������� ��������� ���������� � false (��� ������ ��� ���� ��������� ��������� ������������)
			return 0;                //������� ������� ��� �� ��� ��������� ����������
		}
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);  //���� ���� ��� ��� ������ ���������, ����� ��� ������������ �������
}

bool InitDirect3D(D3DFORMAT ColorFormat, D3DFORMAT DepthFormat)
{
	if ((g_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)//������� ��������� Direct3D
		return false;                //����� ���������� false

	D3DPRESENT_PARAMETERS PresParam;        //��������� � ������� ������� �������� ���������� ���������� ���������� ��� ��� ��������
	ZeroMemory(&PresParam, sizeof(PresParam));    //��������

	HRESULT hr = NULL;                //������� ���������� ��� ������ � �� ����������� ������ �������

	D3DDISPLAYMODE DisplayMode;            //��������� ��� ��������� ���������� � ������ ����������� � �������
	hr = g_pDirect3D->GetAdapterDisplayMode(    //�������� ����� �����������
		D3DADAPTER_DEFAULT,              //���������� ��������� ����������
		&DisplayMode);                //���������� ����� ����������� � DisplayMode

	if (FAILED(hr))                  //���� �� ����������
		return false;                //���������� false

	PresParam.hDeviceWindow = g_hWnd;        //���������� ����
	PresParam.Windowed = true;            //������� �����?
	PresParam.BackBufferWidth = g_iWindowWidth;    //������ ������� ������
	PresParam.BackBufferHeight = g_iWindowHeight;  //������ ������� ������
	PresParam.BackBufferCount = 1;          //���������� ������ �������
	PresParam.EnableAutoDepthStencil = true;    //���������� ����� ������� � ������� �����
	PresParam.AutoDepthStencilFormat = DepthFormat;  //������ ������ �������
	PresParam.SwapEffect = D3DSWAPEFFECT_DISCARD;    //����� ����� ������
	PresParam.BackBufferFormat = DisplayMode.Format;//������������� ������ ������� ����������� � �������
	//PresParam.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	PresParam.FullScreen_RefreshRateInHz = 0;
	PresParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	//PresParam.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;

	hr = g_pDirect3D->CreateDevice(        //������� ���������� ����������
		D3DADAPTER_DEFAULT,              //���������� ��������� ����������
		D3DDEVTYPE_HAL,                //���������� ���������� ���������� ����������� ����������
		g_hWnd,                    //���������� ����
		D3DCREATE_HARDWARE_VERTEXPROCESSING,    //������������ �������� �����������
		&PresParam,                  //������ ��������� ����������
		&g_pDirect3DDevice);            //������� ���������� ����������

	if (SUCCEEDED(hr))                //���� ���������� 
		return true;                //���������� true

	hr = g_pDirect3D->CreateDevice(          //������� ���������� ����������
		D3DADAPTER_DEFAULT,              //���������� ��������� ����������
		D3DDEVTYPE_HAL,                //���������� ���������� ���������� ����������� ����������
		g_hWnd,                    //���������� ����
		D3DCREATE_MIXED_VERTEXPROCESSING,      //������������ �������� ������� (����������� � �����������)
		&PresParam,                  //������ ��������� ����������
		&g_pDirect3DDevice);            //������� ���������� ����������

	if (SUCCEEDED(hr))                //���� ����������
		return true;                //���������� true

	hr = g_pDirect3D->CreateDevice(          //������� ���������� ����������
		D3DADAPTER_DEFAULT,              //���������� ��������� ����������
		D3DDEVTYPE_HAL,                //���������� ���������� ���������� ����������� ����������
		g_hWnd,                    //���������� ����
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,    //������������ �������� ����������
		&PresParam,                  //������ ��������� ����������
		&g_pDirect3DDevice);            //������� ���������� ����������

	if (SUCCEEDED(hr))                //���� ����������
		return true;                //���������� true

	return false;                  //���������� false
}

void SetupLight()
{
	g_pDirect3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_pDirect3DDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(255, 255, 255));
	D3DLIGHT9 light;    // create the light struct

	ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
	light.Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set the light's color
	light.Direction = D3DXVECTOR3(.0f, 1.0f, -1.0f);

	g_pDirect3DDevice->SetLight(0, &light);    // send the light struct properties to light #0
	g_pDirect3DDevice->LightEnable(0, TRUE);    // turn on light #0

	g_pDirect3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	//g_pDirect3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
	
}

void DrawFrame()
{
	HRESULT hr = g_pDirect3DDevice->TestCooperativeLevel();//��������� ������� �� Direct3DDevice ����������

	if (hr == D3DERR_DEVICELOST)            //���� �� ��
		return;                   //������ �� �������  

	g_pDirect3DDevice->Clear(            //������� ������ �����
		0L,                     //������ ������, 0 - ���� �����
		NULL,                     //������� ������� ����� �������, NULL - ���� �����
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,     //������ ������ ����� � ����� �������
		D3DCOLOR_XRGB(10, 10, 10),            //���� � ������� ������� ������ �����, � ����� ������ ������
		1.0f,                     //������� ����� �������, �������� ��� ���������
		0L);                    //���� ��������� ������������� ��� ��� �� ��������� �������������� ����

	g_pDirect3DDevice->BeginScene();        //������ �����
	//g_pDirect3DDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, 1);
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pDirect3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	g_pCamera->Process();
	g_pResourceManager->Render();
	g_pDirect3DDevice->EndScene();          //����� �����
	g_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);//���������� ���� ������ �����
}

void Shutdown()
{
	if (g_pDirect3DDevice != NULL)          //���� �� ��� �� ���������� ��������� ����������
	{
		g_pDirect3DDevice->Release();        //�� ����������� ���
		g_pDirect3DDevice = NULL;          //� ������������� � ����
	}

	if (g_pDirect3D != NULL)              //���� �� ��� �� ���������� ��������� d3d
	{
		g_pDirect3D->Release();            //�� ����������� ���
		g_pDirect3D = NULL;              //� ������������� � ����
	}

	if (!DestroyWindow(g_hWnd))            //���� �� ���������� ��������� ����
		g_hWnd = NULL;                //������������� ���������� ���� � ����

	if (!UnregisterClass("RnRMapViewer", g_hInstance))  //���� �� ���������� ������� ���� ����������������� ����
		g_hInstance = NULL;              //������������� ���������� ���������� � ����
}
