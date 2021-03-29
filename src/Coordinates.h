#ifndef COORDINATES_H
#define COORDINATES_H  1

typedef double coord_t;

typedef struct Point {
    coord_t x;
    coord_t y;
} Point_t;

class Coordinates {
public:
    /* X and Y are initialized to 0.0 */
    Coordinates(void);
    /* X, Y are set with the given value
    * Maximum coordinates for X and Y are also stored.
    */
    Coordinates(coord_t x, coord_t y, coord_t maxX, coord_t maxY);

    ~Coordinates(void);

    /*
    * How much to displace in X and Y axis.
    * Does nothing when out of bounds
    */
    void displace(coord_t x, coord_t y);

    void getPosition(coord_t& x, coord_t& y);
    const Point_t* getPosition(void);

    bool setMaximumCoordinates(coord_t x, coord_t y);
    bool setPosition(coord_t x, coord_t y);

private:
    Point_t position;
    Point_t maximumCoordinates;
};

#endif /* !COORDINATES_H */
