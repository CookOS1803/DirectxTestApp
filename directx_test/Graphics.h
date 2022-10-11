#pragma once
#include "NormWin.h"
#include <d3d11.h>
#include <xnamath.h>
#include <vector>
#include "Camera.h"
#include "SimpleVertex.h"
#include "SceneObject.h"
#include "DXDeleter.h"
#include "Timer.h"

struct ConstantBuffer
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
	XMFLOAT4 lightDir[2];
	XMFLOAT4 lightColor[2];
	XMFLOAT4 outputColor;
};

struct VertexConstantBuffer
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

struct PixelConstantBuffer
{
	XMFLOAT4 ambientlLight;
	XMFLOAT4 directionalLight;
	XMFLOAT4 lightDirection;
};

class Graphics
{
	struct GraphicObject
	{
		SceneObject* obj = nullptr;
		XMMATRIX world;

		GraphicObject(SceneObject* obj, XMMATRIX world)
			: obj(obj), world(world) {}
	};

public:
	Graphics(HWND hWnd, int width, int height);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	void SetFullscreenState(bool state);
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;
	void Render();
	[[nodiscard]] ID3D11Buffer* CreateVertexBuffer(const std::vector<SimpleVertex>& newVertices);
	[[nodiscard]] ID3D11Buffer* CreateIndexBuffer(const std::vector<WORD>& newIndices);
	void AddObject(SceneObject* obj);
	void AddUIObject(SceneObject* obj);
	
	constexpr Camera& GetCamera() noexcept {return camera;}

	static HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

private:
	void CreateDeviceAndSwapChain(const HWND& hWnd, int width, int height);
	void CreateRenderTargetView();
	[[nodiscard]] DXGI_FORMAT CreateDepthStencilTexture(int width, int height);
	void CreateDepthStencilView(DXGI_FORMAT format);
	void InitializeViewport(int width, int height);
	[[nodiscard]] ID3DBlob* CompileAndCreateVertexShader();
	void DefineAndCreateInputLayout(ID3DBlob* pVSBlob);
	void CompileAndCreatePixelShader();
	void CreateConstantBuffer();
	void InitializeMatrices(int width, int height);
	void Draw(const Graphics::GraphicObject& o, ID3D11PixelShader* ps, XMMATRIX proj);

	std::unique_ptr<ID3D11Device, DXDeleter<ID3D11Device>> pDevice = nullptr;
	std::unique_ptr<IDXGISwapChain, DXDeleter<IDXGISwapChain>> pSwap = nullptr;
	std::unique_ptr<ID3D11DeviceContext, DXDeleter<ID3D11DeviceContext>> pContext = nullptr;
	std::unique_ptr<ID3D11RenderTargetView, DXDeleter<ID3D11RenderTargetView>> pTarget = nullptr;
	std::unique_ptr<ID3D11Buffer, DXDeleter<ID3D11Buffer>> pVertexConstantBuffer = nullptr;
	std::unique_ptr<ID3D11Buffer, DXDeleter<ID3D11Buffer>> pPixelConstantBuffer = nullptr;
	std::unique_ptr<ID3D11Texture2D, DXDeleter<ID3D11Texture2D>> pDepthStencil = nullptr;
	std::unique_ptr<ID3D11DepthStencilView, DXDeleter<ID3D11DepthStencilView>> pDepthStencilView = nullptr;
	ID3D11VertexShader* pVertexShader = nullptr;
	ID3D11PixelShader* pPixelShader = nullptr;
	ID3D11PixelShader* pPixelShaderSolid = nullptr;

	XMMATRIX view;
	XMMATRIX projection;
	XMMATRIX uiProjection;

	Camera camera;
	Camera uiCamera;

	std::vector<GraphicObject> objects;
	std::vector<GraphicObject> uiObjects;

	XMFLOAT4 currentLightDir;

	Timer timer;
};

