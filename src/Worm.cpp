#include <iostream>
#include <cmath>    /* fabs */
#include <limits>   /* numeric_limits */

#include "Worm.h"
#include "config.h"


#define JUMP_INIT_SPEED (4.5)

#define COS60           (0.5)                   /* cos(60?) == cos(pi/3) */
#define SIN60           (0.866025403784438)     /* sin(60?) == sin(pi/3) */

#define LATERAL_MOVE_SPEED 27.0

#define CONFIRMATION_FRAMES 5  //frames segun consigna
#define WALK_WARM_UP_FRAMES 3
#define JUMP_WARM_UP_FRAMES 4

#define JUMP_X_COORD    (JUMP_INIT_SPEED * COS60 * this->jumpTime) //calculo de las coord segun vel y tiempo
#define JUMP_Y_COORD    (JUMP_INIT_SPEED * SIN60 * this->jumpTime \
                        - this->worldGravity * this->jumpTime * this->jumpTime  / 2)

bool compareDouble(double a, double b) 
{
    return fabs(a - b) < std::numeric_limits<double>::epsilon();
}

Worm::Worm(void) : position() 
{
	this->isTouchingFloor = false;
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

	this->setPointingDirection(direction);
	this->frame = WF1;
}

Worm::~Worm() 
{
    this->pointingDirection = NOT_INIT;
	this->frameCounter = 0;
	this->state = IDLE;
	this->spd = { 0 };
}

bool Worm::setMaximumPosition(Point_t pos)  //devuelve la pos max
{
    return position.setMaximumCoordinates(pos.x, pos.y);
}

bool Worm::setPosition(Point_t pos) //devuelve pos
{
    return position.setPosition(pos.x, pos.y);
}

bool Worm::setPointingDirection(WormPointing dir) //devulve direc 
{
	if (dir == LEFT || dir == RIGHT) 
	{
		this->pointingDirection = dir;
	}
	else
	{
		return false;
	}

    return true;
}

void Worm::setTouchingFloor(bool value)  //me fijo si esta tocando el piso
{
	this->isTouchingFloor = value;
}

bool Worm::setSpeed(speed_t newSpd)  //defino la vel del worm
{
	this->spd = newSpd;
	return true;
}

const Point_t* Worm::getCurrentPosition(void)  //devuelve pos actual
{
    return this->position.getPosition();
}
void Worm::getCurrentPosition(coord_t& x, coord_t& y)
{
    this->position.getPosition(x, y);
    return;
}

WormPointing Worm::getPointingDirection(void)  //devuelve direc a la que apunta actualmente
{
    return this->pointingDirection;
}

void Worm::getPointingDirection(WormPointing& direction)
{
    direction = this->pointingDirection;
    return;
}

int Worm::getFrame(void)  //devuekve en que frame esta para graficar
{
	return this->frame;
}

bool Worm::getTouchingFloor(void)  //devuelve si esta tocando o no el piso actualmente
{
	return this->isTouchingFloor;
}

speed_t Worm::getSpeed(void)  //devuvlve la vel actual
{
	return this->spd;
}

WormStatesType Worm::getState(void)  //devuelve estado actual
{
	return this->state;
}

bool Worm::setState(WormStatesType newState) 
{
	if (newState == IDLE || newState == WARM_JUMP || newState == WARM_MOVE)
	{
		this->state = newState;
		if (newState == IDLE)
		{
			this->update();
		}
		return true;
	}

	return false;
}

void Worm::update() //se actualizan los frames y velocidades
{
	switch (this->state)
	{
		case IDLE:
			this->frameCounter = 0;
			this->frame = WF1;
			this->spd.x = 0;
			this->spd.y = 0;
			break;

		case WARM_MOVE:
			if ( frameCounter > CONFIRMATION_FRAMES && this->frameCounter < CONFIRMATION_FRAMES + WALK_WARM_UP_FRAMES)
			{
				this->frame++;
			}
			else if (this->frameCounter == CONFIRMATION_FRAMES + WALK_WARM_UP_FRAMES)
			{
				this->frame++;
				this->state = MOVING;
			}
			this->frameCounter++;
			break;

		case MOVING:
			if (this->frameCounter <= FPS)
			{
				if ((this->frameCounter - 9) % 14 == 0 || (this->frameCounter - 16) % 14 == 0 )
				{
					this->frame--;
				}

				this->frame++;
			}

			if (this->frame > WF15)
			{
				if (this->frameCounter >= FPS)
				{
					this->frame = WF1;
				}
				else
				{
					this->frame = WF4;
				}
				this->spd.x = 0;
			}
			else if (this->frame == WF15)
			{
				this->spd.x = 9;
				this->spd.x *= (this->pointingDirection == LEFT) ? -1 : 1;
			}

			if (this->frameCounter == FPS)
			{
				this->state = IDLE;
			}


			this->frameCounter++;
#ifdef DEBUG
			std::cout << this->frameCounter << std::endl;
#endif // DEBUG
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
				this->spd.y = -JUMP_INIT_SPEED * SIN60;
				this->spd.x = JUMP_INIT_SPEED * COS60 * ((this->pointingDirection == LEFT) ? -1 : 1);
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
			this->frameCounter++;
			break;
		default:
			break;
		}
}
