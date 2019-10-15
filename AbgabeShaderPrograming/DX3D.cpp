#include "DX3D.h"

DX3D::DX3D()
{
	swapChain = 0;
	device = 0;
	deviceContext = 0;
	renderTargetView = 0;
	depthStencilBuffer = 0;
	depthStencilState = 0;
	depthStencilView = 0;
	rasterizerState = 0;
}

DX3D::~DX3D()
{
}

bool DX3D::Init(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	HRESULT result;
	
	#pragma region Deklarationsteil

	// Die Factory enthält Methoden um DXGI-Objekte zu erstellen
	IDXGIFactory* factory;

	// Der Adapter repräsentiert Grafikarteneinstellungen
	IDXGIAdapter* adapter;

	// Der Output repräsentiert einen Monitor
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator, stringLength;

	numerator = 0;
	denominator = 0;

	// Datenstruktur für Display-Einstellungen
	DXGI_MODE_DESC* displayModeList;

	// Datenstruktur welche Adapter-Einstellungen enthält
	DXGI_ADAPTER_DESC adaperDesc;

	// Datenstruktur, welche Buffer Swapping Einstellungen enthält
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	// Beschreibung des Feature Levels
	D3D_FEATURE_LEVEL featureLevel;

	// Backbuffer als 2D-Textur
	ID3D11Texture2D* backBuffer;

	// Datenstruktur für die Tiefenbuffer-Textur
	D3D11_TEXTURE2D_DESC depthBufferDesc;

	// Datenstruktur für Stencil-Informationen
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	// Datenstruktur für die Sub-Resourcen der Stencil-Informationen
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	// Datenstruktur für die Konfiguration des Rasterizers
	D3D11_RASTERIZER_DESC rasterizerDesc;

	// Viewport-Dimensionen
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;
	vsyncEnabled = vsync;

	#pragma endregion

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);

	if (FAILED(result)) return false;

	// Gibt die primäre Grafikkarte zurück
	result = factory->EnumAdapters(0, &adapter);

	if (FAILED(result)) return false;

	// Gib die Monitor-Einstellungen zurück
	result = adapter->EnumOutputs(0, &adapterOutput);

	if (FAILED(result)) return false;

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);

	if (FAILED(result)) return false;

	displayModeList = new DXGI_MODE_DESC[numModes];

	if (!displayModeList) return false;

	// Befülle unsere Display Mode List Struktur
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);

	if (FAILED(result)) return false;


	for(i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int) screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int) screenHeight)
			{
				// Bildwiederholfrequenz speichern
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}
	
	result = adapter->GetDesc(&adaperDesc);

	if (FAILED(result)) return false;

	videoCardMemory = (int)(adaperDesc.DedicatedVideoMemory / 1024 / 1024);

	// Alternative zu wcstombs
	// char buffer[128];
	// int error = wcstombs(buffer, adaperDesc.Description, sizeof(buffer));

	int error = wcstombs_s(NULL, videoCardDescription, 128, adaperDesc.Description, 128);

	if (error != 0) return false;
	
	// Releasen / deleten von nicht mehr benötigten Variablen
	delete[] displayModeList;
	displayModeList = 0;

	adapterOutput->Release();
	adapterOutput = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;

	// Swap Chain 
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// 32-Bit Backbuffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (vsyncEnabled)
	{
		// Backbuffer wird sich an der Bildwiederholfrequenz orientieren 
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		// Backbuffer wird schnellstmöglich aktualisieren
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;

	// Multisampling ausschalten
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Microsoft-Empfehlung
	// IDXGISwapChain::SetFullscreenState()

	if (fullscreen) swapChainDesc.Windowed = false;
	else swapChainDesc.Windowed = true;

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	featureLevel = D3D_FEATURE_LEVEL_11_0;

	result = D3D11CreateDeviceAndSwapChain(	NULL, 
											D3D_DRIVER_TYPE_HARDWARE, 
											NULL, 
											0,
											&featureLevel, 
											1, 
											D3D11_SDK_VERSION, 
											&swapChainDesc, 
											&swapChain, 
											&device, 
											NULL, 
											&deviceContext);
	
	if (FAILED(result)) return false;

	result = swapChain->GetBuffer(	0, 
									__uuidof(ID3D11Texture2D),
									(LPVOID*)&backBuffer);

	if (FAILED(result)) return false;

	result = device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);

	if (FAILED(result)) return false;

	backBuffer->Release();
	backBuffer = 0;

	// Tiefenbuffer
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	
	// 24 Bit für die Tiefe und 8 Bit für den Stencil
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;

	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);

	if (FAILED(result)) return false;

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Default Einstellungen für die Stencil Operations, wenn das Frontpixel in Richtung Kamera zeigt
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Default Einstellungen für die Stencil Operations, wenn das Pixel in die entgegengesetze Richtung der Kamera zeigt
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);

	if (FAILED(result)) return false;

	deviceContext->OMSetDepthStencilState(depthStencilState, 1);

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);

	if (FAILED(result)) return false;

	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	result = device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	
	if (FAILED(result)) return false;

	deviceContext->RSSetState(rasterizerState);

	viewport.Width = screenWidth;
	viewport.Height = screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	deviceContext->RSSetViewports(1, &viewport);

	fieldOfView = (float)XM_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Matrizenrechnung

	projectionMatrix = XMMatrixPerspectiveLH(fieldOfView, screenAspect, screenNear, screenDepth);
	worldMatrix = XMMatrixIdentity();
	orthoMatrix = XMMatrixOrthographicLH(screenWidth, screenHeight, screenNear, screenDepth);
	   
	return true;
}

void DX3D::Release()
{
	if (swapChain)
	{
		swapChain->SetFullscreenState(false, NULL);
	}

	if (rasterizerState)
	{
		rasterizerState->Release();
		rasterizerState = 0;
	}

	if (depthStencilState)
	{
		depthStencilState->Release();
		depthStencilState = 0;
	}

	if (depthStencilView)
	{
		depthStencilView->Release();
		depthStencilView = 0;
	}

	if (depthStencilBuffer)
	{				
		depthStencilBuffer->Release();
		depthStencilBuffer = 0;
	}

	if (renderTargetView)
	{
		renderTargetView->Release();
		renderTargetView = 0;
	}

	if (deviceContext)
	{
		deviceContext->Release();
		deviceContext = 0;
	}

	if (device)
	{
		device->Release();
		device = 0;
	}

	if (swapChain)
	{
		swapChain->Release();
		swapChain = 0;
	}
}

void DX3D::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	deviceContext->ClearRenderTargetView(renderTargetView, color);

	deviceContext->ClearDepthStencilView(	depthStencilView, 
											D3D11_CLEAR_DEPTH, 
											1.0f, 
											0);
}

void DX3D::EndScene()
{
	if (vsyncEnabled) swapChain->Present(1, 0);
	else swapChain->Present(0, 0);
}

ID3D11Device * DX3D::GetDevice()
{
	return device;
}

ID3D11DeviceContext * DX3D::GetDeviceContext()
{
	return deviceContext;
}

void DX3D::GetProjectionMatrix(XMMATRIX& _projectionMatrix)
{
	 _projectionMatrix = projectionMatrix;
}

void DX3D::GetWorldMatrix(XMMATRIX& _worldMatrix)
{
	_worldMatrix = worldMatrix;
}

void DX3D::GetOrthoMatrix(XMMATRIX& _orthoMatrix)
{
	_orthoMatrix = orthoMatrix;
}

void DX3D::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, videoCardDescription);
	memory = videoCardMemory;
}
