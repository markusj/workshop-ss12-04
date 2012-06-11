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

    /*
     * Coords
     */

    Coords::Coords(CoordFactory &factory, const uint8_t x, uint8_t y) :
        factory(factory),
        x(x),
        y(y) {
    }

    Coords::~Coords() {
    }

    bool Coords::inValidRange(const uint8_t x, const uint8_t y) const {
        return factory.inValidRange(x, y);
    }

    Coords& Coords::valueOf(const uint8_t x, const uint8_t y) const {
        // Bounds are checked by factory
        return factory.getCoords(x, y);
    }

    uint8_t Coords::getX() const {
        return x;
    }

    uint8_t Coords::getY() const {
        return y;
    }

    CoordFactory& Coords::getFactory() const {
        return factory;
    }

    Coords& Coords::setX(const uint8_t x) const {
        // Bounds are checked by factory
        return valueOf(x, y);
    }

    Coords& Coords::setY(const uint8_t y) const {
        // Bounds are checked by factory
        return valueOf(x, y);
    }

    /*
     * CoordFactory
     */

    static uint16_t coordsToIndex(const uint8_t dimX, const uint8_t x, const uint8_t y) {
        return dimX * x + y;
    }

    CoordFactory::CoordFactory(const uint8_t dimX, uint8_t dimY) :
        dimX(dimX),
        dimY(dimY),
        coords(initCoords()),
        coordInstances(new size_t) {
        *coordInstances = 1;
    }

    Coords* CoordFactory::initCoords() {
        if (dimX == 0 || dimY == 0) {
            throw new std::invalid_argument("At least one dimension is set to zero.");
        }

        void * const raw = operator new[](dimX * dimY * sizeof(Coords));
        Coords *result = (Coords *) raw;

        for (uint16_t i = 0; i < dimX * dimY; i++) {
            new(&result[i]) Coords(*this, i / dimX, i % dimX);
        }

        return result;
    }

    CoordFactory::CoordFactory(const CoordFactory &cf) :
        dimX(cf.dimX),
        dimY(cf.dimY),
        coords(cf.coords),
        coordInstances(cf.coordInstances) {
        *coordInstances += 1; // Keep invariants valid
    }

    CoordFactory::~CoordFactory() {
        *coordInstances -= 1;

        if (*coordInstances == 0) { // This was the last instance using "coords"
            // cleanup
            uint16_t i = dimX * dimY;

            do {
                coords[--i].~Coords();
            } while (i > 0);

            operator delete[] ((void *) coords); // delete coord array
            delete coordInstances; // and also instance counter, too!
        }
    }

    CoordFactory& CoordFactory::operator=(const CoordFactory &cf) {
        if (this != &cf) {
            // A special hackaround used to initialize the "const" members
            // of the copy target and keep all invariants valid

            this->~CoordFactory(); // Cleanup old garbage
            new(this) CoordFactory(cf); // Invoke copy constructor
        }

        return *this;
    }

    bool CoordFactory::inValidRange(const uint8_t x, const uint8_t y) const {
        return x < dimX && y < dimY;
    }

    Coords& CoordFactory::getCoords(const uint8_t x, const uint8_t y) const {
        if (x >= dimX || y >= dimY) {
            throw new std::out_of_range("At least one dimension exceedes the valid range.");
        }

        return coords[coordsToIndex(dimX, x, y)];
    }

    uint8_t CoordFactory::getDimX() const {
        return dimX;
    }

    uint8_t CoordFactory::getDimY() const {
        return dimY;
    }

}
