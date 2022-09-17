#pragma once
#include "NormWin.h"
#include <d3d11.h>
#include <xnamath.h>
#include <initializer_list>
#include <vector>

struct SimpleVertex
{
	SimpleVertex(XMFLOAT3 position, XMFLOAT4 color) : position(position), color(color)
	{
	}

	XMFLOAT3 position;
	XMFLOAT4 color;
};

struct ConstantBuffer
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

class Graphics
{
public:
	Graphics(HWND hWnd, int width, int height);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void SetFullscreenState(bool state);
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;
	void Render();
	void CreateVertexBuffer(std::initializer_list<SimpleVertex> newVertices);
	void CreateIndexBuffer(std::initializer_list<WORD> newIndices);

	static HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

private:
	void CreateDeviceAndSwapChain(const HWND& hWnd, int width, int height);
	void CreateRenderTargetView();
	void InitializeViewport(int width, int height);
	[[nodiscard]] ID3DBlob* CompileAndCreateVertexShader();
	void DefineAndCreateInputLayout(ID3DBlob* pVSBlob);
	void CompileAndCreatePixelShader();
	void CreateConstantBuffer();
	void InitializeMatrices(int width, int height);

	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;
	ID3D11Buffer* pConstantBuffer = nullptr;

	ID3D11VertexShader* pVertexShader = nullptr;
	ID3D11PixelShader* pPixelShader = nullptr;

	std::vector<SimpleVertex> vertices;
	std::vector<WORD> indices;

	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

