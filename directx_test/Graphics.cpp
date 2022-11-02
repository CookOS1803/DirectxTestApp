#include "Graphics.h"
#include <d3dcompiler.h>
#include <array>
#include <cmath>
#include <D3DX11tex.h>
#include <DirectXColors.h>

Graphics::Graphics(HWND hWnd, int width, int height)
	: camera(), uiCamera()
{
	CreateDeviceAndSwapChain(hWnd, width, height);
	CreateRenderTargetView();
	auto format = CreateDepthStencilTexture(width, height);
	CreateDepthStencilView(format);
	auto tempTarget = pTarget.get();
	pContext->OMSetRenderTargets(1, &tempTarget, pDepthStencilView.get());
	InitializeViewport(width, height);
	auto blob = CompileAndCreateVertexShader();
	DefineAndCreateInputLayout(blob);
	CreateConstantBuffer();
	InitializeMatrices(width, height);
	CreateTexture();


	m_font = std::make_unique<DirectX::SpriteFont>(pDevice.get(), L"myfile.spritefont");
	m_fontPos = { 600.f, 600.f };
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(pContext.get());



	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	D3D11_RASTERIZER_DESC cmdesc{};
	cmdesc.CullMode = D3D11_CULL_NONE;
	ID3D11RasterizerState* rs;
	pDevice->CreateRasterizerState(&cmdesc, &rs);
	RSCullNone.reset(rs);

	ID3D11DepthStencilState* ds = nullptr;
	pDevice->CreateDepthStencilState(&dssDesc, &ds);
	DSLessEqual.reset(ds);
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

	ID3D11Device* tempDevice = nullptr;
	IDXGISwapChain* tempSwap = nullptr;
	ID3D11DeviceContext* tempContext = nullptr;

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&tempSwap,
		&tempDevice,
		nullptr,
		&tempContext
	);
	
	pDevice.reset(tempDevice);
	pSwap.reset(tempSwap);
	pContext.reset(tempContext);
}

void Graphics::CreateRenderTargetView()
{
	ID3D11Resource* pBackBuffer = nullptr;

	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));

	if (pBackBuffer)
	{
		ID3D11RenderTargetView* tempTarget = nullptr;

		pDevice->CreateRenderTargetView(
			pBackBuffer,
			nullptr,
			&tempTarget
		);
		
		pTarget.reset(tempTarget);

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

	ID3D11Texture2D* tempds = nullptr;
	HRESULT hr = pDevice->CreateTexture2D(&descDepth, NULL, &tempds);
	if (FAILED(hr))
		exit(-3);

	pDepthStencil.reset(tempds);

	return descDepth.Format;
}

void Graphics::CreateDepthStencilView(DXGI_FORMAT format)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};
	descDSV.Format = format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	ID3D11DepthStencilView* tempsv = nullptr;
	HRESULT hr = pDevice->CreateDepthStencilView(pDepthStencil.get(), &descDSV, &tempsv);
	if (FAILED(hr))
		exit(-3);

	pDepthStencilView.reset(tempsv);
}

void Graphics::CreateTexture()
{
	ID3D11ShaderResourceView* tempRV = nullptr;
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(pDevice.get(), L"brick.dds", NULL, NULL, &tempRV, NULL);
	if (FAILED(hr))
		exit(-1);

	pTextureRV.reset(tempRV);
	
	// Create the sample state
	ID3D11SamplerState* tempSampler = nullptr;
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = pDevice->CreateSamplerState(&sampDesc, &tempSampler);
	if (FAILED(hr))
		exit(-1);

	pSamplerLinear.reset(tempSampler);


	////

	D3DX11_IMAGE_LOAD_INFO loadInfo;
	loadInfo.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_TEXTURECUBE;
	ID3D11Texture2D* tempSky = nullptr;
	hr = D3DX11CreateTextureFromFile(pDevice.get(), L"cubemap.dds", &loadInfo, NULL, (ID3D11Resource**)&tempSky, NULL);
	if (FAILED(hr))
		throw 1;
	D3D11_TEXTURE2D_DESC texDesc {};
	tempSky->GetDesc(&texDesc);
	D3D11_SHADER_RESOURCE_VIEW_DESC shadDesc;
	shadDesc.Format = texDesc.Format;
	shadDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURECUBE;
	shadDesc.TextureCube.MipLevels = texDesc.MipLevels;
	shadDesc.TextureCube.MostDetailedMip = 0;
	tempRV = nullptr;
	hr = pDevice->CreateShaderResourceView(tempSky, &shadDesc, &tempRV);
	tempSky->Release();
	if (FAILED(hr))
		throw 1;

	pSkyView.reset(tempRV);
}

void Graphics::InitializeViewport(int width, int height)
{
	viewport.Width = (FLOAT)width;
	viewport.Height = (FLOAT)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	pContext->RSSetViewports(1, &viewport);
}

