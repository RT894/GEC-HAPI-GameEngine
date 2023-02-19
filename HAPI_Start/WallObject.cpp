#include "WallObject.h"
#include "PlayerObject.h"
#include "Vector3.h"
#include "Rectangle.h"
#include "Visualisation.h"
#include "World.h"

void WallObject::Update(World& w)
{
	//Walls don't need to update
}

void WallObject::CheckCollision(std::vector<std::shared_ptr<Object>>& o, std::shared_ptr<PlayerObject>& p, World& w)
{
	//Doesn't need to do anything if a wall hits something
}