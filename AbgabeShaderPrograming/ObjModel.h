#ifndef _OBJMODEL_H_
#define _OBJMODEL_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include "Vertex.h"

class ObjModel
{
public:
	ObjModel();
	~ObjModel();

	bool Init(ID3D11Device*);
	virtual void Update(float deltaTime);
	void Render(ID3D11DeviceContext*);
	void Release();

	// Genauso als würe dam das return norma ans ende der methode schreiben
	UINT GetIndexCount() { return indexCount; };

	void SetPosition(DirectX::FXMVECTOR _position);
	void SetRotation(DirectX::FXMVECTOR _rotation);
	void SetScale(DirectX::FXMVECTOR _scale);

	DirectX::XMMATRIX GetWorldMatrix();

protected:
	bool InitVertexBuffer(ID3D11Device* device);
	bool InitIndexBuffer(ID3D11Device* device);

	virtual bool CreateVertexArray() = 0;
	virtual bool CreateIndexArray() = 0;

	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;

	int vertexCount = 0;
	int indexCount = 0;

	UINT* indexArray = nullptr;
	Vertex* vertexArray = nullptr;

	// Transform
	DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };

	DirectX::XMFLOAT4X4 worldMatrix;
};
#endif

