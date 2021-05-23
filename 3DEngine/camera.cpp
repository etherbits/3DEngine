#include "camera.h"

Camera::Camera(Vec3 position, Vec3 normal)
	: position(position), normal(normal)
{}

void Camera::Transform(Vec3 vec)
{
	this->position += vec;
}

const Vec3& Camera::GetPosition() const
{
	return position;
}
const Vec3& Camera::GetNormal() const
{
	return normal;
}