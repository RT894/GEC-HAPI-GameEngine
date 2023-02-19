#pragma once
#include <HAPI_lib.h>

#include "Vector3.h"

class Visualisation;

class StatBar {
private:
	Vector3 m_screenPosition;
	unsigned int m_maxValue;
	unsigned int m_currentValue;
	HAPISPACE::HAPI_TColour m_colour;
public:
	StatBar(Vector3& pos, unsigned int max, HAPISPACE::HAPI_TColour& colour);
	void SetValue(unsigned int n);
	void Render(Visualisation& visual);
};