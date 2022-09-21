#include "Camera.h"

Camera::Camera()
	: position(), lookAt(), upVector(), forwardVector()
{
}

void Camera::SetPosition(float newX, float newY, float newZ)
{
	position = XMVectorSet(newX, newY, newZ, 0.f);
	lookAt = position + forwardVector;
}

void Camera::SetRotation(float aroundX, float aroundY, float aroundZ)
{
	forwardVector = XMVector3Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(aroundX, aroundY, aroundZ));
	lookAt = position + forwardVector;
	upVector = XMVector3Transform(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), XMMatrixRotationRollPitchYaw(aroundX, aroundY, aroundZ));

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
