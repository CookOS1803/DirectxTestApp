#include "Mesh.h"

Mesh& Mesh::operator=(const Mesh& other)
{
    vertices = other.vertices;
    indices = other.indices;

    return *this;
}
