#include "Object.h"
#include "Vector3.h"
#include "Rectangle.h"
#include "Visualisation.h"
#include "World.h"

#include <HAPI_lib.h>

using namespace HAPISPACE;

Object::Object(std::pair<int, int> h, std::string k, float x, float y, float z, int m, ObjectTag t, bool a, float s)
{
	m_position = std::make_shared<Vector3>(x, y, z);
	m_velocity = std::make_shared<Vector3>(0.0f, 0.0f, 0.0f);
	m_maxFrame = m;
	m_hitboxDimensions = h;
	m_spriteKey = k;
	m_tag = t;
	m_isActive = a;

	if (s < 0.0f) {
		s = 0.0f;
	}

	m_moveSpeed = s;
}

void Object::SetPosition(const Vector3& v)
{
	m_position = std::make_shared<Vector3>(v);
}

void Object::Translate(Vector3& v)
{
	Vector3 temp = *m_position;
	temp = temp + v;
	m_position = std::make_shared<Vector3>(temp);
}

void Object::SetCurrentFrame(int f)
{
	//Sets current frame of the animation
	//Loops back to the first frame if "f" is too large
	if (f < 0) {
		m_currentFrame = 0;
	}
	else if (f >= m_maxFrame) {
		if (m_tag == ObjectTag::E_PARTICLE) {
			m_isActive = false;
			return;
		}
		else {
			m_currentFrame = 0;
		}
	}
	else {
		m_currentFrame = f;
	}
}

void Object::SetActive(bool a)
{
	m_isActive = a;
}

void Object::SetVelocity(const Vector3& v)
{
	Vector3 newVelocity = v;
	m_velocity = std::make_shared<Vector3>(newVelocity);
}

bool Object::Render(Visualisation& v, float s, Vector3& c)
{
	//Won't render if the object is inactive
	if (m_isActive == false) {
		return true;
	}

	//Lerps the object between where it was the previous update and where it will be the next update
	Vector3 lerpPosition;
	lerpPosition.Lerp(*m_position - c, (*m_position + (*m_velocity * m_moveSpeed)) - c, s);

	const Vector3 lerped = lerpPosition;

	if (!v.RenderTexture(lerped, m_spriteKey, m_currentFrame)) {
		HAPI.UserMessage("Texture Does Not Exist In Visualisation", "ERROR");
		HAPI.Close();
		return false;
	}

	return true;
}