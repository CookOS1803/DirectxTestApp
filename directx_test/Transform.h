#pragma once

struct Transform
{
	Transform() : position(), eulerRotation(), scale(1.f, 1.f, 1.f) {}

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 eulerRotation;
	DirectX::XMFLOAT3 scale;
};

