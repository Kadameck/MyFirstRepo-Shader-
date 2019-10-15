#include "System.h"

System::System()
{
	input = 0;
	graphics = 0;
}

System::~System()
{
}

bool System::Init()
{
	bool result = true;
	int screenWidth, screenHeight;
	screenWidth, screenHeight = 0;
	
	InitWindows(screenWidth, screenHeight);

	input = new Input;
	if (!input) return false;
	input->Init();

	graphics = new Graphics;
	if (!graphics) return false;

	// result immer false, da nicht deklariert
	result = graphics->Init(screenWidth, screenHeight, hwnd);

	if (!result) return false;
	return true;		
}

void System::Shutdown()
{
	if (graphics)
	{
		graphics->Release();
		delete graphics;
		graphics = 0;
	}

	if (input)
	{
		delete input;
		input = 0;
	}

	ShutdownWindows();
}

void System::Run()
{
	MSG msg;
	bool done, result;

	// Befüllt eine Datenstruktur mit nullen
	ZeroMemory(&msg, sizeof(MSG));

	done = false;

	// Game loop
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			// Windows System Message wird an die Windows Procedure verschickt
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = Frame();
			if (!result) done = true;
		}
	}
}

LRESULT System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		case WM_KEYDOWN:
		{
			input->KeyDown((unsigned int)wparam);
			return 0;
		}
		case WM_KEYUP:
		{
			input->KeyUp((unsigned int)wparam);
			return 0;
		}
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

bool System::Frame()
{
	bool result;

	if (input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	result = graphics->Frame();

	if (!result) return false;

	return true;
}

bool System::InitWindows(int& height, int& width)
{
	WNDCLASSEX wc;
	wc = { };

	// Enthält Informationen über die Umgebung
	DEVMODE dmScreenSettings;
	int posX, posY;
	ApplicationHandle = this;

	// Wir erhalten die Instanz dieser Anwendung
	hInstance = GetModuleHandle(NULL);
	applicationName = L"Meine Erste Engine";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_HAND);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_HAND);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));

		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = width;
		dmScreenSettings.dmPelsHeight = height;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else
	{
		width = 800;
		height = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
				applicationName,
				applicationName,
				WS_CLIPSIBLINGS | WS_CLIPCHILDREN |	WS_POPUP,
				posX,
				posY,
				width,
				height,
				NULL,
				NULL,
				hInstance,
				NULL);

	if (!hwnd)
	{
		DWORD d = GetLastError();
		return false;
	}

	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	ShowCursor(false);

	return true;
}

void System::ShutdownWindows()
{
	ShowCursor(true);

	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(hwnd);
	hwnd = NULL;
	UnregisterClass(applicationName, hInstance);
	hInstance = NULL;
	ApplicationHandle = NULL;
}

bool System::InitHighFrequencyTimer()
{

	return false;
}

bool System::UpdateHightFrequencyTimer()
{
	//LARGE_INTEGER currentTicks = {};
	//QueryPerformanceCounter();
	return false;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
		break;
	}
}
