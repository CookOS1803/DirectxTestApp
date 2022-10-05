#include "Mesh.h"
#include "Graphics.h"

Mesh::Mesh(const Mesh& other)
{
    pGfx = other.pGfx;
    SetVertices(other.vertices);
    SetIndices(other.indices);
}

Mesh::~Mesh()
{
    if (pVertexBuffer) pVertexBuffer->Release();
    if (pIndexBuffer) pIndexBuffer->Release();
}

void Mesh::SetVertices(const std::vector<SimpleVertex>& newVertices)
{
    vertices = newVertices;

    if (pVertexBuffer) pVertexBuffer->Release();
    pVertexBuffer.reset(pGfx->CreateVertexBuffer(vertices));
}

void Mesh::SetIndices(const std::vector<WORD>& newIndices)
{
    indices = newIndices;

    if (pIndexBuffer) pIndexBuffer->Release();
    pIndexBuffer.reset(pGfx->CreateIndexBuffer(indices));
}

Mesh& Mesh::operator=(const Mesh& other)
{
    vertices = other.vertices;
    indices = other.indices;

    return *this;
}
