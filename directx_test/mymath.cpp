#include "mymath.h"
#include <cmath>
#include <DirectXMath.h>

float sind(float a)
{
	return std::sin(a * (DirectX::XM_PI / 180));
}

float cosd(float a)
{
	return std::cos(a * (DirectX::XM_PI / 180));
}
