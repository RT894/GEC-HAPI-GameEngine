#include "DoorObject.h"
#include "Rectangle.h"
#include "Vector3.h"
#include "PlayerObject.h"
#include "World.h"

DoorObject::DoorObject(DoorDirection d, std::pair<int, int> h, std::string k, float x, float y, float z, int m, ObjectTag t, bool a) : Object(h, k, x, y, z, m, t, a)
{
	m_tag = ObjectTag::E_DOOR;
	m_direction = d;
	//m_health = 99999999;
}

void DoorObject::Update(World& w)
{
	//Doesn't move so doesn't have to update
}

void DoorObject::CheckCollision(std::vector<std::shared_ptr<Object>>& o, std::shared_ptr<PlayerObject>& p, World& w)
{
	//Calls world function to move rooms if a collision with the player is made
	Rectangle myHitbox((int)m_position->GetX(), (int)m_position->GetX() + m_hitboxDimensions.first, (int)m_position->GetY(), (int)m_position->GetY() + m_hitboxDimensions.second);

	Rectangle playerHitbox((int)p->GetPosition()->GetX(), (int)p->GetPosition()->GetX() + p->GetHitbox().first, (int)p->GetPosition()->GetY(), (int)p->GetPosition()->GetY() + p->GetHitbox().second);

	if (myHitbox.IsOverlap(playerHitbox) == true || playerHitbox.IsOverlap(myHitbox) == true) {
		w.MoveRoom(m_direction);
	}
}

//Used for debugging incase a door is removed from the world
//THIS SHOULD NEVER HAPPEN
DoorObject::~DoorObject()
{
	std::cout << "Door Removed" << std::endl;
}
