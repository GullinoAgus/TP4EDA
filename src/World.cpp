#include "World.h"
#include <iostream>

void World::update() {
	
	static int frameCounter = 0;

	while (wormList != NULL) {
		
		switch (worm.state)
		{
		case MOVING_LEFT:

		default:
			break;
		}
	}

}

#define LATERAL_MOVE_SPEED 27.0
#define FPS 50
#define CONFIRMATION_FRAMES 5
#define WARM_UP_FRAMES 3

void Worm::update() 
{
	static int frameCounter = 0;

	switch (this->state)
	{
	case MOVING_LEFT_WUP:case MOVING_RIGHT_WUP:
		
		if (frameCounter == CONFIRMATION_FRAMES + 1)
		{
			this->frame = WF3;
		}
		else if (frameCounter <= CONFIRMATION_FRAMES + WARM_UP_FRAMES - 1 && frameCounter > CONFIRMATION_FRAMES + 1)
		{
			this->frame--;
		}
		else
		{
			this->frame--;
			this->state = this->state == MOVING_LEFT_WUP ? MOVING_LEFT : MOVING_RIGHT;
		}
		frameCounter++;
		break;
	case MOVING_LEFT:case MOVING_RIGHT:
		if (frameCounter == CONFIRMATION_FRAMES + WARM_UP_FRAMES + 1)
		{
			this->velX = LATERAL_MOVE_SPEED / (double)(FPS - (CONFIRMATION_FRAMES + WARM_UP_FRAMES));
			this->velX *= this->state == MOVING_LEFT ? -1 : 1;
			this->frame = WF1;
		}
		else if (frameCounter <= FPS)
		{
			this->frame++;
		}
		else
		{
			this->state = IDLE;
			this->velX = 0;
		}
		if (this->frame > WF14)
		{
			this->frame = WF1;
		}
		break;

	case JUMPING_LEFT_WUP: case JUMPING_RIGHT_WUP:
		/*if (frameCounter <= CONFIRMATION_FRAMES)      //TODO: Como carajos es el warmup de salto
		{
			frameCounter++;
		}
		else if (frameCounter <= CONFIRMATION_FRAMES + WARM_UP_FRAMES - 1)
		{
			this->frame++;
			frameCounter++;
		}
		else
		{
			this->frame++;
			frameCounter++;
			this->state = this->state == JUMPING_LEFT_WUP ? JUMPING_LEFT : JUMPING_RIGHT;
		}*/
		if (frameCounter < )
		{

		}
		frameCounter++;
		break;

	case JUMPING_LEFT: case JUMPING_RIGHT:

		break;
	default:
		break;
	}
}