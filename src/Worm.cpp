#include <iostream>
#include <cmath>    /* fabs */
#include <limits>   /* numeric_limits */

#include "Worm.h"

#define JUMP_INIT_SPEED (4.5)

#define COS60           (0.5)                   /* cos(60°) == cos(pi/3) */
#define SIN60           (0.866025403784438)     /* sin(60°) == sin(pi/3) */

#define JUMP_X_COORD    (JUMP_INIT_SPEED * COS60 * this->jumpTime)
#define JUMP_Y_COORD    (JUMP_INIT_SPEED * SIN60 * this->jumpTime \
                        - this->worldGravity * this->jumpTime * this->jumpTime  / 2)

bool compareDouble(double a, double b) 
{
    return fabs(a - b) < std::numeric_limits<double>::epsilon();
}

Worm::Worm(void) : position() 
{
    this->displacementDirection = WormStatesType::IDLE;
    this->pointingDirection = WormPointing::NOT_INIT;
    this->worldGravity = 0;
    this->jumpInitialPosition = { 0 };
    this->jumpDistance = 0;
    this->jumpTime = 0;
}

Worm::Worm(Point_t initPosition, Point_t maxPosition, 
    WormPointing direction, double gravity) 
    : position(initPosition.x, initPosition.y, maxPosition.x, maxPosition.y) 
{
    this->displacementDirection = WormStatesType::IDLE;
    this->pointingDirection = direction;

    this->jumpInitialPosition = *(position.getPosition());
    this->jumpTime = 0;

    this->setGravity(gravity);
}

Worm::~Worm() 
{
    displacementDirection = WormStatesType::IDLE;
    pointingDirection = WormPointing::NOT_INIT;
    worldGravity = 0;
    jumpDistance = 0;
    jumpInitialPosition = { 0 };
    jumpTime = 0;
}

bool Worm::setMaximumPosition(Point_t pos)
{
    return position.setMaximumCoordinates(pos.x, pos.y);
}

bool Worm::setInitialPosition(Point_t pos)
{
    return position.setPosition(pos.x, pos.y);
}

bool Worm::setInitialPointingDirection(WormPointing dir)
{
    if (dir == WormPointing::NOT_INIT)
    {
        return false;
    }

    pointingDirection = dir;

    return true;
}

bool Worm::setGravity(double gravity) 
{
    if (islessequal(gravity, 0.0)) 
    {
        return false;
    }

    this->worldGravity = gravity;
    this->jumpDistance = 2 * JUMP_INIT_SPEED * SIN60 / gravity;

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

bool Worm::move(WormPointing direction) 
{
    /* Either not initialized or a jump is in progress */
    // TODO: Remove jump condition?
    if (direction == WormPointing::NOT_INIT
        || !compareDouble(jumpInitialPosition.x, (position.getPosition()->x)))
    {
        return false;
    }

    if (direction == WormPointing::LEFT) 
    {
        position.displace(-1, 0);
        this->pointingDirection = WormPointing::LEFT;
    }
    else if (direction == WormPointing::RIGHT) 
    {
        position.displace(1, 0);
        this->pointingDirection = WormPointing::RIGHT;
    }
    this->jumpInitialPosition = *(position.getPosition());

    return true;
}

bool Worm::jump(void) 
{
    this->jumpTime++;

    if (this->pointingDirection == WormPointing::LEFT)
    {
        this->position.setPosition(this->jumpInitialPosition.x - JUMP_X_COORD,
            this->jumpInitialPosition.y + JUMP_Y_COORD);
    }
    else
    {
        this->position.setPosition(this->jumpInitialPosition.x + JUMP_X_COORD,
            this->jumpInitialPosition.y + JUMP_Y_COORD);
    }
        
    /* Jump's over */
    if (compareDouble(this->jumpInitialPosition.x, 
        this->jumpDistance + position.getPosition()->x))
    {
        this->jumpInitialPosition = *(this->position.getPosition());
        this->jumpTime = 0;
    }

    return true;
}

