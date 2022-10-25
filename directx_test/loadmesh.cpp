#include "loadmesh.h"
#include <WaveFrontReader.h>

void lad(std::vector<vertexload>& vertices, std::vector<WORD>& indices)
{
	WaveFrontReader<WORD> d;
	d.Load(L"Padlock.obj");

	vertices.reserve(d.vertices.size());

	for (const auto& v : d.vertices)
	{
		vertices.emplace_back(v.position.x, v.position.y, v.position.z,
			v.normal.x, v.normal.y, v.normal.z, v.textureCoordinate.x, v.textureCoordinate.y);
	}

	indices = d.indices;
}
