#ifndef _SYSTEM_H_
#define _SYSTEM_H_

// Beschleunigt den Build indem es die Headergröße reduziert
#define WIN32_LEAN_AND_MEAN

#include "Input.h"
#include "Graphics.h"
#include <Windows.h>

class System
{
public:
	System();
	~System();

	bool Init();
	void Shutdown();
	void Run();
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	bool InitWindows(int& width, int& height);
	void ShutdownWindows();
	LPCWSTR applicationName;
	HWND hwnd;
	HINSTANCE hInstance;

	Input* input;
	Graphics* graphics;

	// Hight Frequency Timer
	float secsPerTick = .0f;
	float deltaTime = .0f;
	float framesPerSeconds = .0f;
	int framesPerSecondCount = 0;
	int framesCounter = 0;
	float frametimer = .0f;
	LARGE_INTEGER oldTicks;

	bool InitHighFrequencyTimer();
	bool UpdateHightFrequencyTimer();
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static System* ApplicationHandle = 0;

#endif
