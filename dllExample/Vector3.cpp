#include "Vector3.h"

Vector3::Vector3()
{
	x, y, z = 0;
}

Vector3 Vector3::operator+(Vector3& ref)
{
	this->x += ref.x;
	this->y += ref.y;
	this->z += ref.z;
	return *this;

}
Vector3 Vector3::operator-(Vector3& ref)
{
	this->x -= ref.x;
	this->y -= ref.y;
	this->z -= ref.z;
	return *this;

}
Vector3 Vector3::operator*(Vector3& ref)
{
	this->x *= ref.x;
	this->y *= ref.y;
	this->z *= ref.z;
	return *this;
}
Vector3 Vector3::operator/(Vector3& ref)
{
	this->x /= ref.x;
	this->y /= ref.y;
	this->z /= ref.z;
	return *this;
}
void Vector3::Normalize()
{
	double mag = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	x = x / mag;
	y = y / mag;
	z = z / mag;
}
float Vector3::DistanceVec3(Vector3 dst, Vector3 src)
{
	return sqrtf(powf(dst.x - src.x, 2) + powf(dst.y - src.y, 2) + powf(dst.z - src.z, 2));
}