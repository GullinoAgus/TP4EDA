#ifndef _WORLD_H_
#define _WORLD_H_

typedef enum AnimeFrameType {
	WF1 = 0,
	WF2,
	WF3,
	WF4,
	WF5,
	WF6,
	WF7,
	WF8,
	WF9,
	WF10,
	WF11,
	WF12,
	WF13,
	WF14,
	WF15,
	JF1,
	JF2,
	JF3,
	JF4,
	JF5,
	JF6,
	JF7,
	JF8,
	JF9,
	JF10,
};

typedef enum WormStatesType {
	IDLE,
	MOVING_LEFT,
	MOVING_LEFT_WUP,
	MOVING_RIGHT,
	MOVING_RIGHT_WUP,
	JUMPING_RIGHT,
	JUMPING_RIGHT_WUP,
	JUMPING_LEFT,
	JUMPING_LEFT_WUP,
};

class World
{
public:

	

	void update();

private:
	Worm *wormList;
};


#endif // !_WORLD_H_



