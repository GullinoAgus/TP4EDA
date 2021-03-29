#ifndef WORM_H
#define WORM_H  1

#include "Coordinates.h"

enum class WormPointing {
    NOT_INIT = -1,
    LEFT = 0,
    RIGHT
};


enum class WormStatesType {
    IDLE = 0,
    MOVING_LEFT,
    MOVING_LEFT_WUP,
    MOVING_RIGHT,
    MOVING_RIGHT_WUP,
    JUMPING_RIGHT,
    JUMPING_RIGHT_WUP,
    JUMPING_LEFT,
    JUMPING_LEFT_WUP,
};

class Worm{
public:
    Worm(void);
    Worm(Point_t initPosition, Point_t maxPosition, WormPointing direction, double gravity);
    ~Worm(void);

    bool move(WormPointing direction);
    bool jump(void);

    bool setMaximumPosition(Point_t pos); /* Set before intial position */
    bool setInitialPosition(Point_t pos);
    bool setInitialPointingDirection(WormPointing dir);
    bool setGravity(double gravity);

    const Point_t* getCurrentPosition(void);
    void getCurrentPosition(coord_t& x, coord_t& y);
    WormPointing getPointingDirection(void);
    void getPointingDirection(WormPointing& direction);

private:
    Coordinates position;

    WormStatesType displacementDirection;
    WormPointing pointingDirection;

    double worldGravity;
    double jumpDistance; /* X axis */
    Point_t jumpInitialPosition;
    unsigned jumpTime; 
};

#endif /* ! WORM_H */