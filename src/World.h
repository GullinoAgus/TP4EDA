#ifndef _WORLD_H_
#define _WORLD_H_
#include "Worm.h"
#include "config.h"
//#define HORIZONTAL 616
//#define MAX_X_POS 1212
//#define MIN_X_POS 701


enum class WormsByName
{
	Isaac,
	Ray
};

enum class WormActions 
{
	JUMP,
	WALK
};

class World
{
public:
	// Corners of the rectangle with the playable area.
	World(Point_t worldSize);
	void update(void);

	Worm* getWormArr(void);

	void warmUpWorm(WormsByName name, WormActions action, WormPointing direction);
	bool forceWormStop(WormsByName name);

private:
	Worm* getWorm(WormsByName name);
	Worm wormArr[MAX_WORMS];
};


#endif // !_WORLD_H_



