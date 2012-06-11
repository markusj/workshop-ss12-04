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

    class Coords {
        public:
            Coords(const uint8_t x, const uint8_t y);
            ~Coords();

            uint8_t getX() const;
            Coords setX(const uint8_t x) const;

            uint8_t getY() const;
            Coords setY(const uint8_t y) const;

        private:
            /* const */ uint8_t x;
            /* const */ uint8_t y;
    };

}

#endif /* COORDS_H_ */