ID3DBlob* Graphics::CompileAndCreateVertexShader()
{
	ID3DBlob* pVSBlob = nullptr;
	HRESULT hr = CompileShaderFromFile(L"Light.fx", "VS", "vs_5_0", &pVSBlob);
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

	////

	ID3DBlob* bbb = nullptr;
	hr = CompileShaderFromFile(L"Light.fx", "SKYMAP_VS", "vs_5_0", &bbb);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);

		exit(-1);
	}

	// Create the vertex shader
	hr = pDevice->CreateVertexShader(bbb->GetBufferPointer(), bbb->GetBufferSize(), NULL, &skyVS);
	if (FAILED(hr))
	{
		bbb->Release();
		exit(-2);
	}

	return pVSBlob;
}

void Graphics::DefineAndCreateInputLayout(ID3DBlob* pVSBlob)
{
	std::array layout =
	{
		D3D11_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		D3D11_INPUT_ELEMENT_DESC{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(SimpleVertex::position), D3D11_INPUT_PER_VERTEX_DATA, 0},
		D3D11_INPUT_ELEMENT_DESC{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(SimpleVertex::position) + sizeof(SimpleVertex::color), D3D11_INPUT_PER_VERTEX_DATA, 0},
		D3D11_INPUT_ELEMENT_DESC{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(SimpleVertex::position) + sizeof(SimpleVertex::color) + sizeof(SimpleVertex::normal), D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	
	HRESULT hr = pDevice->CreateInputLayout(layout.data(), layout.size(), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr))
		exit(-2);

}

ID3D11PixelShader* Graphics::CompileAndCreatePixelShader(std::wstring_view fileName, std::string_view shaderName, std::string_view shaderVersion)
{
	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	HRESULT hr = CompileShaderFromFile(fileName.data(), shaderName.data(), shaderVersion.data(), &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);

		exit(-1);
	}

	// Create the pixel shader
	hr = pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &pixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		exit(-2);

	return pixelShader;
}

ID3D11Buffer* Graphics::CreateVertexBuffer(const std::vector<SimpleVertex>& vertices)
{	
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData{};
	InitData.pSysMem = vertices.data();
	ID3D11Buffer* pVertexBuffer = nullptr;
	HRESULT hr = pDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer);
	if (FAILED(hr))
		exit(-3);

	return pVertexBuffer;
}

ID3D11Buffer* Graphics::CreateIndexBuffer(const std::vector<UINT>& indices)
{
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT) * indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData{};
	InitData.pSysMem = indices.data();
	ID3D11Buffer* pIndexBuffer = nullptr;
	HRESULT hr = pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer);
	if (FAILED(hr))
		exit(-3);
	
	return pIndexBuffer;

}


void Graphics::Draw(const SceneObject& obj)
{
	DrawOld(obj, view, projection);
}

void Graphics::DrawUI(const SceneObject& obj)
{
	DrawOld(obj, uiView, uiProjection);
}

void Graphics::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VertexConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	ID3D11Buffer* tempcb = nullptr;
	HRESULT hr = pDevice->CreateBuffer(&bd, NULL, &tempcb);
	if (FAILED(hr))
		exit(-3);

	pVertexConstantBuffer.reset(tempcb);

	tempcb = nullptr;
	bd.ByteWidth = sizeof(PixelConstantBuffer);

	hr = pDevice->CreateBuffer(&bd, NULL, &tempcb);
	if (FAILED(hr))
		exit(-3);

	pPixelConstantBuffer.reset(tempcb);
}

void Graphics::InitializeMatrices(int width, int height)
{
	camera.SetRotation(0, 0, 0);
	camera.SetPosition(0, 0, -5);
	uiCamera.SetRotation(0, 0, 0);
	uiCamera.SetPosition(0, 0, -5);

	// Initialize the projection matrix
	projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, width / (FLOAT)height, 0.01f, 1000.0f);
	uiProjection = DirectX::XMMatrixOrthographicLH(width / 100, height / 100, 0.01f, 100.0f);
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
	pContext->ClearRenderTargetView(pTarget.get(), color);
}

