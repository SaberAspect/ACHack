#pragma once
#include <math.h>
struct Vector3
{
	float x, y, z;
	Vector3();

	Vector3 operator+(Vector3& ref);
	Vector3 operator-(Vector3& ref);
	Vector3 operator*(Vector3& ref);
	Vector3 operator/(Vector3& ref);
	static float DistanceVec3(Vector3 dst, Vector3 src);
	void Normalize();
};

struct Vector4
{
	float x, y, z, w;
};
