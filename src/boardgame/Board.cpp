/*
 * Board.cpp
 *
 *  Created on: 11.06.2012
 *      Author: markus
 */

#include <cstddef>
#include <stdexcept>
#include <vector>
#include "Board.h"
#include "BoardgameStatusCodes.h"

namespace boardgame {

    Board::Board(const Coords &dimensions) :
        dim(dimensions),
        board(new Figure*[dim.getX() * dim.getY()]){
    }

    Board::~Board() {
        for (size_t i = 0; i < dim.getX() * dim.getY(); i++) {
            delete board[i];
        }

        delete[] board;
    }

    Board::Board(const Board &b) :
        dim(b.dim),
        board(new Figure*[dim.getX() * dim.getY()]) {

        for (size_t i = 0; i < dim.getX() * dim.getY(); i++) {
            board[i] = b.board[i]->clone();
        }
    }

    Board& Board::operator=(const Board &b) {
        if (&b != this) {
            this->~Board();
            new(this) Board(b);
        }

        return *this;
    }

    const Coords& Board::getDimensions() {
        return dim;
    }

    const Figure* Board::get(const Coords &pos) const {
        return board[coordsToIndex(pos)];
    }



    bool Board::apply(BoardTransaction &tr) {
        MutableBoardTransaction mtr(tr);

        if (mtr.isAccepted()) {
            return false; // transaction has already been accepted/applied
        }

        mtr.setTransactionNumber(nextTransaction); // assign preliminary transaction number

        if (mtr.getBoardOps().size() == 0) { // empty transaction, apply rules
            if (!inRange(mtr.getFrom()) || !inRange(mtr.getTo())) {
                throw std::out_of_range("Source or target coordinates are outside of board range");
            }

            if (mtr.getFrom() == mtr.getTo()) {
                mtr.setStateCode(bsc_SourceAndDestinationAreEqual);

                return false;
            }

            Figure *fig = board[coordsToIndex(mtr.getFrom())];
            if (fig == NULL) {
                return false; // no figure which could do something is there
            }

            if (!fig->apply(mtr)) {
                mtr.setAccepted(false);
                mtr.getBoardOps().clear();

                return false; // failed to prepare transaction
            }
        }

        std::vector<BoardOp::Operation*> &ops = mtr.getBoardOps();

        bool result = true;
        size_t i = 0;

        for (; result && i < ops.size(); i++) {
            result &= ops[i]->apply(*this);
        }

        if (result) {
            mtr.setAccepted(true);
            nextTransaction++; // Increment for next transaction

            return true; // transaction has been applied successfully
        } // else: transaction failed, rollback

        result = true;

        do {
            i--;
            result &= ops[i]->undo(*this);
        } while (result && i != 0);

        if (!result) { // rollback has failed, too ... something is broken utterly
            // we don't bother about invalidating the transaction, this is a major
            // problem which can not be resolved
            throw std::logic_error("Undo operation failed to recover from failure");
        }

        mtr.setAccepted(false);

        return false;
    }

    bool Board::undo(BoardTransaction &tr) {
        MutableBoardTransaction mtr(tr);

        if (!mtr.isAccepted()) {
            return false; // transaction was not accepted or already undone
        }

        if (mtr.getTransactionNumber() != nextTransaction - 1) {
            return false; // not the least recently issued transaction
        }

        std::vector<BoardOp::Operation*> &ops = mtr.getBoardOps();
        bool result = true;
        size_t i = ops.size();

        if (i == 0) {
            mtr.setAccepted(false); // undo done, invalidate transaction
            nextTransaction = mtr.getTransactionNumber();

            return true; // empty transaction? strange, but undo is done ...
        }

        do  {
            i--; // undo -> replay backwards
            result &= ops[i]->undo(*this);
        } while (result && i != 0);

        if (result) {
            mtr.setAccepted(false); // undo done, invalidate transaction
            ops.clear(); // especially: clear all ops
            nextTransaction = mtr.getTransactionNumber();

            return true; // all went well, leave
        } // else: something has gone wrong, undoing undo to restore previous state

        i++; // last operation failed, increment by one to reach the first undone operation
        result = true;

        for (; result && i < ops.size(); i++) {
            result &= ops[i]->apply(*this);
        }

        if (!result) { // undo rollback has failed, too ... something is broken utterly
            // we don't bother about invalidating the transaction, this is a major
            // problem which can not be resolved
            throw std::logic_error("Undo operation failed to recover from failure");
        }

        return false; // rollback went well, don't invalidate transaction
    }

    bool Board::move(const Coords &from, const Coords &to) {
        if (board[coordsToIndex(from)] == NULL || board[coordsToIndex(to)] == NULL) {
            return false;
        }
        // TODO: Figure parametrisieren
        board[coordsToIndex(to)] = board[coordsToIndex(from)];
        board[coordsToIndex(from)] = NULL;

        return true;
    }

    Figure* Board::swap(const Coords &pos, const Figure * const fig) {
        Figure * const result = board[coordsToIndex(pos)];
        // TODO: Figure parametrisieren
        board[coordsToIndex(pos)] = (fig == NULL) ? NULL : fig->clone();

        return result;
    }

    bool Board::inRange(const Coords &c) const {
        return c.getX() < dim.getX() && c.getY() < dim.getY();
    }

    size_t Board::coordsToIndex(const Coords &c) const {
        if (!inRange(c)) {
            throw std::out_of_range("Passed coords are outside of board range.");
        }

        return dim.getX() * c.getX() + c.getY();
    }

    BoardOp::Operation::Operation() :
        applied(false) {
    }

    bool BoardOp::Operation::apply(Board &b) {
        if (applied) {
            return false;
        }

        applied = _applyImpl(b);

        return applied;
    }

    bool BoardOp::Operation::undo(Board &b) {
        if (!applied) {
            return false;
        }

        applied = !_applyImpl(b);

        return !applied;
    }

    bool BoardOp::Operation::isApplied() {
        return applied;
    }

    BoardOp::Move::Move(const Coords &from, const Coords &to) :
        BoardOp::Operation(),
        from(from),
        to(to) {
    }

    BoardOp::Operation* BoardOp::Move::clone() {
        return new Move(*this);
    }

    bool BoardOp::Move:: _applyImpl(Board &b) {
        return b.move(from, to);
    }

    bool BoardOp::Move::_undoImpl(Board &b) {
        return b.move(to, from);
    }

    BoardOp::Replace::Replace(const Coords &pos, Figure *fig) :
        BoardOp::Operation(),
        position(pos),
        newFigure((NULL == fig) ? (NULL) : (fig->clone())),
        oldFigure(NULL) {
    }

    BoardOp::Replace::Replace(const BoardOp::Replace &r) :
        BoardOp::Operation(),
        position(r.position),
        newFigure((r.newFigure == NULL) ? NULL : r.newFigure->clone()),
        oldFigure((r.oldFigure == NULL) ? NULL : r.oldFigure->clone()) {
    }

    BoardOp::Operation* BoardOp::Replace::clone() {
        return new Replace(*this);
    }

    BoardOp::Replace::~Replace() {
        // Safe since empty field are marked by NULL and delete NULL is a no-op
        // newFigure is always owned by this instance, the ownership of oldFigure
        // has been taken over after executing
        delete newFigure;
        delete oldFigure;
    }

    bool BoardOp::Replace::_applyImpl(Board &b) {
        oldFigure = b.swap(position, newFigure);

        return true;
    }

    bool BoardOp::Replace:: _undoImpl(Board &b) {
        delete b.swap(position, oldFigure);
        delete oldFigure;

        oldFigure = NULL;

        return true;
    }

}
