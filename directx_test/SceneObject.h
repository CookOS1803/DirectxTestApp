#pragma once
#include "NormWin.h"
#include <vector>
#include <memory>
#include <concepts>
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Updateable.h"
#include "Transform.h"

template<class T, class U>
concept Derived = std::is_base_of<U, T>::value;

class SceneObject
{
public:

	SceneObject() : p_mesh(nullptr) {}
	SceneObject(const SceneObject& other) = delete;

	constexpr const Mesh* GetMesh() const noexcept { return p_mesh; }
	constexpr void SetMesh(Mesh* newMesh) noexcept { p_mesh = newMesh; }

	constexpr MeshRenderer& GetMeshRenderer() noexcept { return m_meshRenderer; }
	constexpr const MeshRenderer& GetMeshRenderer() const noexcept { return m_meshRenderer; }

	constexpr Transform& GetTransform() noexcept { return m_transform; }
	constexpr const Transform& GetTransform() const noexcept { return m_transform; }

	template<Derived<Updateable> T>
	void SetUpdateable()
	{
		p_updateable.reset(new T(this));
	}

	constexpr Updateable* GetUpdateable() { return p_updateable.get(); }
	


private:

	Mesh* p_mesh;
	MeshRenderer m_meshRenderer;
	Transform m_transform;
	std::unique_ptr<Updateable> p_updateable = nullptr;
};

