#pragma once
#include "NormWin.h"
#include <d3dx11.h>
#include <vector>
#include <memory>
#include "SimpleVertex.h"
#include "DXDeleter.h"

class Mesh
{
public:

	constexpr Mesh(class Graphics* gfx) : m_vertices(), m_indices(), p_gfx(gfx) {}
	Mesh(const Mesh& other);
	~Mesh();

	constexpr const std::vector<SimpleVertex>& Vertices() const noexcept { return m_vertices; }
	constexpr const std::vector<WORD>& Indices() const noexcept { return m_indices; }
	constexpr ID3D11Buffer* VertexBuffer() const noexcept { return p_vertexBuffer.get(); }
	constexpr ID3D11Buffer* IndexBuffer() const noexcept { return p_indexBuffer.get(); }
	constexpr ID3D11PixelShader* PixelShader() const noexcept { return p_pixelShader; }

	void SetVertices(const std::vector<SimpleVertex>& vertices);
	void SetIndices(const std::vector<WORD>& indices);
	constexpr void SetPixelShader(ID3D11PixelShader* pixelShader) noexcept { p_pixelShader = pixelShader; }

	Mesh& operator=(const Mesh& other);

private:

	Graphics* p_gfx;

	std::vector<SimpleVertex> m_vertices;
	std::vector<WORD> m_indices;

	std::unique_ptr<ID3D11Buffer, DXDeleter<ID3D11Buffer>> p_vertexBuffer = nullptr;
	std::unique_ptr<ID3D11Buffer, DXDeleter<ID3D11Buffer>> p_indexBuffer = nullptr;

	ID3D11PixelShader* p_pixelShader = nullptr;
};

