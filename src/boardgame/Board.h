/*
 * Board.h
 *
 *  Created on: 11.06.2012
 *      Author: markus
 */


#ifndef BOARD_H_
#define BOARD_H_

#include <cstddef>
#include "boardgame.h"
#include "Coords.h"
#include "BoardTransaction.h"
#include "Figure.h"

namespace boardgame {

    class Board {
            friend class BoardOp::Move;
            friend class BoardOp::Replace;

        public:
            Board(const Coords &dimensions);
            virtual ~Board();

            Board(const Board &b);
            Board& operator=(const Board &b);

            const Coords& getDimensions();

            const Figure* get(const Coords &pos) const;

            bool apply(BoardTransaction &tr);
            bool undo(BoardTransaction &tr);


            // TODO: Observer
        protected:
            bool move(const Coords &from, const Coords &to);
            Figure* swap(const Coords &pos, const Figure * const fig);

        private:
            bool inRange(const Coords &c) const;
            size_t coordsToIndex(const Coords &c) const;

            Coords dim;
            Figure **board;
            TransactionNumber_t nextTransaction;
    };

    namespace BoardOp {

        class Operation {
                friend class boardgame::Board;

            public:
                Operation();
                virtual ~Operation() { };

            protected:
                bool apply(Board &b);
                bool undo(Board &b);

                bool isApplied();

                virtual bool _applyImpl(Board &b) = 0;
                virtual bool _undoImpl(Board &b) = 0;
            private:
                bool applied;
        };

        class Move : public Operation {
                Move(const Coords &from, const Coords &to);
                virtual ~Move() { };

            protected:
                virtual bool _applyImpl(Board &b);
                virtual bool _undoImpl(Board &b);

            private:
                Coords from;
                Coords to;
        };

        class Replace : public Operation {
            public:
                Replace(const Coords &pos, Figure &fig);
                virtual ~Replace();

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
