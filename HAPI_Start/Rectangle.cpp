#include "Rectangle.h"
#include "Vector3.h"

Rectangle::Rectangle(int l, int r, int t, int b)
{
	m_left = l;
	m_right = r;
	m_top = t;
	m_bottom = b;
}

void Rectangle::Clip(const Rectangle& r)
{
	//Clips this rect to another given rect
	//If the current bounds of this rect exceeds the bounds of the other, they are matched
	if (m_left < r.m_left) {
		m_left = r.m_left;
	}

	if (m_right > r.m_right) {
		m_right = r.m_right;
	}

	if (m_top < r.m_top) {
		m_top = r.m_top;
	}

	if (m_bottom > r.m_bottom) {
		m_bottom = r.m_bottom;
	}
}

void Rectangle::Translate(const Vector3& v)
{
	m_left += (int)std::round(v.GetX());
	m_right += (int)std::round(v.GetX());
	m_top += (int)std::round(v.GetY());
	m_bottom += (int)std::round(v.GetY());
}

bool Rectangle::IsOverlap(const Rectangle& r)
{
	//Checks for an overlap between itself and a given rectangle
	//Used mainly for collision detection
	bool leftOverlap{ false };
	bool rightOverlap{ false };
	bool topOverlap{ false };
	bool bottomOverlap{ false };

	if (m_left >= r.m_left && m_left <= r.m_right) {
		leftOverlap = true;
	}

	if (m_right >= r.m_left && m_right <= r.m_right) {
		rightOverlap = true;
	}

	if (m_top >= r.m_top && m_top <= r.m_bottom) {
		topOverlap = true;
	}

	if (m_bottom >= r.m_top && m_bottom <= r.m_bottom) {
		bottomOverlap = true;
	}

	

	return (topOverlap || bottomOverlap) && (leftOverlap || rightOverlap);
}
