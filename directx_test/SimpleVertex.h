#pragma once

#include "NormWin.h"
#include <DirectXMath.h>

struct SimpleVertex
{
	SimpleVertex()
		: position(), color(1.f, 1.f, 1.f), normal(1.f, 0.f, 0.f), texCoord(0.f, 0.f) {}
	SimpleVertex(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 color, DirectX::XMFLOAT3 normal, DirectX::XMFLOAT2 texCoord)
		: position(position), color(color), normal(normal), texCoord(texCoord)
	{
	}
	
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 color;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 texCoord;
};