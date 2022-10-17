#pragma once
#include "NormWin.h"
#include <xnamath.h>

struct SimpleVertex
{
	SimpleVertex(XMFLOAT3 position, XMFLOAT3 color, XMFLOAT3 normal, XMFLOAT2 texCoord)
		: position(position), color(color), normal(normal), texCoord(texCoord)
	{
	}

	XMFLOAT3 position;
	XMFLOAT3 color;
	XMFLOAT3 normal;
	XMFLOAT2 texCoord;
};