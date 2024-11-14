#pragma comment(lib, "d3d9.lib")	// Ссылка  на d3d9.lib
#pragma comment(lib, "d3dx9.lib")	// Ссылка  на d3dx9.lib
#pragma comment(lib, "winmm.lib")	// Ссылка  на winmm.lib

#include <windows.h>		// Заголовочный файл  для Windows

#include "CameraManager.h"
#include "ResourceManager.h"

HINSTANCE g_hInstance = NULL;      //Дескриптор приложения
HWND g_hWnd = NULL;            //Дескриптор окна
int g_iWindowWidth = 1280;        //Ширина окна
int g_iWindowHeight = 768;        //Высота окна
bool g_bApplicationState = true;    //Состояние приложения (true - работает/false - не работает)
IDirect3D9 *g_pDirect3D = NULL;      //Интерфейс для создания устройства рендеринга
IDirect3DDevice9 *g_pDirect3DDevice = NULL;  //Интерфейс устройства рендеринга

CameraManager *g_pCamera = NULL;
ResourceManager *g_pResourceManager = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow); //Точка старта приложения
long WINAPI WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);//Обработчик сообщений
bool InitDirect3D(D3DFORMAT ColorFormat, D3DFORMAT DepthFormat);    //Инициализация Direct3D
void DrawFrame();                          //Рисуем кадр
void Shutdown();                          //Освобождаем память
void SetupLight();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	g_hInstance = GetModuleHandle(NULL);

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);        //Размер структуры
	wc.style = CS_HREDRAW | CS_VREDRAW;      //Стили класса окна
	wc.lpfnWndProc = WndProc;              //Функция обработки сообщений
	wc.cbClsExtra = 0;                //Количество выделяемой памяти при создании приложения
	wc.cbWndExtra = 0;                //Количество выделяемой памяти при создании приложения
	wc.hInstance = g_hInstance;            //Дескриптор приложения
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);  //Загружаем стандартную иконку
	wc.hCursor = LoadCursor(0, IDC_ARROW);      //Загружаем стандартный курсор
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//Окно будет закрашено в белый цвет
	wc.lpszMenuName = 0;                //Не используем меню
	wc.lpszClassName = "RnRMapViewer";            //Названия класса
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);  //Загружаем стандартную иконку

	if (!RegisterClassEx(&wc))                //Регистрируем класс в Windows
	{
		Shutdown();                    //Освобождаем память
		MessageBox(NULL, "Can`t register window class", "Error", MB_OK | MB_ICONERROR); //Выводим сообщение
		return 0;                    //Завершаем работу приложения
	}

	RECT theRect;
	theRect.top = 50;
	theRect.left = 50;
	theRect.right = g_iWindowWidth + 50;
	theRect.bottom = g_iWindowHeight + 50;
	AdjustWindowRect(&theRect, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0);


	g_hWnd = CreateWindowEx(              //Создаем окно
		WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,        //Расширенный стиль окна
		"RnRMapViewer",                    //Названия класса окна
		"RnR Map Viewer",    //Названия окна
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,//Стиль окна
		theRect.left,
		theRect.top,
		theRect.right - theRect.left,
		theRect.bottom - theRect.top,
		NULL,                    //Это наше главное окно
		NULL,                    //Нету меню
		g_hInstance,                //Дескриптор приложения
		NULL);                    //Дополнительный настроек не используем

	if (g_hWnd == NULL)                //Если не создали окно
	{
		Shutdown();
		MessageBox(NULL, "Can`t create window", "Error", MB_OK | MB_ICONERROR);//Выводим сообщение
		return 0;                  //Завершаем работу приложения
	}

	if (!InitDirect3D(D3DFMT_R8G8B8, D3DFMT_D24S8))    //Если не смогли инициализировать Direct3D
	{
		Shutdown();
		MessageBox(NULL, "Can`t create direct3d", "Error", MB_OK | MB_ICONERROR);//Выводим сообщение
		return 0;                  //Завершаем работу приложения
	}

	g_pCamera = new CameraManager(g_pDirect3DDevice);
	g_pResourceManager = new ResourceManager(g_pDirect3DDevice);

	SetupLight();

	ShowWindow(g_hWnd, SW_SHOW);            //Отображаем окно  
	UpdateWindow(g_hWnd);              //Обновляем окно
	SetFocus(g_hWnd);                //Устанавливаем фокус на наше окно
	SetForegroundWindow(g_hWnd);          //Устанавливаем приоритет окна выше среднего

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (g_bApplicationState)            //Начинаем бесконечный цикл обработки сообщений
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))//Получаем сообщения
		{
			TranslateMessage(&msg);          //Обрабатываем сообщения
			DispatchMessage(&msg);          //Обрабатываем сообщения
		}
		else
			DrawFrame();              //Если сообщений нету рисуем кадры
	}

	Shutdown();                    //Освобождаем память
	return 0;                    //Завершаем работу приложения
}

long WINAPI WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
		case WM_DESTROY:              //Если получаем сообщение о разрушении окна
		{
			g_bApplicationState = false;      //Устанавливаем состояния приложения в false (это значит что цикл обработки сообщений остановиться)
			return 0;                //Говорим виндовс что мы это сообщение обработали
		}
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);  //Если нету для нас нужных сообщений, пусть это обрабатывает виндовс
}

