#pragma once
#include <memory>
#include <HAPI_lib.h>

using namespace HAPISPACE;

class Vector3;
class Visualisation;
class PlayerObject;
class World;
class EnemyObject;

enum class ObjectTag {
	E_FRIENDLY,
	E_ENEMY,
	E_NEUTRAL,
	E_DOOR,
	E_FRIENDLY_BULLET,
	E_ENEMY_BULLET,
	E_PARTICLE
};

class Object {
protected:
	std::shared_ptr<Vector3> m_position;
	std::shared_ptr<Vector3> m_velocity;
	float m_moveSpeed{ 5.0f };
	int m_currentFrame{ 0 };
	int m_maxFrame;
	std::pair<int, int> m_hitboxDimensions;
	std::string m_spriteKey;
	bool m_isActive{ true };
	ObjectTag m_tag;
	//int m_health{ 10 };
public:
	Object(std::pair<int, int> h, std::string k, float x = 0.0f, float y = 0.0f, float z = 0.0f, int m = 0, ObjectTag t = ObjectTag::E_NEUTRAL, bool a = true, float s = 0.0f);
	void SetPosition(const Vector3& v);
	const std::shared_ptr<Vector3>& GetPosition() const { return m_position; };
	void Translate(Vector3& v);
	const int GetCurrentFrame() const { return m_currentFrame; };
	void SetCurrentFrame(int f);
	const int GetMaxFrame() const { return m_maxFrame; };
	const std::pair<int, int>& GetHitbox() const { return m_hitboxDimensions; };
	const std::string& GetSpriteKey() const { return m_spriteKey; };
	const std::shared_ptr<Vector3>& GetVelocity() const { return m_velocity; };
	const ObjectTag GetTag() const { return m_tag; };
	const bool GetIsActive() const { return m_isActive; };
	const float GetMoveSpeed() const { return m_moveSpeed; };
	//const int GetHealth() const { return m_health; };
	void SetActive(bool a);
	void SetVelocity(const Vector3& v);
	virtual bool Render(Visualisation& v, float s, Vector3& c);
	//bool TakeDamage(unsigned int d);
	virtual void Update(World& w) = 0;
	virtual void CheckCollision(std::vector<std::shared_ptr<Object>>& o, std::shared_ptr<PlayerObject>& p, World& w) = 0;
};