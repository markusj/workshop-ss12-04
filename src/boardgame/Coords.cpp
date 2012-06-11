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

    Coords::Coords(const uint8_t x, const uint8_t y) :
        x(x),
        y(y) {
    }

    Coords::~Coords() {
    }

    uint8_t Coords::getX() const {
        return x;
    }

    Coords Coords::setX(const uint8_t x) const {
        // Bounds are checked by factory
        return Coords(x, y);
    }

    uint8_t Coords::getY() const {
        return y;
    }

    Coords Coords::setY(const uint8_t y) const {
        // Bounds are checked by factory
        return Coords(x, y);
    }
}
