#ifndef WORM_H
#define WORM_H  1

#include "Coordinates.h"

#define BITMAPHEIGHT 10 //TODO: Ver el alto del bitmap

typedef struct {
    double x;
    double y;
}speed_t;

enum WormPointing {
    NOT_INIT = 0,
    LEFT,
    RIGHT
} ;

enum WormStatesType {
    IDLE = 0,
    WARM_MOVE,
    MOVING,
    WARM_JUMP,
    JUMPING,
};

enum AnimeFrameType {
    INVALID = -1,
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

class Worm{
public:
    Worm(void);
    Worm(Point_t initPosition, Point_t maxPosition, WormPointing direction);
    ~Worm(void);

    void update(void);

    bool setMaximumPosition(Point_t pos); /* Set before intial position */
    bool setPosition(Point_t pos);
    bool setInitialPointingDirection(WormPointing dir);
    void setTouchingFloor(bool value);
    bool setSpeed(speed_t newSpd);

    const Point_t* getCurrentPosition(void);
    void getCurrentPosition(coord_t& x, coord_t& y);
    WormPointing getPointingDirection(void);
    void getPointingDirection(WormPointing& direction);
    int getFrame(void);
    bool getTouchingFloor(void);
    speed_t getSpeed(void);

private:
    Coordinates position;

    WormPointing pointingDirection;

    bool isTouchingFloor;

    int frame;
    int frameCounter;
    WormStatesType state;

    speed_t spd;
};

#endif /* ! WORM_H */