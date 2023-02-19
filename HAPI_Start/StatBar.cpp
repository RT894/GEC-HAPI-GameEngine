#include "StatBar.h"
#include "Visualisation.h"

StatBar::StatBar(Vector3& pos, unsigned int max, HAPISPACE::HAPI_TColour& colour)
{
	m_screenPosition = pos;
	m_maxValue = max;
	m_currentValue = max;
	m_colour = colour;
}

//Sets the value of the slider, which determines how long the slider should be when rendered
void StatBar::SetValue(unsigned int n)
{
	if (n > m_maxValue) {
		n = m_maxValue;
	}

	m_currentValue = n;
}

//Render a default shape with a length based on the value of the slider
void StatBar::Render(Visualisation& visual)
{
	float amountToRender = (float)m_currentValue / (float)m_maxValue;
	amountToRender *= 10;

	visual.RenderDefault(m_screenPosition, (unsigned int)amountToRender * 100, 20, m_colour);
}
