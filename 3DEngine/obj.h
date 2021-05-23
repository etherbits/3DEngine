#pragma once
#include "math.h"

struct Object
{
	Vec3 position;
	//void Transform(Vec3 vec);
	Object(const Vec3 position);

	// MeshObject
	virtual Vec3* GetVertices() const;
	virtual Face* GetFaces() const;
	virtual size_t GetVertexCount() const;
	virtual size_t GetFaceCount() const;
};

struct MeshObject : public Object
{
	const size_t vertCount, faceCount;
	Vec3* pVertices;
	Face* pFaces;
	MeshObject(const Vec3 position, const size_t vertCount, const size_t faceCount, Vec3* pVertices, Face* pFaces);

	Vec3* GetVertices() const override;
	Face* GetFaces() const override;
	size_t GetVertexCount() const override;
	size_t GetFaceCount() const override;
};

struct Cube : public MeshObject
{
	Vec3 vertices[8];
	Face faces[12];
	Cube();
};