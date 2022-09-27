#pragma once
#include "NormWin.h"
#include <vector>
#include <memory>
#include <xnamath.h>
#include "Mesh.h"

class SceneObject
{
public:

	constexpr SceneObject() : pMesh(nullptr) {}
	SceneObject(const SceneObject& other) : pMesh(std::make_unique<Mesh>(*other.pMesh)) {}

	const Mesh* GetMesh() const { return pMesh.get(); }
	void SetMesh(const Mesh& mesh) { pMesh = std::make_unique<Mesh>(mesh); }

private:

	std::unique_ptr<Mesh> pMesh;
	//XMVECTOR position;
};

