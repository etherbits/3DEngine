#include "math.h"

// V--V Vec2 V--V
Vec2::Vec2(float x, float y)
    : x(x), y(y)
{}

Vec2::Vec2()
    : x(0), y(0)
{}

Vec2 Vec2::operator+(const Vec2& rVec) const
{
    return Vec2(this->x + rVec.x, this->y + rVec.y);
}

Vec2 Vec2::operator-(const Vec2& rVec) const
{
    return Vec2(this->x - rVec.x, this->y - rVec.y);
}

Vec2 Vec2::operator*(const float rMagnitude) const
{
    return Vec2(this->x * rMagnitude, this->y * rMagnitude);
}

Vec2 Vec2::operator/(const float rMagnitude) const
{
    return Vec2(this->x / rMagnitude, this->y / rMagnitude);
}


// V--V Vec3 V--V
Vec3::Vec3(float x, float y, float z)
	: x(x), y(y), z(z)
{}

Vec3::Vec3()
	: x(0), y(0), z(0)
{}

Vec3 Vec3::operator+(const Vec3& rVec) const
{
	return Vec3(this->x + rVec.x, this->y + rVec.y, this->z + rVec.z);
}

Vec3 Vec3::operator-(const Vec3& rVec) const
{
	return Vec3(this->x - rVec.x, this->y - rVec.y, this->z - rVec.z);
}

Vec3 Vec3::operator*(const float amount) const
{
	return Vec3(this->x * amount, this->y * amount, this->z * amount);
}

Vec3 Vec3::operator/(const float amount) const
{
	return Vec3(this->x / amount, this->y / amount, this->z / amount);
}

void Vec3::operator+=(const Vec3& rVec)
{
	this->x = this->x + rVec.x;
	this->y = this->y + rVec.y;
	this->z = this->z + rVec.z;
}

double VecAngleBetween(const Vec3& lVec, const Vec3& rVec)
{
	return acos(VecDot(lVec, rVec) / (lVec.GetMagnitude() * rVec.GetMagnitude()));
}

float VecDot(const Vec3& lVec, const Vec3& rVec)
{
	return float(lVec.x * rVec.x + lVec.y * rVec.y + lVec.z * rVec.z);
}

float Vec3::GetMagnitude() const
{
	return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
}

Vec3 Mat4::operator *(const Vec3& rVec)
{
	Vec3 tVec;
	tVec.x = this->matrix[0][0] * rVec.x + this->matrix[0][1] * rVec.y + this->matrix[0][2] * rVec.z + this->matrix[0][3] * 1;
	tVec.y = this->matrix[1][0] * rVec.x + this->matrix[1][1] * rVec.y + this->matrix[1][2] * rVec.z + this->matrix[1][3] * 1;
	tVec.z = this->matrix[2][0] * rVec.x + this->matrix[2][1] * rVec.y + this->matrix[2][2] * rVec.z + this->matrix[2][3] * 1;
	return tVec;
}

Mat4 Mat4::operator *(const Mat4& rMatrix)
{
	Mat4 mat4;
	for (int lRow = 0; lRow < 4; lRow++)
	{
		for (int rCol = 0; rCol < 4; rCol++)
		{
			mat4.matrix[lRow][rCol] = 0;
			for(int i = 0; i < 4; i++)
			{
				mat4.matrix[lRow][rCol] += this->matrix[lRow][i] * rMatrix.matrix[i][rCol];
			}
		}
	}
	return mat4;
}

Mat4 Mat4::Transform(const Vec3& vec)
{
	return Mat4
	{
		1, 0, 0, vec.x,
		0, 1, 0, vec.y,
		0, 0, 1, vec.z,
		0, 0, 0, 0,
	};
}

Mat4 Mat4::RotateX(const float& angle)
{
	return Mat4
	{
		1, 0, 0, 0,
		0, cosf(angle), -sinf(angle), 0,
		0,  sinf(angle), cosf(angle), 0,
		0, 0, 0, 0,
	};
}

Mat4 Mat4::RotateY(const float& angle)
{
	return Mat4
	{
		cosf(angle), 0, sinf(angle), 0,
		0, 1, 0, 0,
		-sinf(angle), 0, cosf(angle), 0,
		0, 0, 0, 0,
	};
}

Mat4 Mat4::RotateZ(const float& angle)
{
	return Mat4
	{
		cosf(angle), -sinf(angle), 0, 0,
		sinf(angle), cosf(angle), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 0,
	};
}

Mat4 Mat4::Scale(const float& factor)
{
	return Mat4
	{
		factor, 0, 0, 0,
		0, factor, 0, 0,
		0, 0, factor, 0,
		0, 0, 0, 0,
	};
}


// V--V Face V--V
Face::Face(const Vec3* a, const Vec3* b, const Vec3* c, const Vec3 normal)
	: vertices{ a, b, c }, normal(normal) 
{}

Vec3 Face::GetCenter() const
{
	return Vec3(vertices[0]->x / 3 + vertices[1]->x / 3 + vertices[2]->x / 3, vertices[0]->y / 3 + vertices[1]->y / 3 + vertices[2]->y / 3, vertices[0]->z / 3 + vertices[1]->z / 3 + vertices[2]->z / 3);
}