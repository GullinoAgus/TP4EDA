#include <iostream>
#include <cmath>    /* fabs */
#include <limits>   /* numeric_limits */

#include "Worm.h"



#define JUMP_INIT_SPEED (4.5)

#define COS60           (0.5)                   /* cos(60�) == cos(pi/3) */
#define SIN60           (0.866025403784438)     /* sin(60�) == sin(pi/3) */

#define LATERAL_MOVE_SPEED 27.0
#define FPS 50
#define CONFIRMATION_FRAMES 5
#define WALK_WARM_UP_FRAMES 3
#define JUMP_WARM_UP_FRAMES 4

#define JUMP_X_COORD    (JUMP_INIT_SPEED * COS60 * this->jumpTime)
#define JUMP_Y_COORD    (JUMP_INIT_SPEED * SIN60 * this->jumpTime \
                        - this->worldGravity * this->jumpTime * this->jumpTime  / 2)

bool compareDouble(double a, double b) 
{
    return fabs(a - b) < std::numeric_limits<double>::epsilon();
}

Worm::Worm(void) : position() 
{
    this->pointingDirection = NOT_INIT;
	this->frame = INVALID;
	this->state = IDLE;
	this->frameCounter = 0;
    this->spd = { 0 };
}

Worm::Worm(Point_t initPosition, Point_t maxPosition, 
    WormPointing direction) 
    : position(initPosition.x, initPosition.y, maxPosition.x, maxPosition.y) 
{
    this->pointingDirection = NOT_INIT;
	this->state = IDLE;
	this->frameCounter = 0;
	this->spd = { 0 };

	this->setInitialPointingDirection(direction);
	this->frame = WF1;
}

Worm::~Worm() 
{
    this->pointingDirection = NOT_INIT;
	this->frameCounter = 0;
	this->state = IDLE;
	this->spd = { 0 };
}

bool Worm::setMaximumPosition(Point_t pos)
{
    return position.setMaximumCoordinates(pos.x, pos.y);
}

bool Worm::setPosition(Point_t pos)
{
    return position.setPosition(pos.x, pos.y);
}

bool Worm::setInitialPointingDirection(WormPointing dir)
{
	if (dir == LEFT || dir == RIGHT) 
	{
		this->pointingDirection = dir;
	}
	else
	{
		std::cout << "Invalid direction!" << std::endl;
		return false;
	}

    return true;
}

void Worm::setTouchingFloor(bool value)
{
	this->isTouchingFloor = value;
}

bool Worm::setSpeed(speed_t newSpd)
{
	this->spd = newSpd;
	return true;
}

const Point_t* Worm::getCurrentPosition(void)
{
    return this->position.getPosition();
}
void Worm::getCurrentPosition(coord_t& x, coord_t& y)
{
    this->position.getPosition(x, y);
    return;
}

WormPointing Worm::getPointingDirection(void)
{
    return this->pointingDirection;
}

void Worm::getPointingDirection(WormPointing& direction)
{
    direction = this->pointingDirection;
    return;
}

int Worm::getFrame(void)
{
	return this->frame;
}

bool Worm::getTouchingFloor(void)
{
	return this->isTouchingFloor;
}

speed_t Worm::getSpeed(void)
{
	return this->spd;
}

void Worm::update() 
{

	switch (this->state)
	{
	case WARM_MOVE:
		
		if (this->frameCounter == CONFIRMATION_FRAMES + 1)
		{
			this->frame = WF3;
		}
		else if (this->frameCounter <= CONFIRMATION_FRAMES + WALK_WARM_UP_FRAMES - 1 && frameCounter > CONFIRMATION_FRAMES + 1)
		{
			this->frame--;
		}
		else if (this->frameCounter == CONFIRMATION_FRAMES + WALK_WARM_UP_FRAMES)
		{
			this->frame--;
			this->state = MOVING;
			this->spd.x = LATERAL_MOVE_SPEED / (double)(FPS - (CONFIRMATION_FRAMES + WALK_WARM_UP_FRAMES));
			this->spd.x *= (this->pointingDirection == LEFT) ? -1 : 1;
		}
		this->frameCounter++;
		break;
	case MOVING:
		if (this->frameCounter == CONFIRMATION_FRAMES + WALK_WARM_UP_FRAMES + 1)
		{
			this->frame = WF1;
		}
		else if (this->frameCounter <= FPS)
		{
			this->frame++;
		}
		else
		{
			this->state = IDLE;
			this->spd.x = 0;
		}

		if (this->frame > WF14)
		{
			this->frame = WF1;
		}
		break;

	case WARM_JUMP:
		
		if (this->frameCounter == CONFIRMATION_FRAMES + 1)
		{
			this->frame = JF1;
		}
		else if (this->frameCounter <= CONFIRMATION_FRAMES + JUMP_WARM_UP_FRAMES - 1 && frameCounter > CONFIRMATION_FRAMES + 1)
		{
			this->frame++;
		}
		else if (this->frameCounter == CONFIRMATION_FRAMES + JUMP_WARM_UP_FRAMES)
		{
			this->frame++;
			this->state = JUMPING;
			this->isTouchingFloor = false;
			this->spd.y = JUMP_INIT_SPEED * SIN60;
			this->spd.x =JUMP_INIT_SPEED * COS60 * ((this->pointingDirection == LEFT) ? -1 : 1);
		}
		this->frameCounter++;
		break;

	case JUMPING:

		if (this->frameCounter == CONFIRMATION_FRAMES + JUMP_WARM_UP_FRAMES + 1)
		{
			this->frame = JF5;
		}
		else if (this->frame == JF10)
		{
			this->frame = JF1;
			this->state = IDLE;
		}
		else if (this->isTouchingFloor)
		{
			this->frame++;
			this->spd = { 0 };
		}
		break;
	default:
		break;
	}
}
