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

void World::update(WormsByName worm)
{
	Point_t auxPoint;
	speed_t auxSpeed;
	
	Worm* worm_ptr = this->getWorm(worm);
	if (worm_ptr == NULL) return;

		worm_ptr->update();

		auxSpeed = worm_ptr->getSpeed();
		auxPoint = *worm_ptr->getCurrentPosition();

		auxPoint.x += auxSpeed.x;
		auxPoint.y += auxSpeed.y;
		if (auxPoint.y > 0.0)
		{
			worm_ptr->setTouchingFloor(true);
			auxPoint.y = (coord_t) 0.0;
		}
		if (!worm_ptr->getTouchingFloor())
		{
			auxSpeed.y += GRAVITY;
		}

		worm_ptr->setPosition(auxPoint);
		worm_ptr->setSpeed(auxSpeed);
		

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
#ifdef DEBUG
	std::cout << "Warm up.." << std::endl;
#endif // DEBUG
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
