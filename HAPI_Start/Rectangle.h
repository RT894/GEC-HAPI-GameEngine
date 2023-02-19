#pragma once

#include <HAPI_lib.h>

using namespace HAPISPACE;

class Vector3;

class Rectangle
{
public:
	int m_left{ 0 };
	int m_right{ 0 };
	int m_top{ 0 };
	int m_bottom{ 0 };
public:
	Rectangle(int l = 0, int r = 0, int t = 0, int b = 0);
	void Clip(const Rectangle& r);
	void Translate(const Vector3& v);
	bool IsOverlap(const Rectangle& r);
};

