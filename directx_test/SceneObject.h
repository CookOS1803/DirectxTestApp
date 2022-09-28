#pragma once
#include "NormWin.h"
#include <vector>
#include <memory>
#include <xnamath.h>
#include "Mesh.h"

class SceneObject
{
public:

	constexpr SceneObject() : mesh() {}
	SceneObject(const SceneObject& other) : mesh(other.mesh) {}

	const Mesh& GetMesh() const { return mesh; }
	void SetMesh(const Mesh& newMesh) { mesh = newMesh; }

private:

	Mesh mesh;
};

