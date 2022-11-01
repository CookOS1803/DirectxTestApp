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

void Mesh::SetIndices(const std::vector<UINT>& indices)
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
    m_vertices.clear();
    m_indices.clear();
    p_vertexBuffer.reset();
    p_indexBuffer.reset();
}

void Mesh::MakeSphere(int slices, int stacks, DirectX::XMVECTORF32 color)
{
    Clear();

    std::vector<DirectX::XMVECTOR> positions;
    positions.reserve((stacks - 1) * slices);

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

            positions.push_back(DirectX::XMVectorSet(x, y, z, 0.f));
        }
    }

    const auto topPosition = DirectX::XMVECTOR{ 0.f, 1.f, 0.f };
    const auto bottomPosition = DirectX::XMVECTOR{ 0.f, -1.f, 0.f };

    SimpleVertex vertex{};
    DirectX::XMStoreFloat3(&vertex.color, color);
    DirectX::XMStoreFloat3(&vertex.position, topPosition);

    // top vertex
    for (int j = 0; j < slices; j++)
    {
        const auto up = DirectX::XMVectorSubtract(topPosition, positions[j]);
        const auto right = DirectX::XMVectorSubtract(positions[(j + 1) % slices], positions[j]);

        DirectX::XMStoreFloat3(&vertex.normal, DirectX::XMVector3Cross(up, right));

        AddVertex(vertex);

    }

    // next to top vertices
    for (int j = 0; j < slices; j++)
    {
        DirectX::XMStoreFloat3(&vertex.position, positions[j]);

        const auto up = DirectX::XMVectorSubtract(topPosition, positions[j]);
        const auto right = DirectX::XMVectorSubtract(positions[(j + 1) % slices], positions[j]);
        const auto down = DirectX::XMVectorSubtract(positions[j + slices], positions[j]);
        const auto left = DirectX::XMVectorSubtract(positions[(j - 1 + slices) % slices], positions[j]);

        DirectX::XMStoreFloat3(&vertex.normal, DirectX::XMVector3Cross(up, right));
        AddVertex(vertex);

        DirectX::XMStoreFloat3(&vertex.normal, DirectX::XMVector3Cross(left, up));
        AddVertex(vertex);

        DirectX::XMStoreFloat3(&vertex.normal, DirectX::XMVector3Cross(down, left));
        AddVertex(vertex);

        DirectX::XMStoreFloat3(&vertex.normal, DirectX::XMVector3Cross(right, down));
        AddVertex(vertex);
    }

    // middle vertices
    for (int i = 1; i < stacks - 2; i++)
    {
        for (int j = 0; j < slices; j++)
        {
            DirectX::XMStoreFloat3(&vertex.position, positions[j + i * slices]);

            const auto up = DirectX::XMVectorSubtract(positions[j + (i - 1) * slices], positions[j + i * slices]);
            const auto right = DirectX::XMVectorSubtract(positions[(j + 1) % slices + i * slices], positions[j + i * slices]);
            const auto down = DirectX::XMVectorSubtract(positions[j + (i + 1)*slices], positions[j + i * slices]);
            const auto left = DirectX::XMVectorSubtract(positions[(j - 1 + slices) % slices + i * slices], positions[j + i * slices]);

            DirectX::XMStoreFloat3(&vertex.normal, DirectX::XMVector3Cross(up, right));
            AddVertex(vertex);

            DirectX::XMStoreFloat3(&vertex.normal, DirectX::XMVector3Cross(left, up));
            AddVertex(vertex);

            DirectX::XMStoreFloat3(&vertex.normal, DirectX::XMVector3Cross(down, left));
            AddVertex(vertex);

            DirectX::XMStoreFloat3(&vertex.normal, DirectX::XMVector3Cross(right, down));
            AddVertex(vertex);
        }
    }

    // before bottom vertices
    for (int j = 0; j < slices; j++)
    {
        DirectX::XMStoreFloat3(&vertex.position, positions[j + (stacks - 2) * slices]);

        const auto up = DirectX::XMVectorSubtract(positions[j + (stacks - 3) * slices], positions[j + (stacks - 2) * slices]);
        const auto right = DirectX::XMVectorSubtract(positions[(j + 1) % slices + (stacks - 2) * slices], positions[j + (stacks - 2) * slices]);
        const auto down = DirectX::XMVectorSubtract(bottomPosition, positions[j + (stacks - 2) * slices]);
        const auto left = DirectX::XMVectorSubtract(positions[(j - 1 + slices) % slices + (stacks - 2) * slices], positions[j + (stacks - 2) * slices]);

        DirectX::XMStoreFloat3(&vertex.normal, DirectX::XMVector3Cross(up, right));
        AddVertex(vertex);

        DirectX::XMStoreFloat3(&vertex.normal, DirectX::XMVector3Cross(left, up));
        AddVertex(vertex);

        DirectX::XMStoreFloat3(&vertex.normal, DirectX::XMVector3Cross(down, left));
        AddVertex(vertex);

        DirectX::XMStoreFloat3(&vertex.normal, DirectX::XMVector3Cross(right, down));
        AddVertex(vertex);
    }

    DirectX::XMStoreFloat3(&vertex.position, bottomPosition);

    // bottom vertex
    for (int j = 0; j < slices; j++)
    {
        const auto up = DirectX::XMVectorSubtract(positions[j + (stacks - 2) * slices], bottomPosition);
        const auto right = DirectX::XMVectorSubtract(positions[(j + 1) % slices + (stacks - 2) * slices], bottomPosition);

        DirectX::XMStoreFloat3(&vertex.normal, DirectX::XMVector3Cross(up, right));

        AddVertex(vertex);
    }    


    for (int i = 0; i < slices; i++)
    {
        AddTriangle(
            i,
            4 * ((i + 1) % slices) + slices + 1,
            4 * i + slices
        );
        
        const UINT b = m_vertices.size() - slices;
        AddTriangle(
            b + i,
            b - slices*4 + i * 4 + 3,
            b - slices * 4 + ((i + 1) % slices) * 4 + 2
        );
    }


    for (int i = 0; i < stacks - 2; i++)
    {
        for (int j = 0; j < slices; j++)
        {
            AddQuad(
                slices + 4*slices*i     + 4*j                + 3,
                slices + 4*slices*i     + 4*((j + 1)%slices) + 2,
                slices + 4*slices*(i+1) + 4 * ((j + 1) % slices) + 1,
                slices + 4*slices*(i+1) + 4 * j
            );
        }
    }

    wchar_t buf[16];
    _itow_s(m_vertices.size(), buf, 10);
    OutputDebugString(lstrcat(buf, L"\n"));
    Rebuild();
}

int Mesh::AddVertex(SimpleVertex d)
{
    m_vertices.push_back(d);

    return m_vertices.size() - 1;
}

void Mesh::AddTriangle(UINT i0, UINT i1, UINT i2)
{
    m_indices.push_back(i0);
    m_indices.push_back(i1);
    m_indices.push_back(i2);
}

void Mesh::AddQuad(UINT i0, UINT i1, UINT i2, UINT i3)
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

    WaveFrontReader<UINT> d;
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
