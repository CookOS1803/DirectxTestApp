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
	SceneObject(const SceneObject& other) : pMesh(other.pMesh) {}

	constexpr const Mesh* GetMesh() const { return pMesh; }
	constexpr void SetMesh(Mesh* newMesh) { pMesh = newMesh; }

	//XMFLOAT3 position;
	//XMFLOAT3 eulerRotation;
	//XMFLOAT3 scale;


private:

	Mesh* pMesh;
};

