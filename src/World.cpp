#include "World.h"
#include <iostream>

void World::update() {
	
	Point_t auxPoint;
	speed_t auxSpeed;
	for (int i = 0; i < MAX_WORMS; i++)
	{
		auxSpeed = this->wormArr[i].getSpeed();
		auxPoint = *this->wormArr[i].getCurrentPosition();

		auxPoint.x += auxSpeed.x;
		auxPoint.y += auxSpeed.y;
		if (auxPoint.y - BITMAPHEIGHT < HORIZONTAL)
		{
			wormArr[i].setTouchingFloor(true);
			auxPoint.y = HORIZONTAL + BITMAPHEIGHT;
		}
		auxSpeed.y -= GRAVITY;

		this->wormArr[i].setPosition(auxPoint);
		this->wormArr[i].setSpeed(auxSpeed);
	}
	
}

