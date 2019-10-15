#ifndef _MODEL_H_
#define _MODEL_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include "Texture.h"

using namespace DirectX;

class Model
{
public:
	Model();
	~Model();
	bool Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* filename);

	void Release();
	void Render(ID3D11DeviceContext*);
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};
	bool InitBuffers(ID3D11Device*);
	void ReleaseBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, const char*);

	void ReleaseTexture();
	Texture* texture;

	ID3D11Buffer *vertexBuffer, *indexBuffer;
	int vertexCount, indexCount;
};

#endif
