#pragma once
#include "NormWin.h"
#include <d3d11.h>
#include <xnamath.h>


class Camera
{
public:

	Camera();

	void SetPosition(float newX, float newY, float newZ);
	void SetRotation(XMVECTOR euler);
	void SetRotation(float aroundX, float aroundY, float aroundZ);
	void Translate(float x, float y, float z);
	void Rotate(XMVECTOR euler);
	void Rotate(float aroundX, float aroundY, float aroundZ);

	constexpr XMVECTOR Position() const noexcept {return position;}
	constexpr XMVECTOR LookAt() const noexcept {return lookAt;}
	constexpr XMVECTOR UpVector() const noexcept {return upVector;}
	constexpr XMVECTOR ForwardVector() const noexcept {return forwardVector;}

private:

	XMVECTOR position;
	XMVECTOR lookAt;
	XMVECTOR upVector;
	XMVECTOR forwardVector;
	XMVECTOR eulerAngles;
};

