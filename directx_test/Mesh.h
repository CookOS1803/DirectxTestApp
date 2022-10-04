#pragma once
#include "NormWin.h"
#include <vector>
#include "SimpleVertex.h"

struct ID3D11Buffer;

class Mesh
{
public:

	constexpr Mesh(class Graphics* gfx) : vertices(), indices(), pGfx(gfx) {}
	Mesh(const Mesh& other);
	~Mesh();

	constexpr const std::vector<SimpleVertex>& Vertices() const noexcept { return vertices; }
	constexpr const std::vector<WORD>& Indices() const noexcept { return indices; }
	constexpr ID3D11Buffer* VertexBuffer() const noexcept { return pVertexBuffer; }
	constexpr ID3D11Buffer* IndexBuffer() const noexcept { return pIndexBuffer; }

	void SetVertices(const std::vector<SimpleVertex>& newVertices);
	void SetIndices(const std::vector<WORD>& newIndices);

	Mesh& operator=(const Mesh& other);

private:

	Graphics* pGfx;

	std::vector<SimpleVertex> vertices;
	std::vector<WORD> indices;

	ID3D11Buffer* pVertexBuffer = nullptr;
	ID3D11Buffer* pIndexBuffer = nullptr;
};

