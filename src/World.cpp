#include <iostream>
#include "World.h"

World::World(Point_t worldSize)
{
	Point_t initPos = { 0 };
	worldSize.x = fabs(worldSize.x);
	worldSize.y = fabs(worldSize.y);

	initPos.x = 10;
	initPos.y = HORIZONTAL;
	wormArr[0] = Worm(initPos, worldSize, RIGHT);
	
	initPos.x = worldSize.x - 10 - WORM_WIDTH;
	initPos.y = HORIZONTAL;
	wormArr[1] = Worm(initPos, worldSize, LEFT);

	return;
}

void World::update() {
	
	Point_t auxPoint;
	speed_t auxSpeed;
	for (int i = 0; i < MAX_WORMS; i++)
	{
		auxSpeed = this->wormArr[i].getSpeed();
		auxPoint = *this->wormArr[i].getCurrentPosition();

		auxPoint.x += auxSpeed.x;
		auxPoint.y += auxSpeed.y;
		if (auxPoint.y - WORM_HEIGHT < HORIZONTAL)
		{
			wormArr[i].setTouchingFloor(true);
			auxPoint.y = (coord_t) HORIZONTAL + WORM_HEIGHT;
		}
		auxSpeed.y -= GRAVITY;

		this->wormArr[i].setPosition(auxPoint);
		this->wormArr[i].setSpeed(auxSpeed);
	}
	
}

Worm* World::getWormArr(void)
{
	return this->wormArr;
}

