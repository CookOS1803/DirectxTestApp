#include "Camera.h"
#include <cmath>

Camera::Camera()
	: position(), lookAt(), upVector(), forwardVector(), eulerAngles()
{
}

void Camera::SetPosition(float newX, float newY, float newZ)
{
	position = XMVectorSet(newX, newY, newZ, 0.f);
	lookAt = position + forwardVector;
}

void Camera::SetRotation(XMVECTOR euler)
{
	XMFLOAT3 e;
	XMStoreFloat3(&e, euler);

	e.x = std::min(std::max(-XM_PIDIV2, e.x), XM_PIDIV2);
	e.z = std::min(std::max(-XM_PIDIV2, e.z), XM_PIDIV2);

	eulerAngles = XMVectorSet(e.x, e.y, e.z, 0.f);

	forwardVector = XMVector3Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYawFromVector(eulerAngles));
	lookAt = position + forwardVector;
	upVector = XMVector3Transform(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), XMMatrixRotationRollPitchYawFromVector(eulerAngles));
}

void Camera::SetRotation(float aroundX, float aroundY, float aroundZ)
{
	SetRotation(XMVectorSet(aroundX, aroundY, aroundZ, 0.f));
}

void Camera::Translate(float x, float y, float z)
{
	XMFLOAT3 forward{}, up{}, left{};
	
	XMStoreFloat3(&forward, forwardVector);
	XMStoreFloat3(&up, upVector);
	XMStoreFloat3(&left, XMVector3Cross(upVector, forwardVector));

	const auto transformation = XMMatrixSet(
		left.x, left.y, left.z, 0.f,
		up.x, up.y, up.z, 0.f,
		forward.x, forward.y, forward.z, 0.f,
		0.f, 0.f, 0.f, 0.f
	);
	const auto translation = XMVector3Transform(XMVectorSet(x, y, z, 0.f), transformation);
	position += translation;
	
	lookAt = position + forwardVector;
}

void Camera::Rotate(XMVECTOR euler)
{
	SetRotation(eulerAngles + euler);
}

void Camera::Rotate(float aroundX, float aroundY, float aroundZ)
{
	SetRotation(eulerAngles + XMVectorSet(aroundX, aroundY, aroundZ, 0.f));
}
