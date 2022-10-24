#include "MeshRenderer.h"

MeshRenderer& MeshRenderer::operator=(const MeshRenderer& other) noexcept
{
    p_pixelShader = other.p_pixelShader;

    return *this;
}
