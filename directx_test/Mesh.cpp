#include "Mesh.h"
#include "Graphics.h"
#include <WaveFrontReader.h>

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

    RecreateVertexBuffer();
}

void Mesh::RecreateVertexBuffer()
{
    if (p_vertexBuffer) p_vertexBuffer->Release();
    p_vertexBuffer.reset(p_gfx->CreateVertexBuffer(m_vertices));
}

void Mesh::SetIndices(const std::vector<WORD>& indices)
{
    m_indices = indices;

    RecreateIndexBuffer();
}

void Mesh::RecreateIndexBuffer()
{
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
    WaveFrontReader<WORD> d;
    d.Load(fileName.data());

    m_vertices.reserve(d.vertices.size());
        
    for (const auto& v : d.vertices)
    {
        m_vertices.emplace_back(
            DirectX::XMFLOAT3{ v.position.x, v.position.y, v.position.z },
            DirectX::XMFLOAT3{ 1.f, 1.f, 1.f },
            DirectX::XMFLOAT3{ v.normal.x, v.normal.y, v.normal.z },
            DirectX::XMFLOAT2{ v.textureCoordinate.x, v.textureCoordinate.y }
        );
    }

    RecreateVertexBuffer();
    SetIndices(d.indices);
}
