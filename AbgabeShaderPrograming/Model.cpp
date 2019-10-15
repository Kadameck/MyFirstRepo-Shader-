#include "Model.h"

Model::Model()
{
	vertexBuffer = 0;
	indexBuffer = 0;
	texture = 0;
}

Model::~Model()
{
}

bool Model::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* filename)
{
	bool result;

	result = InitBuffers(device);

	if (!result) return false;

	result = LoadTexture(device, deviceContext, filename);

	if (!result) return false;

	return true;
}

void Model::Release()
{
	ReleaseTexture();
	ReleaseBuffers();
}

void Model::Render(ID3D11DeviceContext * deviceContext)
{
	RenderBuffers(deviceContext);
}

int Model::GetIndexCount()
{
	return indexCount;
}

bool Model::InitBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	HRESULT result;

	vertexCount = 3;
	indexCount = 3;

	vertices = new VertexType[vertexCount];
	if (!vertices) return false;

	indices = new unsigned long[indexCount];
	if (!indices) return false;

	// Assemble triangle

	// Unten links
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);

	// Oben mitte
	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[1].texture = XMFLOAT2(0.5f, 0.0f);

	// Unten rechts
	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertices[2].texture = XMFLOAT2(1.0f, 1.0f);

	indices[0] = 0; // Unten links
	indices[1] = 1; // Oben mitte
	indices[2] = 2; // Unten rechts

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	if (FAILED(result)) return false;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Dieser Kommt ist nur zum test
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

	if (FAILED(result)) return false;

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void Model::ReleaseBuffers()
{
	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = 0;
	}

	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = 0;
	}
}

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(	0, 
										1, 
										&vertexBuffer, 
										&stride, 
										&offset);

	deviceContext->IASetIndexBuffer(	indexBuffer,
										DXGI_FORMAT_R32_UINT,
										0);

	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

ID3D11ShaderResourceView * Model::GetTexture()
{
	return texture->GetTexture();
}

bool Model::LoadTexture(ID3D11Device * device, ID3D11DeviceContext * deviceContext,const char * filename)
{
	bool result;

	texture = new Texture;
	if (!texture) return false;
	
	result = texture->Init(device, deviceContext, filename);

	if (!result) return false;

	return true;
}

void Model::ReleaseTexture()
{
	if (texture)
	{
		texture->Release();
		delete texture;
		texture = 0;
	}
}
