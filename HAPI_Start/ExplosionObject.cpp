#include "ExplosionObject.h"

void ExplosionObject::Update(World& w)
{
	//Sets itself to false if all frames of the explosion have been looped through
	if (m_currentFrame >= m_maxFrame - 1) {
		m_isActive = false;
	}
}

void ExplosionObject::CheckCollision(std::vector<std::shared_ptr<Object>>& o, std::shared_ptr<PlayerObject>& p, World& w)
{
	//No collisions
}
