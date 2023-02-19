#pragma once
#include <HAPI_lib.h>

#include "Object.h"
//#include "CharacterObject.h"

class PlayerObject : public Object {
private:
	DWORD m_currentTime{ 0 };
	DWORD m_shotTime{ 0 };
	DWORD m_shotCooldown{ 250 };
	int m_health{ 10 };
	int m_maxHealth{ 10 };
public:
	PlayerObject(std::pair<int, int> h, std::string k, float x = 0.0f, float y = 0.0f, float z = 0.0f, int m = 0, ObjectTag t = ObjectTag::E_NEUTRAL, bool a = true);
	void Update(World& w);
	void CheckCollision(std::vector<std::shared_ptr<Object>>& o, std::shared_ptr<PlayerObject>& p, World& w);
	bool TakeDamage(unsigned int d);
	int GetHealth() { return m_health; };
	void Reset();
};