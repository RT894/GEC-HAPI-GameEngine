#pragma once
#include <HAPI_lib.h>

#include "Vector3.h"

class Visualisation;

class Button {
private:
	Vector3 m_screenPosition;
	unsigned int m_width;
	unsigned int m_height;
	std::string m_message;
	HAPISPACE::HAPI_TColour m_colour;
	bool m_isActive;
public:
	Button(Vector3& pos, unsigned int width, unsigned int height, std::string message = "Text", HAPISPACE::HAPI_TColour colour = HAPISPACE::HAPI_TColour::WHITE, bool active = false);
	bool HasClick(Vector3& pos);
	void Render(Visualisation& visual);
	void SetIsActive(bool active);
};