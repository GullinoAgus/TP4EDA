#define _USE_MATH_DEFINES   /* For M_PI */

#include <iostream>
#include <cmath> /* M_PI, sin(), cos(), isgreater, isless */

#include "Coordinates.h"

/******************** DEFINITIONS ********************/

/******************** MACROS ********************/

/******************** CONSTRUCTOR ********************/
Coordinates::Coordinates() {
    this->maximumCoordinates = { 0 };
    this->position = { 0 };
 }

Coordinates::Coordinates(coord_t x, coord_t y, coord_t maxX, coord_t maxY) {  //verifico que las coord max sean validas y las setteo
    this->maximumCoordinates = { 0 };
    this->position = { 0 };

    if (islessequal(maxX, 0.0) || islessequal(maxY, 0.0)) {
        std::cout << "Both maxX and maxY must be greater than 0.0" << std::endl;
        return;
    }
    else if (islessequal(maxX, 0.0) || islessequal(maxY, 0.0)) {
        std::cout << "Both X and Y must be greater than 0.0" << std::endl;
        return;
    }
    
    this->maximumCoordinates.x = maxX;
    this->maximumCoordinates.y = maxY;

    this->position.x = x;
    this->position.y = y;
    
    return;
}

Coordinates::~Coordinates(void) {
    this->position.x = 0;
    this->position.y = 0;
    this->maximumCoordinates.x = 0;
    this->maximumCoordinates.y = 0;
}

/******************** PUBLIC METHODS ********************/
void Coordinates::displace(coord_t x, coord_t y) {
    this->position.x += x;
    this->position.y += y;
    if (isless(this->position.x, 0.0)) { //si el worm quiere salirse del area valida, hace que se quede en la pos x limite
        this->position.x = 0;
    }
    else if (isgreater(this->position.x, maximumCoordinates.x)) {
        this->position.x = this->maximumCoordinates.x;
    }
    if (isless(this->position.y, 0.0)) {  // si el worm quiere salirse del area valida, hace que se quede en la pos y limite
        this->position.y = 0;
    }
    else if (isgreater(this->position.y, maximumCoordinates.y)) {
        this->position.y = this->maximumCoordinates.y;
    }
}

void Coordinates::getPosition(coord_t& x, coord_t& y) { //obtengo pos
    x = this->position.x;
    y = this->position.y;
    return;
}

const Point_t* Coordinates::getPosition(void) {
    return &(this->position);
}

bool Coordinates::setMaximumCoordinates(coord_t x, coord_t y) {
    if (islessequal(x, 0.0) || islessequal(y, 0.0)) {
        std::cout << "Both maxX and maxY must be greater than 0.0" << std::endl;
        return false;
    }

    this->maximumCoordinates.x = x;
    this->maximumCoordinates.y = y;

    return true;
}


bool Coordinates::setPosition(coord_t x, coord_t y) //evita que el worm se salga del area valida de juego
{
    if (isless(x, 0.0))
    {
        this->position.x = 0.0;
    }
    else if (isgreater(x, this->maximumCoordinates.x))
    {
        this->position.x = this->maximumCoordinates.x;
    }
    else
    {
        this->position.x = x;
    }

   
    if (isgreater(y, this->maximumCoordinates.y))
    {
        this->position.y = this->maximumCoordinates.y;
    }
    else
    {
        this->position.y = y;
    }


    return true;
}

/******************** PRIVATE METHODS ********************/
