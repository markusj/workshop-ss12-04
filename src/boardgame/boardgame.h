/*
 * boardgame.h
 *
 *  Created on: 15.06.2012
 *      Author: markus
 */

#ifndef BOARDGAME_H_
#define BOARDGAME_H_

#include <vector>

// Forward declarations
namespace boardgame {
    typedef unsigned char CoordsBase_t;

    typedef unsigned int StateCode_t;
    typedef unsigned int TransactionNumber_t;

    class Board;
    class BoardTransaction;
    class Coords;
    class MutableBoardTransaction;
    class Figure;
    class Player;

    namespace BoardOp {
        class Operation;
        class Move;
        class Replace;
    }

    typedef std::vector<BoardOp::Operation*> BoardOpVec;
}

#endif /* BOARDGAME_H_ */
