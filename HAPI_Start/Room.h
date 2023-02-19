#pragma once
#include <HAPI_lib.h>

#include "Vector3.h"

//Room will act as a node in a graph, with the map acting as the overall graph
//Map will set connections, the Room is purely for holding connection and spawn point data

class Map;

using namespace HAPISPACE;

enum class RoomDirection {
	E_UP,
	E_DOWN,
	E_LEFT,
	E_RIGHT
};

struct Room {
private:
	bool m_up{ false };
	bool m_down{ false };
	bool m_left{ false };
	bool m_right{ false };
	bool m_isCleared{ false };
	bool m_isBossRoom{ false };
	//size_t m_index;
	std::vector<Vector3> m_spawnPoints;
	HAPI_TColour m_roomColour;

	friend class Map;
public:
	Room();
	bool IsReachable();
	const std::vector<Vector3>& GetSpawnPoints() { return m_spawnPoints; };
	const HAPI_TColour& GetColour() { return m_roomColour; };
	bool HasUpDoor() { return m_up; };
	bool HasDownDoor() { return m_down; };
	bool HasLeftDoor() { return m_left; };
	bool HasRightDoor() { return m_right; };
	bool GetIsCleared() { return m_isCleared; };
	bool GetIsBossRoom() { return m_isBossRoom; };
	void SetIsCleared(bool c);
};