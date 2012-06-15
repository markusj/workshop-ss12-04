/*
 * Coords.cpp
 *
 *  Created on: 10.06.2012
 *      Author: markus
 */

#include <cstring>
#include <stdexcept>
#include "Coords.h"

namespace boardgame {

    Coords::Coords(const CoordsBase_t x, const CoordsBase_t y) :
        x(x),
        y(y) {
    }

    Coords::~Coords() {
    }

    bool Coords::operator==(const Coords &c) const {
        return x == c.x && y == c.y;
    }

    CoordsBase_t Coords::getX() const {
        return x;
    }

    Coords Coords::setX(const CoordsBase_t x) const {
        // Bounds are checked by factory
        return Coords(x, y);
    }

    CoordsBase_t Coords::getY() const {
        return y;
    }

    Coords Coords::setY(const CoordsBase_t y) const {
        // Bounds are checked by factory
        return Coords(x, y);
    }
}
