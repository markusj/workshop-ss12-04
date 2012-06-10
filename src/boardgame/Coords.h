/*
 * Coords.h
 *
 *  Created on: 10.06.2012
 *      Author: markus
 */

#ifndef COORDS_H_
#define COORDS_H_

#include <cstddef>

namespace boardgame {

    typedef unsigned char uint8_t; // Evil but i don't care ...
    typedef unsigned short uint16_t; // This is evil, too

    class CoordFactory;

    class Coords {
        public:
            Coords& valueOf(const uint8_t x, const uint8_t y) const;

            uint8_t getX() const;
            uint8_t getY() const;

            CoordFactory& getFactory() const;

            Coords& setX(const uint8_t x) const;
            Coords& setY(const uint8_t y) const;

        private:
            friend class CoordFactory;

            Coords(CoordFactory &factory, const uint8_t x, uint8_t y);
            Coords(Coords &c);
            ~Coords();

            Coords& operator=(const Coords &c);

            CoordFactory& factory;
            const uint8_t x;
            const uint8_t y;
    };

    class CoordFactory {
        public:
            CoordFactory(const uint8_t dimX, const uint8_t dimY);
            CoordFactory(const CoordFactory &cf);
            ~CoordFactory();

            CoordFactory& operator=(const CoordFactory &cf);

            Coords& getCoords(const uint8_t x, const uint8_t y) const;

        private:

            Coords* initCoords();

            const uint8_t dimX;
            const uint8_t dimY;

            Coords * const coords;
            size_t * const coordInstances;
    };

}

#endif /* COORDS_H_ */
