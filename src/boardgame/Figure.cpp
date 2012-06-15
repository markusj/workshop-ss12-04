/*
 * Figure.cpp
 *
 *  Created on: 11.06.2012
 *      Author: markus
 */

#include "Figure.h"

namespace boardgame {

    AbstractFigure::AbstractFigure(Player * const p) :
        player(p),
        board(NULL),
        position(Coords(0, 0)) {
    }

    Player* AbstractFigure::getPlayer() const {
        return player;
    }

    void AbstractFigure::setPlayer(Player &p) {
        player = &p;
    }

    const Coords& AbstractFigure::getPosition() const {
        return position;
    }

    void AbstractFigure::setPosition(const Coords& c) {
        position = c;
    }

    Board* AbstractFigure::getBoard() const {
        return board;
    }

    void AbstractFigure::setBoard(Board &b) {
        board = &b;
    }

}
