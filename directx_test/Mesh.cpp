#include "Mesh.h"
#include "Graphics.h"
#include "loadmesh.h"

Mesh::Mesh(const Mesh& other)
{
    p_gfx = other.p_gfx;
    SetVertices(other.m_vertices);
    SetIndices(other.m_indices);
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

void Mesh::LoadFromFile(std::wstring_view fileName)
{
    std::vector<vertexload> l_vertices;
    std::vector<WORD> indices;

    lad(l_vertices, indices);

    std::vector<SimpleVertex> vertices;
    vertices.reserve(l_vertices.size());

    for (const auto& v : l_vertices)
    {
        vertices.emplace_back(XMFLOAT3{ v.position.x, v.position.y, v.position.z }, XMFLOAT3{ 1.f, 1.f, 1.f }, XMFLOAT3{ v.normal.x, v.normal.y, v.normal.z }, XMFLOAT2{ v.texCoord.x, v.texCoord.y });
    }

    SetVertices(vertices);
    SetIndices(indices);
}
