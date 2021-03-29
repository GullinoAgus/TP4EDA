#ifndef _WORLD_H_
#define _WORLD_H_
#include "Worms.h"

#define HORIZONTAL 616
#define MAX_WORMS 2

class World
{
public:

	

	void update();

private:
	Worm wormArr[MAX_WORMS];
};


#endif // !_WORLD_H_



