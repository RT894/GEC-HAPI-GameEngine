#include "BulletObject.h"
#include "PlayerObject.h"
#include "EnemyObject.h"
#include "Vector3.h"
#include "Rectangle.h"
#include "Visualisation.h"
#include "World.h"

void BulletObject::Update(World& w)
{
	if (m_lifeTime >= 20.0f) {
		m_lifeTime = 0.0f;
		m_isActive = false;
		return;
	}

	//Update position
	Translate(*m_velocity * m_moveSpeed);

	m_lifeTime += 0.1f;
}

void BulletObject::CheckCollision(std::vector<std::shared_ptr<Object>>& o, std::shared_ptr<PlayerObject>& p, World& w)
{
	//Checks for any collisions using rectangles
	//Handles killing itself and damaging characters with a different tag
	Rectangle myHitbox((int)m_position->GetX(), (int)m_position->GetX() + m_hitboxDimensions.first, (int)m_position->GetY(), (int)m_position->GetY() + m_hitboxDimensions.second);

	//Only loops through the world to collide with enemies if the bullet was fired by the player
	for (std::shared_ptr<Object> object : o) {
		Rectangle otherHitbox((int)object->GetPosition()->GetX(), (int)object->GetPosition()->GetX() + object->GetHitbox().first, (int)object->GetPosition()->GetY(), (int)object->GetPosition()->GetY() + object->GetHitbox().second);

		if (myHitbox.IsOverlap(otherHitbox) == true && object->GetIsActive() == true) {
			if (object->GetTag() == ObjectTag::E_ENEMY && m_tag == ObjectTag::E_FRIENDLY_BULLET) {
				//Dowcasts the current object so that TakeDamage() can be called
				std::shared_ptr<EnemyObject> enemy = std::dynamic_pointer_cast<EnemyObject, Object>(object);
				bool isDead = enemy->TakeDamage(1);

				if (isDead == true) {
					w.SpawnExplosion(*enemy->GetPosition());
					w.GainPoints(10);
				}
				else {
					w.PlayHitSound();
				}

				m_isActive = false;
				return;
			}
			else if (object->GetTag() == ObjectTag::E_NEUTRAL) {
				m_isActive = false;
				return;
			}
		}
	}

	//Checks collision with the player only if the bullet was fired by an enemy
	if (m_tag != ObjectTag::E_FRIENDLY_BULLET) {
		Rectangle otherHitbox((int)p->GetPosition()->GetX(), (int)p->GetPosition()->GetX() + p->GetHitbox().first, (int)p->GetPosition()->GetY(), (int)p->GetPosition()->GetY() + p->GetHitbox().second);

		if (myHitbox.IsOverlap(otherHitbox) == true && p->GetIsActive() == true) {
			m_isActive = false;
			bool isDead = p->TakeDamage(1);
			if (isDead == true) {
				w.SpawnExplosion(*p->GetPosition());
			}
		}
	}
}

void BulletObject::ResetTimer()
{
	m_lifeTime = 0.0f;
}