bool InitDirect3D(D3DFORMAT ColorFormat, D3DFORMAT DepthFormat)
{
	if ((g_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)//Создаем интерфейс Direct3D
		return false;                //Иначе возвращяем false

	D3DPRESENT_PARAMETERS PresParam;        //Структура с помощью которой передаем информацию устройству рендеринга при его создании
	ZeroMemory(&PresParam, sizeof(PresParam));    //Обнуляем

	HRESULT hr = NULL;                //Создаем переменную для записи в неё результатов работы функций

	D3DDISPLAYMODE DisplayMode;            //Структура для получения информации о режиме отображения в виндовс
	hr = g_pDirect3D->GetAdapterDisplayMode(    //Получаем режим отображения
		D3DADAPTER_DEFAULT,              //Используем первичную видеокарту
		&DisplayMode);                //Записываем режим отображения в DisplayMode

	if (FAILED(hr))                  //Если не получилось
		return false;                //Возвращаем false

	PresParam.hDeviceWindow = g_hWnd;        //Дескриптор окна
	PresParam.Windowed = true;            //Оконный режим?
	PresParam.BackBufferWidth = g_iWindowWidth;    //Ширина заднего буфера
	PresParam.BackBufferHeight = g_iWindowHeight;  //Высота заднего буфера
	PresParam.BackBufferCount = 1;          //Количество задних буферов
	PresParam.EnableAutoDepthStencil = true;    //Используем буфер глубины и стенцил буфер
	PresParam.AutoDepthStencilFormat = DepthFormat;  //Формат буфера глубины
	PresParam.SwapEffect = D3DSWAPEFFECT_DISCARD;    //Режим смены кадров
	PresParam.BackBufferFormat = DisplayMode.Format;//Устанавливаем формат пикселя определеный в виндовс
	//PresParam.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	PresParam.FullScreen_RefreshRateInHz = 0;
	PresParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	//PresParam.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;

	hr = g_pDirect3D->CreateDevice(        //Создаем устройство рендеринга
		D3DADAPTER_DEFAULT,              //Используем первичную видеокарту
		D3DDEVTYPE_HAL,                //Устройства рендеринга использует возможности видеокарты
		g_hWnd,                    //Дескриптор окна
		D3DCREATE_HARDWARE_VERTEXPROCESSING,    //Обрабатываем вершинны видеокартой
		&PresParam,                  //Отдаем параметры устройства
		&g_pDirect3DDevice);            //Создаем устройство рендеринга

	if (SUCCEEDED(hr))                //Если получилось 
		return true;                //Возвращяем true

	hr = g_pDirect3D->CreateDevice(          //Создаем устройство рендеринга
		D3DADAPTER_DEFAULT,              //Используем первичную видеокарту
		D3DDEVTYPE_HAL,                //Устройства рендеринга использует возможности видеокарты
		g_hWnd,                    //Дескриптор окна
		D3DCREATE_MIXED_VERTEXPROCESSING,      //Обрабатываем вершинны смешано (видеокартой и процессором)
		&PresParam,                  //Отдаем параметры устройства
		&g_pDirect3DDevice);            //Создаем устройство рендеринга

	if (SUCCEEDED(hr))                //Если получилось
		return true;                //Возвращяем true

	hr = g_pDirect3D->CreateDevice(          //Создаем устройство рендеринга
		D3DADAPTER_DEFAULT,              //Используем первичную видеокарту
		D3DDEVTYPE_HAL,                //Устройства рендеринга использует возможности видеокарты
		g_hWnd,                    //Дескриптор окна
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,    //Обрабатываем вершинны процесором
		&PresParam,                  //Отдаем параметры устройства
		&g_pDirect3DDevice);            //Создаем устройство рендеринга

	if (SUCCEEDED(hr))                //Если получилось
		return true;                //Возвращяем true

	return false;                  //Возвращяем false
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
	HRESULT hr = g_pDirect3DDevice->TestCooperativeLevel();//Проверяем потерял ли Direct3DDevice устройство

	if (hr == D3DERR_DEVICELOST)            //Если да то
		return;                   //Выходи из функции  

	g_pDirect3DDevice->Clear(            //Очищаем задний буфер
		0L,                     //Размер буфера, 0 - весь буфер
		NULL,                     //Область которую будем очищать, NULL - весь буфер
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,     //Чистим задний буфер и буфер глубины
		D3DCOLOR_XRGB(10, 10, 10),            //Цвет в который очищаем задний буфер, в нашем случае черный
		1.0f,                     //Очищаем буфер глубины, заполнив его единицами
		0L);                    //Этот параметрт игнорируеться так как не выстовлен соответсвующий флаг

	g_pDirect3DDevice->BeginScene();        //Начало сцены
	//g_pDirect3DDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, 1);
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pDirect3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	g_pCamera->Process();
	g_pResourceManager->Render();
	g_pDirect3DDevice->EndScene();          //Конец сцены
	g_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);//Отображаем весь задний буфер
}

void Shutdown()
{
	if (g_pDirect3DDevice != NULL)          //Если мы еще не освободили интерфейс рендеринга
	{
		g_pDirect3DDevice->Release();        //То освобождаем его
		g_pDirect3DDevice = NULL;          //И устанавливаем в ноль
	}

	if (g_pDirect3D != NULL)              //Если мы еще не освободили интерфейс d3d
	{
		g_pDirect3D->Release();            //То освобождаем его
		g_pDirect3D = NULL;              //И устанавливаем в ноль
	}

	if (!DestroyWindow(g_hWnd))            //Если не получилось разрушить окно
		g_hWnd = NULL;                //Устанавливаем дескриптор окна в ноль

	if (!UnregisterClass("RnRMapViewer", g_hInstance))  //Если не получилось удалить наше зарегестрированое окно
		g_hInstance = NULL;              //Устанавливаем дескриптор приложения в ноль
}
