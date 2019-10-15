#ifndef _COLORSHADER_H_
#define _COLORSHADER_H_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

#pragma comment(lib,"D3dcompiler.lib")

using namespace DirectX;
using namespace std;

class ColorShader
{
public:
	ColorShader();
	~ColorShader();
	bool Init(ID3D11Device*, HWND);
	void Release();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	ID3D11Buffer* matrixBuffer;
	
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
	bool InitShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);
	void ReleaseShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);
	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);



};

#endif