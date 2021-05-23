#pragma once
#include <math.h>
#define PI 3.14159265358979323846f

struct Vec2
{
    float x, y;
    Vec2(const float x, const float y);
    Vec2();
    Vec2 operator+(const Vec2& rVec) const;
    Vec2 operator-(const Vec2& rVec) const;
    Vec2 operator*(const float magnitude) const;
    Vec2 operator/(const float magnitude) const;
};

struct Vec3 {
	float x, y, z;
	Vec3();
	Vec3(const float x, const float y, const float z);
	Vec3 operator+(const Vec3& rVec) const;
	Vec3 operator-(const Vec3& rVec) const;
	Vec3 operator*(const float magnitude) const;
	Vec3 operator/(const float magnitude) const;
	void operator+=(const Vec3& rVec);
	float GetMagnitude() const;

};

double VecAngleBetween(const Vec3& a, const Vec3& b);
float VecDot(const Vec3& lVec, const Vec3& rVec);

struct Mat4
{
	// [row]->[col]
	float matrix[4][4];

	Vec3 operator *(const Vec3& rVec);
	Mat4 operator *(const Mat4& rMatrix);
	static Mat4 Transform(const Vec3& vec);
	static Mat4 RotateX(const float& angle);
	static Mat4 RotateY(const float& angle);
	static Mat4 RotateZ(const float& angle);
	static Mat4 Scale(const float& factor);
};

struct Face {
	const Vec3* vertices[3];
	Vec3 normal;
	Face(const Vec3* a, const Vec3* b, const Vec3* c, const Vec3 normal);
	Vec3 GetCenter() const;
};

