#pragma once
#include <memory>
#include <HAPI_lib.h>
#include <chrono>

#include "Vector3.h"

using namespace HAPISPACE;

class Object;
class PlayerObject;
class Visualisation;
class Map;
class DoorObject;
class EnemyObject;
class Audio;
class StatBar;
class RoamingEnemyObject;
class ChasingEnemyObject;
class BossEnemyObject;
class BulletObject;
class ExplosionObject;
class Button;

enum class DoorDirection;
enum class ObjectTag;

enum class Difficulty {
	E_EASY,
	E_NORMAL,
	E_HARD
};

class World {
private:
	std::vector<std::shared_ptr<Object>> m_worldObjects;
	std::shared_ptr<PlayerObject> m_playerObject;
	std::vector<std::shared_ptr<BulletObject>> m_bulletPool;
	std::vector<std::shared_ptr<ExplosionObject>> m_explosionPool;
	std::vector<std::shared_ptr<EnemyObject>> m_enemyPool;
	std::vector<std::shared_ptr<RoamingEnemyObject>> m_roamingEnemyPool;
	std::vector<std::shared_ptr<ChasingEnemyObject>> m_chasingEnemyPool;
	std::shared_ptr<BossEnemyObject> m_bossEnemy;
	std::shared_ptr<Map> m_map;
	std::shared_ptr<Audio> m_audio;
	std::shared_ptr<StatBar> m_playerHealthBar;
	std::shared_ptr<Button> m_resumeButton;
	std::shared_ptr<Button> m_restartButton;
	std::shared_ptr<Button> m_quitButton;
	std::shared_ptr<Button> m_easyButton;
	std::shared_ptr<Button> m_normalButton;
	std::shared_ptr<Button> m_hardButton;
	int m_enemyCount{ 0 };
	int m_otherExtraInstanceCount{ 0 };
	bool m_isPaused{ false };
	size_t m_worldStartSize;
	size_t m_points{ 0 };
	size_t m_highscore;
	Difficulty m_difficulty{ Difficulty::E_NORMAL };
	bool m_isPlaying{ false };
	Vector3 m_cameraPosition;
	std::chrono::steady_clock::time_point m_lastUpdate;
	std::chrono::steady_clock::time_point m_lastAnimation;
public:
	World();
	void Run();
	void SpawnBullet(Vector3& p, Vector3& v, ObjectTag t);
	void SpawnExplosion(const Vector3& p);
	void MoveRoom(DoorDirection& d);
	const Vector3& GetEnemyPosition(size_t i);
	const Vector3& GetPlayerPosition();
	void GainPoints(const size_t p);
	void PlayHitSound();
private:
	void MasterRender(Visualisation& v, float s);
	bool CheckAllEnemiesDead();
	void SpawnEnemies();
	void ResetWorld();
	void CleaUpRuntimeObjects();
	void SavePoints();
};
