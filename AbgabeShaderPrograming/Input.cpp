#include "Input.h"

Input::Input()
{
}

Input::Input(const Input &)
{
}

Input::~Input()
{
}

void Input::KeyDown(unsigned int input)
{
	keys[input] = true;
}

void Input::KeyUp(unsigned int input)
{
	keys[input] = false;
}

bool Input::IsKeyDown(unsigned int input)
{
	return keys[input];
}

void Input::Init()
{
	int i;

	for (i = 0; i < 256; i++)
	{
		keys[i] = false;
	}
}

void Input::Release()
{
	// Können hier Objekte löschen
}