#pragma once
#include <HAPI_lib.h>

#include "Room.h"

struct MapCoords {
	size_t m_x;
	size_t m_y;
};

class Map {
private:
	//std::vector<Room> m_rooms;
	//std::shared_ptr<Room> m_startRoom;
	//std::shared_ptr<Room> m_currentRoom;
	size_t m_currentRoomX{ 2 };
	size_t m_currentRoomY{ 2 };
	Room m_rooms[5][5];
public:
	Map();
	void GenerateMap();
	Room& GetCurrentRoom() { return m_rooms[m_currentRoomY][m_currentRoomX]; };
	void StepRoom(RoomDirection d);
};