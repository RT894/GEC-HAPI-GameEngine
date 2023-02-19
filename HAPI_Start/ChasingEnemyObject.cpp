#include "ChasingEnemyObject.h"
#include "World.h"
#include "PlayerObject.h"

ChasingEnemyObject::ChasingEnemyObject(std::pair<int, int> h, std::string k, float x, float y, float z, int m, ObjectTag t, bool a) : EnemyObject(h, k, x, y, z, m, t, a, 5, 5.0f)
{
	//m_health = 5;

	m_targetPositions.push_back(*m_position);
	m_targetPositions.push_back(*m_position + Vector3(0.0f, 200.0f, 0.0f));
}

void ChasingEnemyObject::Update(World& w)
{
	if (m_health <= 0) {
		return;
	}

	HAPISPACE::DWORD currentTime = HAPI.GetTime();

	UpdateState(w.GetPlayerPosition());

	//Sets the target position to wherever the player is
	Vector3 playerPosition = w.GetPlayerPosition();
	Vector3 playerDirection = playerPosition - *m_position;
	playerDirection.Normalize();

	//Either follows the player, shoots the player if it's within range, or patrols between two set points
	switch (m_currentState)
	{
	case State::E_STATIONARY:
		return;
		break;
	case State::E_ATTACKING:
		if (currentTime - m_shotTime > (DWORD)1000) {
			w.SpawnBullet(*m_position, playerDirection, ObjectTag::E_ENEMY_BULLET);

			m_shotTime = HAPI.GetTime();
		}

		*m_velocity = Vector3(0.0f, 0.0f, 0.0f);
		break;
	case State::E_PATROLLING:
		*m_velocity = m_targetPositions[m_targetIndex] - *m_position;
		break;
	case State::E_CHASING:
		*m_velocity = playerDirection;
		break;
	default:
		break;
	}

	m_velocity->Normalize();

	Translate(*m_velocity * m_moveSpeed);
}

void ChasingEnemyObject::CheckCollision(std::vector<std::shared_ptr<Object>>& o, std::shared_ptr<PlayerObject>& p, World& w)
{
}

void ChasingEnemyObject::UpdateState(const Vector3& p)
{
	//Changes the state
	//If the player is very close, the enemy will attack
	//If the player is within a slightly wider range, the enemy will chase
	//Otherwise, the enemy will roam between two set points
	Vector3 playerPos = p;
	Vector3 targetDifference = m_targetPositions[m_targetIndex] - *m_position;
	Vector3 playerDifference = playerPos - *m_position;

	if (m_currentState == State::E_PATROLLING) {
		if (targetDifference.GetX() <= 20.0f && targetDifference.GetX() >= -20.0f && targetDifference.GetY() <= 20.0f && targetDifference.GetY() >= -20.0f) {
			*m_position = m_targetPositions[m_targetIndex];
			m_currentState = State::E_PATROLLING;

			m_targetIndex++;
			if (m_targetIndex >= m_targetPositions.size()) {
				m_targetIndex = 0;
			}
		}
		else {
			m_currentState = State::E_PATROLLING;
		}
	}

	if (playerDifference.GetX() <= 100.0f && playerDifference.GetX() >= -100.0f && playerDifference.GetY() <= 100.0f && playerDifference.GetY() >= -100.0f) {
		m_currentState = State::E_ATTACKING;
	}
	else if (playerDifference.GetX() <= 300.0f && playerDifference.GetX() >= -300.0f && playerDifference.GetY() <= 300.0f && playerDifference.GetY() >= -300.0f) {
		m_currentState = State::E_CHASING;
	}
	else {
		m_currentState = State::E_PATROLLING;
	}
}
