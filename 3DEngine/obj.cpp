#include "obj.h"

Object::Object(const Vec3 position)
	: position(position)
{}

Vec3* Object::GetVertices() const { return nullptr; }
Face* Object::GetFaces() const { return nullptr; }
size_t Object::GetVertexCount() const { return 0; }
size_t Object::GetFaceCount() const { return 0; }

MeshObject::MeshObject(const Vec3 position, const size_t vertCount, const size_t faceCount, Vec3* pVertices, Face* pFaces)
	: Object(position), vertCount(vertCount), faceCount(faceCount), pVertices(pVertices), pFaces(pFaces)
{}

Vec3* MeshObject::GetVertices() const { return pVertices; }
Face* MeshObject::GetFaces() const { return pFaces; }
size_t MeshObject::GetVertexCount() const { return vertCount; }
size_t MeshObject::GetFaceCount() const { return faceCount; }

Cube::Cube()
	:
	vertices
{
	Vec3(-.5f, -.5f, -.5f), Vec3(-.5f, .5f, -.5f), Vec3(.5f, .5f, -.5f), Vec3(.5f, -.5f, -.5f),
	Vec3(-.5f, -.5f, .5f), Vec3(-.5f, .5f, .5f), Vec3(.5f, .5f, .5f), Vec3(.5f, -.5f, .5f)
},
faces
{
	Face(&vertices[1], &vertices[5], &vertices[0], Vec3(-1.f, 0, 0)), Face(&vertices[5], &vertices[4], &vertices[0], Vec3(-1.f, 0, 0)),
	Face(&vertices[3], &vertices[7], &vertices[2], Vec3(1.f, 0, 0)), Face(&vertices[7], &vertices[6], &vertices[2], Vec3(1.f, 0, 0)),

	Face(&vertices[0], &vertices[4], &vertices[3], Vec3(0, -1.f, 0)), Face(&vertices[4], &vertices[7], &vertices[3], Vec3(0, -1.f, 0)),
	Face(&vertices[2], &vertices[6], &vertices[1], Vec3(0, 1.f, 0)), Face(&vertices[6], &vertices[5], &vertices[1], Vec3(0, 1.f, 0)),

	Face(&vertices[3], &vertices[2], &vertices[1], Vec3(0, 0, -1.f)), Face(&vertices[1], &vertices[0], &vertices[3], Vec3(0, 0, -1.f)),
	Face(&vertices[4], &vertices[5], &vertices[6], Vec3(0, 0, 1.f)), Face(&vertices[6], &vertices[7], &vertices[4], Vec3(0, 0, 1.f))
}, MeshObject(Vec3(0, 0, 0), 8, 12, vertices, faces)
{}