void Graphics::Render(float t)
{
	//static float t = 0.0f;
	//t += timer.Mark();
	//
	//const auto spin0 = XMMatrixRotationZ(std::sin(t - XM_PI));
	//const auto translation0 = XMMatrixTranslation(0.f, 2 * std::sin(t - XM_PIDIV4), 0.f);
    //objects[0].world = spin0 * translation0;
	//
	//const auto spin = XMMatrixRotationZ(-t);
	//const auto translation1 = XMMatrixTranslation(5 * std::cos(t) - 5.f, 5.f * std::abs(std::sin(t)), 5.f * std::sin(t));
	//const auto scale = XMMatrixScaling(0.3f, 0.3f, 0.3f);
	//objects[1].world = scale * spin * translation1;
	//
	//const auto translation2 = XMMatrixTranslation(5 * std::cos(t) + 5.f, -5.f * std::abs(std::sin(t)), -5.f * std::sin(t));
	//objects[2].world = scale * spin * translation2;
	//
	//uiObjects[0].world = XMMatrixRotationRollPitchYaw(XM_PIDIV2, 0, 0) * XMMatrixTranslation(6, -3, 0);
	//
	//
	currentLightDir.x = std::sin(t);
	currentLightDir.y = -1.f;
	currentLightDir.z = std::cos(t);

	ClearBuffer(0.5, 0.5, 0.5);
	pContext->ClearDepthStencilView(pDepthStencilView.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	auto tempcb = pVertexConstantBuffer.get();
	pContext->VSSetConstantBuffers(1, 1, &tempcb);

	tempcb = pPixelConstantBuffer.get();
	pContext->PSSetConstantBuffers(0, 1, &tempcb);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->IASetInputLayout(pVertexLayout);
	pContext->RSSetViewports(1, &viewport);

	auto tempRV = pTextureRV.get();
	auto tempSky = pSkyView.get();
	auto tempSampler = pSamplerLinear.get();
	pContext->PSSetShaderResources(0, 1, &tempRV);
	pContext->PSSetShaderResources(1, 1, &tempSky);
	pContext->PSSetSamplers(0, 1, &tempSampler);

	uiView = DirectX::XMMatrixLookAtLH(uiCamera.Position(), uiCamera.LookAt(), uiCamera.UpVector());
	view = DirectX::XMMatrixLookAtLH(camera.Position(), camera.LookAt(), camera.UpVector());

	VertexConstantBuffer vcb{};
	vcb.world = DirectX::XMMatrixTranspose(DirectX::XMMatrixScaling(10, 10, 10) * DirectX::XMMatrixTranslationFromVector(camera.Position()));
	vcb.view = DirectX::XMMatrixTranspose(view);
	vcb.projection = DirectX::XMMatrixTranspose(projection);
	pContext->UpdateSubresource(pVertexConstantBuffer.get(), 0, NULL, &vcb, 0, 0);
	pContext->VSSetShader(skyVS, NULL, 0);
	pContext->PSSetShader(skyPS, NULL, 0);
	pContext->OMSetDepthStencilState(DSLessEqual.get(), 0);
	pContext->RSSetState(RSCullNone.get());
	pContext->DrawIndexed(skyMesh->Indices().size(), 0, 0);

	pContext->VSSetShader(pVertexShader, NULL, 0);
	pContext->OMSetDepthStencilState(NULL, 0);
}

void Graphics::DrawText()
{
	ID3D11DepthStencilState* st = nullptr;
	UINT sten = 0;
	pContext->OMGetDepthStencilState(&st, &sten);

	m_spriteBatch->Begin();

	const auto output = L"Sample Text";

	const auto origin = m_font->MeasureString(output);
	
	m_font->DrawString(m_spriteBatch.get(), output,
		m_fontPos, DirectX::Colors::White, 0.f, origin);

	m_spriteBatch->End();

	pContext->OMSetDepthStencilState(st, sten);
}

void Graphics::DrawOld(const SceneObject& o, DirectX::XMMATRIX v, DirectX::XMMATRIX proj)
{
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	auto vb = o.GetMesh()->VertexBuffer();
	pContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	pContext->IASetIndexBuffer(o.GetMesh()->IndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	const auto pos = o.GetTransform().position;
	const auto rot = o.GetTransform().eulerRotation;
	const auto sc = o.GetTransform().scale;

	const auto world = DirectX::XMMatrixScaling(sc.x, sc.y, sc.z) * DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

	VertexConstantBuffer vcb{};
	vcb.world = DirectX::XMMatrixTranspose(world);
	vcb.view = DirectX::XMMatrixTranspose(v);
	vcb.projection = DirectX::XMMatrixTranspose(proj);
	pContext->UpdateSubresource(pVertexConstantBuffer.get(), 0, NULL, &vcb, 0, 0);

	PixelConstantBuffer pcb{};
	const auto al = .2f;
	pcb.ambientlLight = { al, al, al, 1.f };
	pcb.directionalLight = { 1.f, 1.f, 1.f, 1.f };
	pcb.lightDirection = currentLightDir;
	pContext->UpdateSubresource(pPixelConstantBuffer.get(), 0, NULL, &pcb, 0, 0);

	pContext->PSSetShader(o.GetMeshRenderer().PixelShader(), NULL, 0);
	pContext->DrawIndexed(o.GetMesh()->Indices().size(), 0, 0);
}

