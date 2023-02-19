#include "PlayerObject.h"
#include "Vector3.h"
#include "Rectangle.h"
#include "Visualisation.h"
#include "World.h"

PlayerObject::PlayerObject(std::pair<int, int> h, std::string k, float x, float y, float z, int m, ObjectTag t, bool a) : Object(h, k, x, y, z, m, t, a, 10.0f)
{
	//m_moveSpeed = 10.0f;
}

void PlayerObject::Update(World& w)
{
	if (m_isActive == false) {
		return;
	}

	m_currentTime = HAPI.GetTime();
	const HAPI_TKeyboardData& keyData = HAPI.GetKeyboardData();

	const HAPI_TControllerData& conData = HAPI.GetControllerData(0);

	const HAPI_TMouseData& mouseData = HAPI.GetMouseData();

	Vector3 playerMove(0.0f, 0.0f, 0.0f);
	Vector3 target(0.0f, 0.0f, 0.0f);

	//Checks to see if the controller is plugged in or if a keyboard and mouse is being used
	if (conData.isAttached) {
		//Controller input
		if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] > 10000 || conData.digitalButtons[HK_DIGITAL_DPAD_UP] == true) {
			playerMove = playerMove + Vector3(0.0f, -5.0f, 0.0f);
		}
		else if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -10000 || conData.digitalButtons[HK_DIGITAL_DPAD_DOWN] == true) {
			playerMove = playerMove + Vector3(0.0f, 5.0f, 0.0f);
		}

		if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > 10000 || conData.digitalButtons[HK_DIGITAL_DPAD_RIGHT] == true) {
			playerMove = playerMove + Vector3(5.0f, 0.0f, 0.0f);
		}
		else if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -10000 || conData.digitalButtons[HK_DIGITAL_DPAD_LEFT] == true) {
			playerMove = playerMove + Vector3(-5.0f, 0.0f, 0.0f);
		}

		Vector3 aimPos = Vector3((float)conData.analogueButtons[HK_ANALOGUE_RIGHT_THUMB_X], (float)-conData.analogueButtons[HK_ANALOGUE_RIGHT_THUMB_Y], 0.0f);
		target = aimPos - *m_position;
		target.Normalize();

		if (conData.digitalButtons[HK_DIGITAL_X] == true && (m_currentTime - m_shotTime) >= m_shotCooldown) {
			std::cout << "X: " << target.GetX() << " Y: " << target.GetY() << std::endl;
			w.SpawnBullet(*m_position, target, ObjectTag::E_FRIENDLY_BULLET);
			m_shotTime = HAPI.GetTime();
		}
	}
	else {
		//Keyboard input
		if (keyData.scanCode[HK_UP] || keyData.scanCode['W']) {
			playerMove = playerMove + Vector3(0.0f, -5.0f, 0.0f);
		} 
		else if (keyData.scanCode[HK_DOWN] || keyData.scanCode['S']) {
			playerMove = playerMove + Vector3(0.0f, 5.0f, 0.0f);
		} 
		else if (keyData.scanCode[HK_LEFT] || keyData.scanCode['A']) {
			playerMove = playerMove + Vector3(-5.0f, 0.0f, 0.0f);
		} 
		else if (keyData.scanCode[HK_RIGHT] || keyData.scanCode['D']) {
			playerMove = playerMove + Vector3(5.0f, 0.0f, 0.0f);
		}
		

		Vector3 mousePos = Vector3((float)mouseData.x, (float)mouseData.y, 0.0f);
		target = mousePos - Vector3(1024.0f / 2, 768.0f / 2, 0.0f);
		target.Normalize();

		if (mouseData.leftButtonDown && (m_currentTime - m_shotTime) >= m_shotCooldown) {
			w.SpawnBullet(*m_position, target, ObjectTag::E_FRIENDLY_BULLET);
			m_shotTime = HAPI.GetTime();
		}
	}

	//Normalises the player movement
	playerMove.Normalize();
	*m_velocity = playerMove;

	//Update position
	Translate(*m_velocity * m_moveSpeed);
}

void PlayerObject::CheckCollision(std::vector<std::shared_ptr<Object>>& o, std::shared_ptr<PlayerObject>& p, World& w)
{
	//Checks for any collisions using rectangles
	Rectangle myHitbox((int)m_position->GetX(), (int)m_position->GetX() + m_hitboxDimensions.first, (int)m_position->GetY(), (int)m_position->GetY() + m_hitboxDimensions.second);

	for (std::shared_ptr<Object> object : o) {
		if (object->GetIsActive() == false) {
			continue;
		}

		Rectangle otherHitbox((int)object->GetPosition()->GetX(), (int)object->GetPosition()->GetX() + object->GetHitbox().first, (int)object->GetPosition()->GetY(), (int)object->GetPosition()->GetY() + object->GetHitbox().second);

		//Prevents the player passing through enemies and walls
		if (myHitbox.IsOverlap(otherHitbox) == true) {
			switch (object->GetTag()) {
			case ObjectTag::E_ENEMY:
				SetPosition(*m_position - (*m_velocity * m_moveSpeed));
				TakeDamage(1);
				break;
			case ObjectTag::E_NEUTRAL:
				SetPosition(*m_position - (*m_velocity * m_moveSpeed));
				break;
			default:
				break;
			}
		}
	}
}

//Applies damage to the player by removing health from them
bool PlayerObject::TakeDamage(unsigned int d)
{
	m_health -= d;

	if (m_health <= 0) {
		m_isActive = false;
		return true;
	}

	return false;
}

//Resets the players stats to their original state
void PlayerObject::Reset()
{
	m_health = m_maxHealth;
	m_currentTime = HAPI.GetTime();
	m_shotTime = m_currentTime;
}
