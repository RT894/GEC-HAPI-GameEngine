#include "Vector3.h"

#include <cmath>
#include<iostream>

Vector3::Vector3(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

void Vector3::SetX(float x)
{
	m_x = x;
}

void Vector3::SetY(float y)
{
	m_y = y;
}

void Vector3::SetZ(float z)
{
	m_z = z;
}

float Vector3::GetX() const
{
	return m_x;
}

float Vector3::GetY() const
{
	return m_y;
}

float Vector3::GetZ() const
{
	return m_z;
}

void Vector3::Normalize()
{
	float xSquared = std::pow(m_x, 2.0f);
	float ySquared = std::pow(m_y, 2.0f);
	float zSquared = std::pow(m_z, 2.0f);
	float squaredLength = xSquared + ySquared + zSquared;

	float length = std::sqrt(squaredLength);

	if (length != 0) {
		m_x /= length;

		m_y /= length;

		m_z /= length;
	}
}

void Vector3::Invert()
{
	m_x -= m_x * 2;
	m_y -= m_y * 2;
	m_z -= m_z * 2;
}

//Linear interpolation
//Mainly used when rendering between updates
void Vector3::Lerp(Vector3& a, Vector3& b, float s)
{
	if (s < 0) {
		s = 0;
	}

	if (s > 1) {
		s = 1;
	}
	Vector3 lerpResult = a * (1.0f - s) + b * s;
	m_x = lerpResult.m_x;
	m_y = lerpResult.m_y;
	m_z = lerpResult.m_z;
}

//Calculates the distance between two points
float Vector3::Distance(Vector3& a, Vector3& b)
{
	Vector3 aToB = b - a;
	float dist = std::sqrt((aToB.m_x * aToB.m_x) + (aToB.m_y * aToB.m_y) + (aToB.m_z * aToB.m_z));
	return dist;
}

//Rotates in radians about the origin
//Used mainly for the boss enemy to turn the velocity given to the bullets
void Vector3::Rotate(float a)
{
	float newX = (m_x * std::cos(a)) - (m_y * std::sin(a));
	float newY = (m_x * std::sin(a)) + (m_y * std::cos(a));

	m_x = newX;
	m_y = newY;
}
