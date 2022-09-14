#pragma once
#include "NormWin.h"
#include <d3d11.h>
#include <xnamath.h>
#include <initializer_list>
#include <vector>

struct SimpleVertex
{
	SimpleVertex(float x, float y, float z) : position(x, y, z)
	{
	}

	XMFLOAT3 position;
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

	static HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

private:
	void CreateDeviceAndSwapChain(const HWND& hWnd);
	void CreateRenderTargetView();
	void InitializeViewport(int width, int height);
	[[nodiscard]] ID3DBlob* CompileAndCreateVertexShader();
	void DefineAndCreateInputLayout(ID3DBlob* pVSBlob);
	void CompileAndCreatePixelShader();

	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;

	ID3D11VertexShader* pVertexShader = nullptr;
	ID3D11PixelShader* pPixelShader = nullptr;

	std::vector<SimpleVertex> vertices;
};

