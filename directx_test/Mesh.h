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

	constexpr void SetVertices(std::initializer_list<SimpleVertex> newVertices) { vertices = newVertices; }
	constexpr void SetIndices(std::initializer_list<WORD> newIndices) { indices = newIndices; }

	Mesh& operator=(const Mesh& other);

private:

	std::vector<SimpleVertex> vertices;
	std::vector<WORD> indices;
};

