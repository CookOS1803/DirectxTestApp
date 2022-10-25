#pragma once
#include "NormWin.h"
#include <vector>
#include "Camera.h"
#include "SimpleVertex.h"
#include "SceneObject.h"
#include "DXDeleter.h"
#include "Timer.h"
#include <SpriteFont.h>

struct ConstantBuffer
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
	DirectX::XMFLOAT4 outputColor;
};

struct VertexConstantBuffer
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

struct PixelConstantBuffer
{
	DirectX::XMFLOAT4 ambientlLight;
	DirectX::XMFLOAT4 directionalLight;
	DirectX::XMFLOAT4 lightDirection;
};

class Graphics
{

public:
	Graphics(HWND hWnd, int width, int height);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	void SetFullscreenState(bool state);
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;
	void Render(float t);
	void DrawText();
	[[nodiscard]] ID3D11Buffer* CreateVertexBuffer(const std::vector<SimpleVertex>& newVertices);
	[[nodiscard]] ID3D11Buffer* CreateIndexBuffer(const std::vector<WORD>& newIndices);
	void Draw(const SceneObject& obj);
	void DrawUI(const SceneObject& obj);
	
	constexpr Camera& GetCamera() noexcept {return camera;}

	static HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	ID3D11PixelShader* CompileAndCreatePixelShader(std::wstring_view fileName, std::string_view shaderName, std::string_view shaderVersion);

private:
	void CreateDeviceAndSwapChain(const HWND& hWnd, int width, int height);
	void CreateRenderTargetView();
	[[nodiscard]] DXGI_FORMAT CreateDepthStencilTexture(int width, int height);
	void CreateDepthStencilView(DXGI_FORMAT format);
	void CreateTexture();
	void InitializeViewport(int width, int height);
	[[nodiscard]] ID3DBlob* CompileAndCreateVertexShader();
	void DefineAndCreateInputLayout(ID3DBlob* pVSBlob);
	void CreateConstantBuffer();
	void InitializeMatrices(int width, int height);
	void DrawOld(const SceneObject& o, DirectX::XMMATRIX v, DirectX::XMMATRIX proj);

	std::unique_ptr<ID3D11Device, DXDeleter<ID3D11Device>> pDevice = nullptr;
	std::unique_ptr<IDXGISwapChain, DXDeleter<IDXGISwapChain>> pSwap = nullptr;
	std::unique_ptr<ID3D11DeviceContext, DXDeleter<ID3D11DeviceContext>> pContext = nullptr;
	std::unique_ptr<ID3D11RenderTargetView, DXDeleter<ID3D11RenderTargetView>> pTarget = nullptr;
	std::unique_ptr<ID3D11Buffer, DXDeleter<ID3D11Buffer>> pVertexConstantBuffer = nullptr;
	std::unique_ptr<ID3D11Buffer, DXDeleter<ID3D11Buffer>> pPixelConstantBuffer = nullptr;
	std::unique_ptr<ID3D11Texture2D, DXDeleter<ID3D11Texture2D>> pDepthStencil = nullptr;
	std::unique_ptr<ID3D11DepthStencilView, DXDeleter<ID3D11DepthStencilView>> pDepthStencilView = nullptr;
	std::unique_ptr<ID3D11ShaderResourceView, DXDeleter<ID3D11ShaderResourceView>> pTextureRV = nullptr;
	std::unique_ptr<ID3D11SamplerState, DXDeleter<ID3D11SamplerState>> pSamplerLinear = nullptr;
	ID3D11VertexShader* pVertexShader = nullptr;
	ID3D11InputLayout* pVertexLayout = nullptr;

	DirectX::XMMATRIX view;
	DirectX::XMMATRIX uiView;
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX uiProjection;

	Camera camera;
	Camera uiCamera;

	DirectX::XMFLOAT4 currentLightDir;

	std::unique_ptr<DirectX::SpriteFont> m_font = nullptr;
	DirectX::XMVECTOR m_fontPos;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	D3D11_VIEWPORT viewport;
};

