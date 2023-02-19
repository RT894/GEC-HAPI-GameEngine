#include "Button.h"
#include "Rectangle.h"
#include "Visualisation.h"

Button::Button(Vector3& pos, unsigned int width, unsigned int height, std::string message, HAPISPACE::HAPI_TColour colour, bool active)
{
	m_screenPosition = pos;
	m_width = width;
	m_height = height;
	m_message = message;
	m_colour = colour;
	m_isActive = active;
}

//p = the position of the mouse
//Returns true if the mouse is over the bounds of the button
bool Button::HasClick(Vector3& pos)
{
	Rectangle buttonRect((int)m_screenPosition.GetX(), (int)m_screenPosition.GetX() + m_width, (int)m_screenPosition.GetY(), (int)m_screenPosition.GetY() + m_width);

	if (pos.GetX() > buttonRect.m_left && pos.GetX() < buttonRect.m_right && pos.GetY() > buttonRect.m_top && pos.GetY() < buttonRect.m_bottom) {
		return true;
	}
	else {
		return false;
	}
}

//Renders a default square of a given colour and text with a set message
void Button::Render(Visualisation& visual)
{
	if (m_isActive == false) {
		return;
	}

	visual.RenderDefault(m_screenPosition, m_width, m_height, m_colour);
	HAPI.RenderText((int)m_screenPosition.GetX(), (int)m_screenPosition.GetY(), HAPISPACE::HAPI_TColour::BLACK, m_message);
}

void Button::SetIsActive(bool active)
{
	m_isActive = active;
}
