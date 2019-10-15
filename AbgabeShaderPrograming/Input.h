#ifndef _INPUT_H_
#define _INPUT_H_

class Input
{
public:
	Input();
	Input(const Input&);
	~Input();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);
	bool IsKeyDown(unsigned int);

	void Init();
	void Release();

private: 
	bool keys[256];
};

#endif 