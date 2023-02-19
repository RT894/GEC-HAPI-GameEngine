#pragma once

#include <HAPI_lib.h>

#include "Object.h"

class WallObject : public Object {
public:
	WallObject(std::pair<int, int> h, std::string k, float x = 0.0f, float y = 0.0f, float z = 0.0f, int m = 0, ObjectTag t = ObjectTag::E_NEUTRAL, bool a = true) : Object(h, k, x, y, z, m, t, a) {};
	void Update(World& w);
	void CheckCollision(std::vector<std::shared_ptr<Object>>& o, std::shared_ptr<PlayerObject>& p, World& w);
};