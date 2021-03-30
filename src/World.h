#ifndef _WORLD_H_
#define _WORLD_H_
#include "Worm.h"

//#define HORIZONTAL 616
//#define MAX_X_POS 1212
//#define MIN_X_POS 701

#define HORIZONTAL	(WORM_HEIGHT)
#define MAX_WORMS	2
#define GRAVITY		0.24

class World
{
public:
	// Corners of the rectangle with the playable area.
	World(Point_t worldSize);
	void update(void);

	Worm* getWormArr(void);

private:
	Worm wormArr[MAX_WORMS];
};


#endif // !_WORLD_H_



