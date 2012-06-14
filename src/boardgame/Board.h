/*
 * Board.h
 *
 *  Created on: 11.06.2012
 *      Author: markus
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "BoardTransaction.h"
#include "Coords.h"
#include "Figure.h"
#include "Player.h"

namespace boardgame {

    // Forward declarations
    class BoardTransaction;
    class Figure;

    class Board {
            friend class Move;
            friend class Swap;

        public:
            Board(Coords dimensions);
            virtual ~Board();

            Figure* atPos(const Coords &pos) const;
            Figure* operator[](const Coords &pos) const;

            bool apply(BoardTransaction &tr);
            bool undo(BoardTransaction &tr);

        protected:
            Figure*& atPos(const Coords &pos);
            Figure*& operator[](const Coords &pos);

        private:
            Coords dim;
            Figure **board;
    };

    namespace BoardOp {

        class Operation {
                friend class Board;

            public:
                Operation();
                virtual ~Operation() { };

            protected:
                bool apply(Board &b);
                bool undo(Board &b);

                virtual bool _applyImpl(Board &b) = 0;
                virtual bool _undoImpl(Board &b) = 0;

            private:
                bool done;
        };

        class Move : public Operation {
                Move(Coords &from, Coords &to);
                virtual ~Move();

            protected:
                virtual bool _applyImpl(Board &b);
                virtual bool _undoImpl(Board &b);

            private:
                Coords from;
                Coords to;
        };

        class Swap : public Operation {
            public:
                Swap(Coords &pos, Figure &fig);
                virtual ~Swap();

            protected:
                virtual bool _applyImpl(Board &b);
                virtual bool _undoImpl(Board &b);

            private:
                Coords position;
                Figure *newFigure;
                Figure *oldFigure;
        };
    }
}

#endif /* BOARD_H_ */
