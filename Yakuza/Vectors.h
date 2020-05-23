#pragma once

#pragma once
#include <cmath>
#include <algorithm>

#define M_PI	3.14159265358979323846264338327950288419716939937510

//Vector2
class Vector2
{
public:
	Vector2() : x(0.f), y(0.f)
	{

	}

	Vector2(float _x, float _y) : x(_x), y(_y)
	{

	}
	~Vector2()
	{

	}
	Vector2 operator-(Vector2 v)
	{
		return Vector2(x - v.x, y - v.y);
	}

	float x;
	float y;
};

//Vector3
class Vector3
{
public:
	Vector3() : x(0.f), y(0.f), z(0.f)
	{

	}

	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{

	}
	~Vector3()
	{

	}

	float x;
	float y;
	float z;

	inline float Dot(Vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline float Distance(Vector3 v)
	{
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}
	inline float Length()
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}

	Vector3 operator=(Vector3 v) 
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	Vector3 operator*=(int i) 
	{
		x *= i;
		y *= i;
		z *= i;
		return *this;
	}

	Vector3 operator+(Vector3 v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(Vector3 v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator-=(const float f)
	{
		x -= f;
		y -= f;
		z -= f;
		return *this;
	}
	Vector3 operator-(const float f)
	{
		x -= f;
		y -= f;
		z -= f;
		return *this;
	}

	Vector3 operator*(float number) const {
		return Vector3(x * number, y * number, z * number);
	}

	Vector3& operator-=(const Vector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	Vector3 operator/=(const float f) {
		x /= f;
		y /= f;
		z /= f;
		return *this;
	}

	void clamp()
	{
		if (this->x > 180.f)
			this->x -= 360.f;

		else if (this->x < -180.f)
			this->x += 360.f;

		if (this->z > 180.f)
			this->z -= 360.f;

		else if (this->z < -180.f)
			this->z += 360.f;

		if (this->x < -89.f)
			this->x = -89.f;

		if (this->x > 89.f)
			this->x = 89.f;

		while (this->z < -180.0f)
			this->z += 360.0f;

		while (this->z > 180.0f)
			this->z -= 360.0f;
	}

};

//Vector4
class Vector4
{
public:
	Vector4() : x(0.f), y(0.f), z(0.f), w(0.f)
	{

	}

	Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
	{

	}
	~Vector4()
	{

	}


	float x;
	float y;
	float z;
	float w;

	inline float Length()
	{
		return sqrt((x * x) + (y * y) + (z * z) + (w * w));
	}

	Vector4 operator-(const float f)
	{
		x -= f;
		y -= f;
		z -= f;
		return *this;
	}

	Vector4 operator-(const Vector4 v) 
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	Vector4 operator/(const Vector4 v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}
	Vector4 operator/=(const float f)
	{
		x /= f;
		y /= f;
		z /= f;
		w /= f;
		return *this;
	}

	Vector4 operator/(const float f) 
	{
		x /= f;
		y /= f;
		z /= f;
		w /= f;
		return *this;
	}

	Vector4 operator+(const Vector4 v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	Vector4 operator*=(const int i)
	{
		x *= i;
		y *= i;
		z *= i;
		w *= i;
		return *this;
	}

	Vector4 operator+=(const Vector4 v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}
};

class Vector4Aimbot
{
public:
public:
	float mData[4];
	float w()
	{
		return mData[3];
	}
	float x()
	{
		return mData[0];
	}
	float y()
	{
		return mData[1];
	}
	float z()
	{
		return mData[2];
	}
};