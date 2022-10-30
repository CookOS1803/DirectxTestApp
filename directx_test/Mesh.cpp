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
    p_vertexBuffer.reset(p_gfx->CreateVertexBuffer(m_vertices));
}

void Mesh::SetIndices(const std::vector<WORD>& indices)
{
    m_indices = indices;

    RecreateIndexBuffer();
}

void Mesh::RecreateIndexBuffer()
{
    p_indexBuffer.reset(p_gfx->CreateIndexBuffer(m_indices));
}

void Mesh::Rebuild()
{
    RecreateVertexBuffer();
    RecreateIndexBuffer();
}

void Mesh::Clear()
{
    p_vertexBuffer.reset();
    p_indexBuffer.reset();
}

void Mesh::MakeSphere(int slices, int stacks)
{
    assert(slices >= 3 && stacks >= 3);

    Clear();

    std::vector<DirectX::XMVECTOR> positions;
    positions.reserve((stacks - 1) * slices + 2);

    //positions.emplace_back(0.f, 1.f, 0.f);

    for (int i = 1; i < stacks; i++)
    {
        const auto phi = DirectX::XM_PI * i / stacks;

        for (int j = 0; j < slices; j++)
        {
            const auto theta = DirectX::XM_2PI * j / slices;
            const auto sinphi = std::sin(phi);

            const auto x = sinphi * std::cos(theta);
            const auto y = std::cos(phi);
            const auto z = sinphi * std::sin(theta);

            positions.emplace_back(x, y, z);
        }
    }

    //positions.emplace_back(0.f, -1.f, 0.f);

    const auto topPosition = DirectX::XMVECTOR{ 0.f, 1.f, 0.f };
    const auto bottomPosition = DirectX::XMVECTOR{ 0.f, -1.f, 0.f };

    SimpleVertex vertex{};
    DirectX::XMStoreFloat3(&vertex.position, topPosition);

    // top vertex
    for (int j = 0; j < slices; j++)
    {
        const auto v1 = DirectX::XMVectorSubtract(topPosition, positions[j]);
        const auto v2 = DirectX::XMVectorSubtract(positions[(j + 1) % slices], positions[j]);

        DirectX::XMStoreFloat3(&vertex.normal, DirectX::XMVector3Cross(v1, v2));

        AddVertex(vertex);
    }

    // next to top vertices
    for (int j = 0; j < slices; j++)
    {
        DirectX::XMStoreFloat3(&vertex.position, positions[j + slices]);

        auto v1 = DirectX::XMVectorSubtract(topPosition, positions[j + slices]);
        auto v2 = DirectX::XMVectorSubtract(positions[(j + 1) % slices], positions[j + slices]);

        DirectX::XMStoreFloat3(&vertex.normal, DirectX::XMVector3Cross(v1, v2));
        AddVertex(vertex);
        //...
    }


    for (int i = 1; i < stacks; i++)
    {

        for (int j = 0; j < slices; j++)
        {
            DirectX::XMStoreFloat3(&vertex.position, positions[j + i * slices]);


            AddVertex(vertex);
        }
    }

    vertex.position = { 0.f, -1.f, 0.f };
    auto bottomIndex = AddVertex(vertex);

    for (int i = 0; i < slices; i++)
    {
        auto i0 = i + 1;
        auto i1 = (i + 1) % slices + 1;
        AddTriangle(topIndex, i1, i0);
        i0 = i + slices * (stacks - 2) + 1;
        i1 = (i + 1) % slices + slices * (stacks - 2) + 1;
        AddTriangle(bottomIndex, i0, i1);
    }

    for (int j = 0; j < stacks - 2; j++)
    {
        auto j0 = j * slices + 1;
        auto j1 = (j + 1) * slices + 1;
        for (int i = 0; i < slices; i++)
        {
            auto i0 = j0 + i;
            auto i1 = j0 + (i + 1) % slices;
            auto i2 = j1 + (i + 1) % slices;
            auto i3 = j1 + i;
            AddQuad(i0, i1, i2, i3);
        }
    }

    Rebuild();
}

int Mesh::AddVertex(SimpleVertex d)
{
    m_vertices.push_back(d);

    return m_vertices.size() - 1;
}

void Mesh::AddTriangle(WORD i0, WORD i1, WORD i2)
{
    m_indices.push_back(i0);
    m_indices.push_back(i1);
    m_indices.push_back(i2);
}

void Mesh::AddQuad(WORD i0, WORD i1, WORD i2, WORD i3)
{
    AddTriangle(i0, i1, i2);
    AddTriangle(i0, i2, i3);
}

Mesh& Mesh::operator=(const Mesh& other)
{
    m_vertices = other.m_vertices;
    m_indices = other.m_indices;

    return *this;
}

void Mesh::LoadFromFile(std::wstring_view fileName)
{
    Clear();

    WaveFrontReader<WORD> d;
    d.Load(fileName.data());

    if (d.vertices.empty())
        throw std::exception("asds");

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
