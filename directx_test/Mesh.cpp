#include "Mesh.h"
#include "Graphics.h"

Mesh::Mesh(const Mesh& other)
{
    p_gfx = other.p_gfx;
    SetVertices(other.m_vertices);
    SetIndices(other.m_indices);
    p_pixelShader = other.p_pixelShader;
}

Mesh::~Mesh()
{
    if (p_vertexBuffer) p_vertexBuffer->Release();
    if (p_indexBuffer) p_indexBuffer->Release();
}

void Mesh::SetVertices(const std::vector<SimpleVertex>& vertices)
{
    m_vertices = vertices;

    if (p_vertexBuffer) p_vertexBuffer->Release();
    p_vertexBuffer.reset(p_gfx->CreateVertexBuffer(m_vertices));
}

void Mesh::SetIndices(const std::vector<WORD>& indices)
{
    m_indices = indices;

    if (p_indexBuffer) p_indexBuffer->Release();
    p_indexBuffer.reset(p_gfx->CreateIndexBuffer(m_indices));
}

Mesh& Mesh::operator=(const Mesh& other)
{
    m_vertices = other.m_vertices;
    m_indices = other.m_indices;

    return *this;
}
