#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <Windows.h>
#include "DX3D.h"
#include "Camera.h"
#include "Model.h"
#include "ColorShader.h"
#include "TextureShader.h"
#include "ObjModel.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Graphics
{
public:
	Graphics();
	~Graphics();
	bool Init(int, int, HWND);
	void Release();
	bool Frame();

private:
	bool Render();
	void Update(float);
	DX3D* dx3d;
	Camera* camera;
	Model* model;
	ColorShader* colorShader;
	TextureShader* textureShader;
	ObjModel* carModel;
};

#endif
