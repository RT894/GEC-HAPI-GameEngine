#include "EnemyObject.h"

EnemyObject::EnemyObject(std::pair<int, int> h, std::string k, float x, float y, float z, int m, ObjectTag t, bool a, unsigned int health, float s) : Object(h, k, x, y, z, m, t, a, s) {
	m_health = health;
	m_maxHealth = m_health;
}

//Takes away from the enemies health and kills them if it is smaller than or equal to 0
//Returns bool value so that the game knows to spawn an explosion on death
bool EnemyObject::TakeDamage(unsigned int d)
{
	m_health -= d;

	if (m_health <= 0) {
		m_isActive = false;
		return true;
	}

	return false;
}

//Resets the enemies stats
void EnemyObject::Reset()
{
	m_health = m_maxHealth;
	m_shotTime = HAPI.GetTime();
	m_isActive = false;
}
