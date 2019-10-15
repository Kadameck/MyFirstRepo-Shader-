#include "ObjModel.h"



ObjModel::ObjModel()
{
}


ObjModel::~ObjModel()
{
}

bool ObjModel::Init(ID3D11Device* device)
{
	if (!InitVertexBuffer(device)) return false;

	if (!InitIndexBuffer(device)) return false;

	return true;
}

void ObjModel::Update(float deltaTime)
{
}

void ObjModel::Render(ID3D11DeviceContext* deviceContext)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void ObjModel::Release()
{
	if (vertexArray) 
	{
		delete[] vertexArray;
		vertexArray = nullptr;
	}

	if (indexArray)
	{
		delete[] indexArray;
		indexArray = nullptr;
	}
}

void ObjModel::SetPosition(DirectX::FXMVECTOR _position)
{
	DirectX::XMStoreFloat3(&position, _position);
}

void ObjModel::SetRotation(DirectX::FXMVECTOR _rotation)
{
	DirectX::XMStoreFloat3(&rotation, _rotation);
}

void ObjModel::SetScale(DirectX::FXMVECTOR _scale)
{
	DirectX::XMStoreFloat3(&scale, _scale);
}

DirectX::XMMATRIX ObjModel::GetWorldMatrix()
{
	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(
		DirectX::XMConvertToRadians(rotation.x), 
		DirectX::XMConvertToRadians(rotation.y), 
		DirectX::XMConvertToRadians(rotation.z));

	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	return DirectX::XMMATRIX();
}

bool ObjModel::InitVertexBuffer(ID3D11Device* device)
{
	if (!CreateVertexArray()) return false;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.ByteWidth = sizeof(Vertex) * vertexCount;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	
	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = vertexArray;
	
	HRESULT hResult;
	hResult = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);

	if (FAILED(hResult)) return false;


	return true;
}

bool ObjModel::InitIndexBuffer(ID3D11Device * device)
{
	if (!CreateIndexArray()) return false;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.ByteWidth = sizeof(UINT) * indexCount;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = indexArray;

	HRESULT hResult;
	hResult = device->CreateBuffer(&bufferDesc, &data, &indexBuffer);

	if (FAILED(hResult)) return false;


	return true;
}
