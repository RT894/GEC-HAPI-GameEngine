#pragma once
#include <HAPI_lib.h>
#include "EnemyObject.h"
#include "Vector3.h"

struct LandingSpots {
private:
	HAPISPACE::DWORD m_startTime{ 0 };
	Vector3 m_landingPosition;
	bool m_hasLanded{ true };

	friend class BossEnemyObject;
};

class BossEnemyObject : public EnemyObject {
private:
	std::vector<LandingSpots> m_spots;
	Vector3 m_sprayDirection;
	HAPISPACE::DWORD m_lastSpray{ 0 };
public:
	BossEnemyObject(std::pair<int, int> h, std::string k, float x = 0.0f, float y = 0.0f, float z = 0.0f, int m = 0, ObjectTag t = ObjectTag::E_ENEMY, bool a = true);
	void Update(World& w);
	void CheckCollision(std::vector<std::shared_ptr<Object>>& o, std::shared_ptr<PlayerObject>& p, World& w);
	void UpdateState(const Vector3& p);
	bool Render(Visualisation& v, float s, Vector3& c);
};