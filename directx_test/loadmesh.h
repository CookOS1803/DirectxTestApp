#pragma once
#include "NormWin.h"
#include <vector>

struct vector3load
{
	constexpr vector3load(float x, float y, float z)
		: x(x), y(y), z(z) {}

	float x, y, z;
};

struct vector2load
{
	constexpr vector2load(float x, float y)
		: x(x), y(y) {}

	float x, y;
};

struct vertexload
{
	constexpr vertexload(float posx, float posy, float posz, float normx, float normy, float normz, float texx, float texy)
		: position(posx, posy, posz), normal(normx, normy, normz), texCoord(texx, texy) {}

	vector3load position;
	vector3load normal;
	vector2load texCoord;
};

void lad(std::vector<vertexload>& vertices, std::vector<WORD>& indices);