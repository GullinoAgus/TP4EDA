#ifndef _WORLD_H_
#define _WORLD_H_
#include "Worm.h"
#include "config.h"
//#define HORIZONTAL 616
//#define MAX_X_POS 1212 //defino los desplazamientos validos para los worms
//#define MIN_X_POS 701


enum class WormsByName //nombres de los worms
{
	Isaac,
	Ray
};

enum class WormActions //acciones de los worms
{
	JUMP,
	WALK
};

class World
{
public:
	// Corners of the rectangle with the playable area.
	World(Point_t worldSize);
	void update(WormsByName worm);

	Worm* getWormArr(void);

	void warmUpWorm(WormsByName name, WormActions action, WormPointing direction);
	bool forceWormStop(WormsByName name);

private:
	Worm* getWorm(WormsByName name);
	Worm wormArr[MAX_WORMS];
};


#endif // !_WORLD_H_



