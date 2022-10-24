#pragma once
#include "NormWin.h"
#include <vector>
#include <memory>
#include <xnamath.h>
#include "Mesh.h"
#include "MeshRenderer.h"

class SceneObject
{
public:

	constexpr SceneObject() : p_mesh(nullptr) {}
	constexpr SceneObject(const SceneObject& other) : p_mesh(other.p_mesh), m_meshRenderer(other.m_meshRenderer) {}

	constexpr const Mesh* GetMesh() const noexcept { return p_mesh; }
	constexpr void SetMesh(Mesh* newMesh) noexcept { p_mesh = newMesh; }

	constexpr MeshRenderer& GetMeshRenderer() noexcept { return m_meshRenderer; }

	//XMFLOAT3 position;
	//XMFLOAT3 eulerRotation;
	//XMFLOAT3 scale;


private:

	Mesh* p_mesh;
	MeshRenderer m_meshRenderer;
};

