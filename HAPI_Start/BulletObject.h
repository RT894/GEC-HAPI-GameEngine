#pragma once

#include <HAPI_lib.h>

#include "Object.h"

class BulletObject : public Object {
private:
	float m_lifeTime{ 0.0f };
public:
	BulletObject(std::pair<int, int> h, std::string k, float x = 0.0f, float y = 0.0f, float z = 0.0f, int m = 0, ObjectTag t = ObjectTag::E_NEUTRAL, bool a = true) : Object(h, k, x, y, z, m, t, a, 7.0f) {};
	void Update(World& w);
	void CheckCollision(std::vector<std::shared_ptr<Object>>& o, std::shared_ptr<PlayerObject>& p, World& w);
	void ResetTimer();
};