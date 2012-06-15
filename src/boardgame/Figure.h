/*
 * Figure.h
 *
 *  Created on: 11.06.2012
 *      Author: markus
 */



#ifndef FIGURE_H_
#define FIGURE_H_

#include "boardgame.h"
#include "Coords.h"
#include "Player.h"

namespace boardgame {

    class Figure {
            friend class Board;

        public:
            virtual ~Figure() { };

            virtual Figure* clone() const = 0;;

            virtual bool canMoveTo(const Coords &to) const = 0;
            virtual bool threatens(const Coords &pos) const = 0;

            virtual bool apply(MutableBoardTransaction &move) const = 0;

            virtual Player* getPlayer() const = 0;

        protected:
            virtual void setPlayer(Player &p) = 0;

            virtual const Coords& getPosition() const = 0;
            virtual void setPosition(const Coords& c) = 0;

            virtual Board* getBoard() const = 0;
            virtual void setBoard(Board &b) = 0;
    };

    class AbstractFigure : public Figure {
        public:
            AbstractFigure(Player * const p);
            virtual ~AbstractFigure() { };

            virtual Player* getPlayer() const;

        protected:
            virtual void setPlayer(Player &p);

            virtual const Coords& getPosition() const;
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
