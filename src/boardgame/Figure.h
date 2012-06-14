/*
 * Figure.h
 *
 *  Created on: 11.06.2012
 *      Author: markus
 */

#ifndef FIGURE_H_
#define FIGURE_H_

#include "Coords.h"
#include "Board.h"
#include "BoardTransaction.h"
#include "Player.h"

namespace boardgame {

    // Forward declarations
    class Board;
    class MutableBoardTransaction;

    class Figure {
            friend class Board;

        public:
            virtual ~Figure() { };

            virtual bool canMoveTo(const Coords to) const = 0;
            virtual bool threatens(const Coords pos) const = 0;

            virtual bool prepareMove(MutableBoardTransaction &move) const = 0;

            virtual Player* getPlayer() const = 0;

        protected:
            virtual void setPlayer(Player &p) = 0;

            virtual Coords getPosition() const = 0;
            virtual void setPosition(const Coords& c) = 0;

            virtual Board* getBoard() const = 0;
            virtual void setBoard(Board &b) = 0;
    };

    class AbstractFigure : public Figure {
        public:
            AbstractFigure(Player * const b);
            virtual ~AbstractFigure() { };

            virtual Player* getPlayer() const;

        protected:
            virtual void setPlayer(Player &p);

            virtual Coords getPosition() const;
            virtual void setPosition(const Coords& c);

            virtual Board* getBoard() const;
            virtual void setBoard(Board &b);

        private:
            Player *player;
            Board *board;
            Coords position;
    };

}
#endif /* FIGURE_H_ */
