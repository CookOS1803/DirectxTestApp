#include "Graphics.h"
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <array>
#include <cmath>

Graphics::Graphics(HWND hWnd, int width, int height)
	: camera()
{
	CreateDeviceAndSwapChain(hWnd, width, height);
	CreateRenderTargetView();
	auto format = CreateDepthStencilTexture(width, height);
	CreateDepthStencilView(format);
	pContext->OMSetRenderTargets(1, &pTarget, pDepthStencilView);
	InitializeViewport(width, height);
	auto blob = CompileAndCreateVertexShader();
	DefineAndCreateInputLayout(blob);
	CompileAndCreatePixelShader();
	CreateConstantBuffer();
	InitializeMatrices(width, height);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Graphics::CreateDeviceAndSwapChain(const HWND& hWnd, int width, int height)
{
	DXGI_SWAP_CHAIN_DESC sd {};
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	);
}

void Graphics::CreateRenderTargetView()
{
	ID3D11Resource* pBackBuffer = nullptr;

	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));

	if (pBackBuffer)
	{
		pDevice->CreateRenderTargetView(
			pBackBuffer,
			nullptr,
			&pTarget
		);

		pBackBuffer->Release();
	}
}

DXGI_FORMAT Graphics::CreateDepthStencilTexture(int width, int height)
{
	D3D11_TEXTURE2D_DESC descDepth{};
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	HRESULT hr = pDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	if (FAILED(hr))
		exit(-3);

	return descDepth.Format;
}

void Graphics::CreateDepthStencilView(DXGI_FORMAT format)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};
	descDSV.Format = format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	HRESULT hr = pDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &pDepthStencilView);
	if (FAILED(hr))
		exit(-3);
}

void Graphics::InitializeViewport(int width, int height)
{
	D3D11_VIEWPORT vp{};
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1, &vp);
}

ID3DBlob* Graphics::CompileAndCreateVertexShader()
{
	ID3DBlob* pVSBlob = nullptr;
	HRESULT hr = CompileShaderFromFile(L"Tutorial02.fx", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);

		exit(-1);
	}

	// Create the vertex shader
	hr = pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		exit(-2);
	}

	return pVSBlob;
}

void Graphics::DefineAndCreateInputLayout(ID3DBlob* pVSBlob)
{
	std::array layout =
	{
		D3D11_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		D3D11_INPUT_ELEMENT_DESC{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleVertex::position), D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	
	ID3D11InputLayout* pVertexLayout = nullptr;
	HRESULT hr = pDevice->CreateInputLayout(layout.data(), layout.size(), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr))
		exit(-2);

	pContext->IASetInputLayout(pVertexLayout);
}

void Graphics::CompileAndCreatePixelShader()
{
	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	HRESULT hr = CompileShaderFromFile(L"Tutorial02.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);

		exit(-1);
	}

	// Create the pixel shader
	hr = pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		exit(-2);
}

void Graphics::CreateVertexBuffer(const std::vector<SimpleVertex>& vertices)
{	
	if (pVertexBuffer) pVertexBuffer->Release();

	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData{};
	InitData.pSysMem = vertices.data();
	HRESULT hr = pDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer);
	if (FAILED(hr))
		exit(-3);

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	pContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
}

void Graphics::CreateIndexBuffer(const std::vector<WORD>& indices)
{
	if (pIndexBuffer) pIndexBuffer->Release();

	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData{};
	InitData.pSysMem = indices.data();
	HRESULT hr = pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer);
	if (FAILED(hr))
		exit(-3);

	// Set index buffer
	pContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
}

void Graphics::AddObject(const SceneObject& obj)
{
	objects.push_back(obj);
}

void Graphics::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	HRESULT hr = pDevice->CreateBuffer(&bd, NULL, &pConstantBuffer);
	if (FAILED(hr))
		exit(-3);
}

void Graphics::InitializeMatrices(int width, int height)
{
	worlds.push_back(XMMatrixIdentity());
	worlds.push_back(XMMatrixIdentity());
	worlds.push_back(XMMatrixIdentity());

	camera.SetRotation(0, 0, 0);
	camera.SetPosition(0, 0, -5);

	// Initialize the projection matrix
	projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / (FLOAT)height, 0.01f, 100.0f);
}

Graphics::~Graphics()
{
	if (pDevice) pDevice->Release();
	if (pSwap) pSwap->Release();
	if (pContext) pContext->Release();
	if (pTarget) pTarget->Release();
	if (pVertexBuffer) pVertexBuffer->Release();
	if (pIndexBuffer) pIndexBuffer->Release();
	if (pConstantBuffer) pConstantBuffer->Release();
	if (pDepthStencil) pDepthStencil->Release();
	if (pDepthStencilView) pDepthStencilView->Release();
}

HRESULT Graphics::CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

void Graphics::SetFullscreenState(bool state)
{
	pSwap->SetFullscreenState(state, nullptr);
}

void Graphics::EndFrame()
{
	pSwap->Present(1u, 0u);
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = {red, green, blue, 1.f};
	pContext->ClearRenderTargetView(pTarget, color);
}

void Graphics::Render()
{
	static float t = 0.0f;
	t += (float)XM_PI * 0.0125f;

	const auto spin0 = XMMatrixRotationZ(std::sin(t - XM_PI));
	const auto translation0 = XMMatrixTranslation(0.f, 2 * std::sin(t - XM_PIDIV4), 0.f);
	worlds[0] = spin0 * translation0;

	const auto spin = XMMatrixRotationZ(-t);
	const auto translation1 = XMMatrixTranslation(5 * std::cos(t) - 5.f, 5.f * std::abs(std::sin(t)), 5.f * std::sin(t));
	const auto scale = XMMatrixScaling(0.3f, 0.3f, 0.3f);
	worlds[1] = scale * spin * translation1;

	const auto translation2 = XMMatrixTranslation(5 * std::cos(t) + 5.f, -5.f * std::abs(std::sin(t)), -5.f * std::sin(t));
	worlds[2] = scale * spin * translation2;

	view = XMMatrixLookAtLH(camera.Position(), camera.LookAt(), camera.UpVector());

	ClearBuffer(0, 0, 1);
	pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	pContext->VSSetShader(pVertexShader, NULL, 0);
	pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	pContext->PSSetShader(pPixelShader, NULL, 0);

	for (size_t i = 0; i < objects.size(); i++)
	{
		CreateVertexBuffer(objects[i].GetMesh()->Vertices());
		CreateIndexBuffer(objects[i].GetMesh()->Indices());

		ConstantBuffer cb;
		cb.world = XMMatrixTranspose(worlds[i]);
		cb.view = XMMatrixTranspose(view);
		cb.projection = XMMatrixTranspose(projection);
		pContext->UpdateSubresource(pConstantBuffer, 0, NULL, &cb, 0, 0);

		pContext->DrawIndexed(objects[i].GetMesh()->Indices().size(), 0, 0);
	}
}
