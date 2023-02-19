#pragma once
#include <HAPI_lib.h>

#include "EnemyObject.h"

class ChasingEnemyObject : public EnemyObject {
public:
	ChasingEnemyObject(std::pair<int, int> h, std::string k, float x = 0.0f, float y = 0.0f, float z = 0.0f, int m = 0, ObjectTag t = ObjectTag::E_ENEMY, bool a = true);
	void Update(World& w);
	void CheckCollision(std::vector<std::shared_ptr<Object>>& o, std::shared_ptr<PlayerObject>& p, World& w);
	void UpdateState(const Vector3& p);
};