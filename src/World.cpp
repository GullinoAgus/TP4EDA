#include <iostream>
#include "World.h"


World::World(Point_t worldSize)
{
	Point_t initPos = { 0 };
	worldSize.x = fabs(worldSize.x);
	worldSize.y = fabs(worldSize.y);

	initPos.x = 10.0 + WORM_WIDTH;
	initPos.y = 0;
	wormArr[0] = Worm(initPos, worldSize, RIGHT);
	
	initPos.x = worldSize.x - 10.0 - WORM_WIDTH;
	initPos.y = 0;
	wormArr[1] = Worm(initPos, worldSize, LEFT);

	return;
}

void World::update()
{
	Point_t auxPoint;
	speed_t auxSpeed;
	for (int i = 0; i < MAX_WORMS; i++)
	{
		this->wormArr[i].update();

		auxSpeed = this->wormArr[i].getSpeed();
		auxPoint = *this->wormArr[i].getCurrentPosition();

		auxPoint.x += auxSpeed.x;
		auxPoint.y += auxSpeed.y;
		if (auxPoint.y > 0.0)
		{
			wormArr[i].setTouchingFloor(true);
			auxPoint.y = (coord_t) 0.0;
		}
		if (!this->wormArr[i].getTouchingFloor())
		{
			auxSpeed.y += GRAVITY;
		}

		this->wormArr[i].setPosition(auxPoint);
		this->wormArr[i].setSpeed(auxSpeed);
		
	}

#ifdef  DEBUG
	std::cout 
		<< "Isaac: "
		<< "\n\tState: " << getWorm(WormsByName::Isaac)->getState()
		<< "\n\tFrame: " << getWorm(WormsByName::Isaac)->getFrame()
		/*<< "\nRay: "
		<< "\n\tState: " << getWorm(WormsByName::Ray)->getState()
		<< "\n\tFrame: " << getWorm(WormsByName::Ray)->getFrame()*/
		<< std::endl;
#endif //  DEBUG
}

Worm* World::getWormArr(void)
{
	return this->wormArr;
}

void World::warmUpWorm(WormsByName name, WormActions action, WormPointing direction)
{
	std::cout << "Warm up.." << std::endl;
	Worm* worm = getWorm(name);
	if (worm == NULL) return;

	WormStatesType state = IDLE;
	if (action == WormActions::JUMP)
	{
		state = WARM_JUMP;
	}
	else if (action == WormActions::WALK)
	{
		state = WARM_MOVE;
	}

	if (worm->getState() == IDLE)
	{
		if (state != WARM_JUMP)
		{
			worm->setPointingDirection(direction);
		}
		worm->setState(state);
	}

	//worm->update();

	return;
}

bool World::forceWormStop(WormsByName name)
{
	Worm* worm = getWorm(name);
	if (worm == NULL) return false;
	worm->setState(IDLE);

	return true;
}

Worm* World::getWorm(WormsByName name)
{
	if (name == WormsByName::Isaac)
	{
		return &this->wormArr[0];
	}
	else if (name == WormsByName::Ray)
	{
		return &this->wormArr[1];
	}
	else
	{
		return NULL;
	}
}
