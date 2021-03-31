#ifndef _WORM_H_
#define _WORM_H_  1

#include "Coordinates.h"

#define WORM_HEIGHT (60)
#define WORM_WIDTH  (WORM_HEIGHT-20)    


typedef struct { //estructura para la velocidad
    double x;
    double y;
}speed_t;

enum WormPointing { //me fijo en que direc apunta el worm
    NOT_INIT = 0,
    LEFT,
    RIGHT
} ;

enum WormStatesType { //en que estado esta el worm
    IDLE = 0,
    WARM_MOVE,
    MOVING,
    WARM_JUMP,
    JUMPING,
};

enum AnimeFrameType { //en que frame esta asi puedo graficar facilmente
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
    N_FRAMES
};

class Worm{
public:
    Worm(void);
    Worm(Point_t initPosition, Point_t maxPosition, WormPointing direction);
    ~Worm(void);

    void update(void);

    bool setMaximumPosition(Point_t pos); /* Set before intial position */
    bool setPosition(Point_t pos);
    bool setPointingDirection(WormPointing dir);
    void setTouchingFloor(bool value);
    bool setSpeed(speed_t newSpd);

    bool setState(WormStatesType newState); // Only let's you set WARM_* and IDLE states

    const Point_t* getCurrentPosition(void);
    void getCurrentPosition(coord_t& x, coord_t& y);
    WormPointing getPointingDirection(void);
    void getPointingDirection(WormPointing& direction);

    WormStatesType getState(void);
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

#endif /* ! _WORM_H_ */