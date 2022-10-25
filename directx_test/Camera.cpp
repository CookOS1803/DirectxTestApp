#include "Camera.h"
#include <cmath>

Camera::Camera()
	: position(), lookAt(), upVector(), forwardVector(), eulerAngles()
{
}

void Camera::SetPosition(float newX, float newY, float newZ)
{
	position = DirectX::XMVectorSet(newX, newY, newZ, 0.f);
	lookAt = DirectX::XMVectorAdd(position, forwardVector);
}

void Camera::SetRotation(DirectX::XMVECTOR euler)
{
	DirectX::XMFLOAT3 e;
	DirectX::XMStoreFloat3(&e, euler);

	e.x = std::min(std::max(-DirectX::XM_PIDIV2, e.x), DirectX::XM_PIDIV2);
	e.z = std::min(std::max(-DirectX::XM_PIDIV2, e.z), DirectX::XM_PIDIV2);

	eulerAngles = DirectX::XMVectorSet(e.x, e.y, e.z, 0.f);

	forwardVector = DirectX::XMVector3Transform(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f), DirectX::XMMatrixRotationRollPitchYawFromVector(eulerAngles));
	lookAt = DirectX::XMVectorAdd(position, forwardVector);
	upVector = DirectX::XMVector3Transform(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), DirectX::XMMatrixRotationRollPitchYawFromVector(eulerAngles));
}

void Camera::SetRotation(float aroundX, float aroundY, float aroundZ)
{
	SetRotation(DirectX::XMVectorSet(aroundX, aroundY, aroundZ, 0.f));
}

void Camera::Translate(float x, float y, float z)
{
	DirectX::XMFLOAT3 forward{}, up{}, left{};
	
	DirectX::XMStoreFloat3(&forward, forwardVector);
	DirectX::XMStoreFloat3(&up, upVector);
	DirectX::XMStoreFloat3(&left, DirectX::XMVector3Cross(upVector, forwardVector));

	const auto transformation = DirectX::XMMatrixSet(
		left.x, left.y, left.z, 0.f,
		up.x, up.y, up.z, 0.f,
		forward.x, forward.y, forward.z, 0.f,
		0.f, 0.f, 0.f, 0.f
	);
	const auto translation = DirectX::XMVector3Transform(DirectX::XMVectorSet(x, y, z, 0.f), transformation);
	position = DirectX::XMVectorAdd(position, translation);
	
	lookAt = DirectX::XMVectorAdd(position, forwardVector);
}

void Camera::Rotate(DirectX::XMVECTOR euler)
{
	SetRotation(DirectX::XMVectorAdd(eulerAngles, euler));
}

void Camera::Rotate(float aroundX, float aroundY, float aroundZ)
{
	SetRotation(DirectX::XMVectorAdd(eulerAngles, DirectX::XMVectorSet(aroundX, aroundY, aroundZ, 0.f)));
}
