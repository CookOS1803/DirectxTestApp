#pragma once
#include "NormWin.h"
#include <vector>
#include "SimpleVertex.h"

class Mesh
{
public:

	constexpr Mesh() : vertices(), indices() {}
	Mesh(const Mesh& other) : vertices(other.vertices), indices(other.indices) {}

	constexpr const std::vector<SimpleVertex>& Vertices() const noexcept { return vertices; }
	constexpr const std::vector<WORD>& Indices() const noexcept { return indices; }

	constexpr void SetVertices(const std::vector<SimpleVertex>& newVertices) { vertices = newVertices; }
	constexpr void SetIndices(const std::vector<WORD>& newIndices) { indices = newIndices; }

private:

	std::vector<SimpleVertex> vertices;
	std::vector<WORD> indices;
};

