#pragma once

class Vector3 {
private:
	float m_x = 0.0f;
	float m_y = 0.0f;
	float m_z = 0.0f;
public:
	Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);
	float GetX() const;
	float GetY() const;
	float GetZ() const;
	void Normalize();
	void Invert();
	void Lerp(Vector3& a, Vector3& b, float s);
	float Distance(Vector3& a, Vector3& b);
	void Rotate(float a);

	friend Vector3 operator+(Vector3& v1, Vector3& v2);
	friend Vector3 operator*(Vector3& v, float n);
	friend Vector3 operator-(Vector3& v1, Vector3& v2);
};

inline Vector3 operator+(Vector3& v1, Vector3& v2) {
	Vector3 result;
	result.m_x = v1.m_x + v2.m_x;
	result.m_y = v1.m_y + v2.m_y;
	result.m_z = v1.m_z + v2.m_z;

	return result;
}

inline Vector3 operator*(Vector3& v, float n) {
	Vector3 result;
	result.m_x = v.m_x * n;
	result.m_y = v.m_y * n;
	result.m_z = v.m_z * n;

	return result;
}

inline Vector3 operator-(Vector3& v1, Vector3& v2) {
	Vector3 result;
	result.m_x = v1.m_x - v2.m_x;
	result.m_y = v1.m_y - v2.m_y;
	result.m_z = v1.m_z - v2.m_z;
	
	return result;
}