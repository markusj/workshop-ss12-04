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

    typedef unsigned char CoordsBase_t;

    class Coords {
        public:
            Coords(const CoordsBase_t x, const CoordsBase_t y);
            ~Coords();

            bool operator==(const Coords &c) const;

            CoordsBase_t getX() const;
            Coords setX(const CoordsBase_t x) const;

            CoordsBase_t getY() const;
            Coords setY(const CoordsBase_t y) const;

        private:
            /* const */ CoordsBase_t x;
            /* const */ CoordsBase_t y;
    };

}

#endif /* COORDS_H_ */
