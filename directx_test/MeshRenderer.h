#pragma once
#include "NormWin.h"
#include <DirectXHelpers.h>

class MeshRenderer
{
public:

	constexpr MeshRenderer() {}
	constexpr MeshRenderer(const MeshRenderer& other) : p_pixelShader(other.p_pixelShader) {}

	constexpr ID3D11PixelShader* PixelShader() const noexcept { return p_pixelShader; }
	constexpr void SetPixelShader(ID3D11PixelShader* shader) noexcept { p_pixelShader = shader; }

	MeshRenderer& operator=(const MeshRenderer& other) noexcept;

private:

	ID3D11PixelShader* p_pixelShader = nullptr;
};

