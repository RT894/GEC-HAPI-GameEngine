#include "BossEnemyObject.h"
#include "Rectangle.h"
#include "Object.h"
#include "PlayerObject.h"
#include "World.h"
#include "Visualisation.h"

BossEnemyObject::BossEnemyObject(std::pair<int, int> h, std::string k, float x, float y, float z, int m, ObjectTag t, bool a) : EnemyObject(h, k, x, y, z, m, t, a, 50, 1.0f)
{
	//m_health = 50;
	m_sprayDirection = Vector3(0.0f, 1.0f, 0.0f);
	m_lastSpray = HAPI.GetTime();

	for (size_t i = 0; i < 5; i++)
	{
		LandingSpots temp;
		temp.m_hasLanded = true;
		temp.m_landingPosition = Vector3((float)(std::rand() % 824 + 100), (float)(std::rand() % 568 + 100), 0.0f);
		temp.m_startTime = 0;
		m_spots.push_back(temp);
	}
}

void BossEnemyObject::Update(World& w)
{
	//Boss has the ability to fire "missiles"
	//These are simply a 64*64 section of the world with a timer
	//When the timer runs out, CheckCollision() will check to see if the player falls within this area and will damage the player if it does
	//Each update, a random shot is chosen
	//If the shot has already landed, it is launched again at a new position, resetting the timer
	int nextShot = std::rand() % 5;

	if (m_spots[nextShot].m_hasLanded == true) {
		m_spots[nextShot].m_hasLanded = false;
		m_spots[nextShot].m_startTime = HAPI.GetTime();
		m_spots[nextShot].m_landingPosition = Vector3((float)(std::rand() % 824 + 100), (float)(std::rand() % 568 + 100), 0.0f);
	}

	UpdateState(w.GetPlayerPosition());

	//The boss can also shoot a rotating spray of bullets when in the ATTACKING state
	//Shoots the bullet at a set velocity
	//The vector holding this velocity is then rotated for the next bullet
	if (m_currentState == State::E_ATTACKING) {
		DWORD currentTime = HAPI.GetTime();
		
		if (currentTime - m_shotTime >= 100) {
			m_shotTime = currentTime;
			w.SpawnBullet(*m_position, m_sprayDirection, ObjectTag::E_ENEMY_BULLET);

			m_sprayDirection.Rotate(0.0872665f);
		}
	}
}

void BossEnemyObject::CheckCollision(std::vector<std::shared_ptr<Object>>& o, std::shared_ptr<PlayerObject>& p, World& w)
{
	//Checks to see if the player falls within the hitbox of any active shots and damages the player if so
	if (p->GetIsActive() == true) {
		Rectangle playerHitbox((int)p->GetPosition()->GetX(), (int)p->GetPosition()->GetX() + p->GetHitbox().first, (int)p->GetPosition()->GetY(), (int)p->GetPosition()->GetY() + p->GetHitbox().second);

		for (size_t i = 0; i < 5; i++) {
			DWORD currentTime = HAPI.GetTime();
			DWORD timeElapsed = currentTime - m_spots[i].m_startTime;
			if (timeElapsed >= (DWORD)2000 && m_spots[i].m_hasLanded == false) {
				m_spots[i].m_hasLanded = true;

				w.SpawnExplosion(m_spots[i].m_landingPosition);

				Rectangle shotBox((int)m_spots[i].m_landingPosition.GetX(), (int)m_spots[i].m_landingPosition.GetX() + 64, (int)m_spots[i].m_landingPosition.GetY(), (int)m_spots[i].m_landingPosition.GetY() + 64);

				if (playerHitbox.IsOverlap(shotBox) == true) {
					p->TakeDamage(1);
				}
			}
		}
	}
}

void BossEnemyObject::UpdateState(const Vector3& p)
{
	//Figures out how long it has been since the boss switched states
	//If this exceeds a set time, the state swaps between stationary and attack
	DWORD currentTime = HAPI.GetTime();
	DWORD timeDiff = currentTime - m_lastSpray;

	if (timeDiff >= (DWORD)10000 && m_currentState == State::E_STATIONARY) {
		m_currentState = State::E_ATTACKING;
		m_lastSpray = HAPI.GetTime();
	}
	else if (timeDiff >= (DWORD)10000 && m_currentState == State::E_ATTACKING) {
		m_currentState = State::E_STATIONARY;
		m_lastSpray = HAPI.GetTime();
	}
}

//The boss renders in a unique way
//The game must render both the boss itself and all of the bosses missiles
bool BossEnemyObject::Render(Visualisation& v, float s, Vector3& c)
{
	//Won't render if the object is inactive
	if (m_isActive == false) {
		return true;
	}

	//Lerps the object between where it was the previous update and where it will be the next update
	Vector3 lerpPosition;
	lerpPosition.Lerp(*m_position - c, (*m_position + (*m_velocity * m_moveSpeed)) - c, s);

	const Vector3 lerped = lerpPosition;

	if (!v.RenderTexture(lerped, m_spriteKey, m_currentFrame)) {
		HAPI.UserMessage("Texture Does Not Exist In Visualisation", "ERROR");
		HAPI.Close();
		return false;
	}

	for (size_t i = 0; i < 5; i++)
	{
		if (m_spots[i].m_hasLanded == false) {
			if (!v.RenderTexture(m_spots[i].m_landingPosition - c, "Target", 0)) {
				HAPI.UserMessage("Texture Does Not Exist In Visualisation", "ERROR");
				HAPI.Close();
				return false;
			}
		}
	}

	return true;
}
