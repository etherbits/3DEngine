#pragma once
#include "math.h"
class Camera
{
	Vec3 position, normal;
public:
	Camera(Vec3 position, Vec3 normal);
	void Transform(Vec3 vec);
	const Vec3& GetPosition() const;
	const Vec3& GetNormal() const;
};