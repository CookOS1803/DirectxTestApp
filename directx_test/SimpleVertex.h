#pragma once
#include "NormWin.h"
#include <xnamath.h>

struct SimpleVertex
{
	SimpleVertex(XMFLOAT3 position, XMFLOAT4 color) : position(position), color(color)
	{
	}

	XMFLOAT3 position;
	XMFLOAT4 color;
};