#include "Room.h"

Room::Room()
{
	m_spawnPoints.push_back(Vector3(256.0f, 192.0f, 0.0f));
	m_spawnPoints.push_back(Vector3(768.0f, 576.0f, 0.0f));
	m_spawnPoints.push_back(Vector3(256.0f, 576.0f, 0.0f));
	m_spawnPoints.push_back(Vector3(768.0f, 192.0f, 0.0f));
	m_spawnPoints.push_back(Vector3(512.0f, 384.0f, 0.0f));

	m_roomColour = HAPI_TColour(std::rand() % 256, std::rand() % 256, std::rand() % 256, 255);
}

bool Room::IsReachable()
{
	return (m_up || m_down || m_left || m_right);
}

void Room::SetIsCleared(bool c)
{
	m_isCleared = c;
}
