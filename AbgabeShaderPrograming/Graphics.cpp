#include "Graphics.h"
#include "ObjParser.h"
#include "ObjModel.h"

Graphics::Graphics()
{
	dx3d = 0;
	camera = 0;
	model = 0;
	colorShader = 0;
	textureShader = 0;
}

Graphics::~Graphics()
{
}

bool Graphics::Init(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

#pragma region Init Direct X

	dx3d = new DX3D;

	if (!dx3d) return false;

	result = dx3d->Init(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	if (!result)
	{
		MessageBeep(0);
		MessageBox(hwnd, L"Could not init DX3D! Start formatting windows...", L"Error" , MB_OK);

		return false;
	}

#pragma endregion

	camera = new Camera;
	if (!camera) return false;

	// Camera Position
	camera->SetPosition(0.0f, 0.0f, -3.0f);
	camera->SetRotation(0.0f, 0.0f, 0.0f);

	model = new Model;
	if (!model) return false;
	result = model->Init(dx3d->GetDevice(), dx3d->GetDeviceContext(), "data/stone01.tga");

	carModel = new ObjParser(TEXT("data/sphere.obj"));

	result = carModel->Init(dx3d->GetDevice());

	if (!result)
	{
		MessageBox(hwnd, L"Could not init model! Format C please", L"Error", MB_OK);

		return false;
	}

	//colorShader = new ColorShader;
	//result = colorShader->Init(dx3d->GetDevice(), hwnd);

	textureShader = new TextureShader;
	if (!textureShader) return false;

	result = textureShader->Init(dx3d->GetDevice(), hwnd);

	if (!result)
	{
		MessageBox(hwnd, L"Could not init color shader! Remove Chrome from your PC", L"Error", MB_OK);

		return false;
	}

	return true;
}

void Graphics::Release()
{
	if (carModel)
	{
		carModel->Release();
		delete carModel;
		carModel = 0;
	}

	if (camera)
	{
		delete camera;
		camera;
	}

	if (textureShader)
	{
		textureShader->Release();
		delete textureShader;
		textureShader = 0;
	}
	if (dx3d)
	{
		dx3d->Release();
		delete dx3d;
		dx3d = 0;
	}
}

bool Graphics::Frame()
{
	bool result;

	result = Render();

	if (!result) return false;
	return true;
}

bool Graphics::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Erster Layer, Background, Skybox, etc
	dx3d->BeginScene(0.1f, 0.1f, 0.1f, 1.0f);

	camera->Render();

	dx3d->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	dx3d->GetProjectionMatrix(projectionMatrix);

	carModel->Render(dx3d->GetDeviceContext());
	
	// TODO: Matrizen Rechnen/////////////////////////////////////////////////////////////

	//result = colorShader->Render(dx3d-//>GetDeviceContext(), model->GetIndexCount//(), 
	//	worldMatrix, 
	//	viewMatrix, 
	//	projectionMatrix);

	result = textureShader->Render(dx3d->GetDeviceContext(), carModel->GetIndexCount(), 
									worldMatrix, viewMatrix, projectionMatrix, model->GetTexture());
	if (!result) return false;

	dx3d->EndScene();

	return true;
}

void Graphics::Update(float deltaTime)
{
	float z = .1f;
	z += deltaTime * 90.0f;

	if (z >= 360.0f)
	{
		z = -360.0f;
	}

	carModel->SetRotation(XMVectorSet(z *.5f, z, .0f, .0f));
}
