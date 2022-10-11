#pragma once
#include "NormWin.h"
#include <xnamath.h>

struct SimpleVertex
{
	SimpleVertex(XMFLOAT3 position, XMFLOAT3 color, XMFLOAT3 normal)
		: position(position), color(color), normal(normal)
	{
	}

	XMFLOAT3 position;
	XMFLOAT3 color;
	XMFLOAT3 normal;
};