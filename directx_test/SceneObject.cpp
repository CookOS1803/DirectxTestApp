#include "SceneObject.h"

SceneObject::SceneObject(const SceneObject& other)
	: p_mesh(other.p_mesh), m_meshRenderer(other.m_meshRenderer), m_transform(other.m_transform)
{

	exit(0);
}
