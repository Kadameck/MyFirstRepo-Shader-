// #include "Windows.h"
#include "System.h"

// WinMain - EntryPoint unserer Anwendung
// HInstance - Handle zu einer Instanc
// hPrevInstance - Wird seit Win16 nicht mehr benutzt
// pCmdLine - Befehl Commandline
// iCmdShow - Flag für minimized, maximized oder normal
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdShow) 
{
	System* system;
	system = new System;
	if (!system) return 0;

	bool result;

	result = system->Init();

	if (result) system->Run();

	system->Shutdown();
	delete system;
	system = 0;

	return 0;
}

