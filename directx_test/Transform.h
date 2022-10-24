#pragma once
#include <xnamath.h>

struct Transform
{
	Transform() : position(), eulerRotation(), scale(1.f, 1.f, 1.f) {}

	XMFLOAT3 position;
	XMFLOAT3 eulerRotation;
	XMFLOAT3 scale;
};

