/*
 * BoardTransaction.cpp
 *
 *  Created on: 12.06.2012
 *      Author: markus
 */

#include "BoardTransaction.h"


namespace boardgame {

    /*
     * BasicBoardTransaction
     */

    BasicBoardTransaction::BasicBoardTransaction(const Coords from, const Coords to) :
        from(from),
        to(to),
        accepted(false),
        stateCode(0),
        num(0),
        boardOps(std::vector<BoardOp::Operation*>()) {
    }

    BasicBoardTransaction::BasicBoardTransaction(std::vector<BoardOp::Operation*>& ops) :
        from(Coords(0, 0)),
        to(from),
        accepted(false),
        stateCode(0),
        num(0),
        boardOps(ops) {
    }

    BasicBoardTransaction::~BasicBoardTransaction() {
        for (size_t i = 0; i < boardOps.size(); i++) {
            delete boardOps[i];
        }

        boardOps.clear();
    }

    BasicBoardTransaction::BasicBoardTransaction(const BasicBoardTransaction &src) :
        from(src.from),
        to(src.to),
        accepted(src.accepted),
        stateCode(src.stateCode),
        num(src.num),
        boardOps(std::vector<BoardOp::Operation*>()) {

        for (size_t i = 0; i < boardOps.size(); i++) {
            boardOps.push_back(src.boardOps[i]->clone());
        }
    }

    BasicBoardTransaction& BasicBoardTransaction::operator=(const BasicBoardTransaction &src) {
        if (this != &src) {
            this->~BasicBoardTransaction();

            new(this) BasicBoardTransaction(src);
        }

        return *this;
    }

    const Coords& BasicBoardTransaction::getFrom() const {
        return from;
    }

    const Coords& BasicBoardTransaction::getTo() const {
        return to;
    }

    bool BasicBoardTransaction::isAccepted() const {
        return accepted;
    }

    void BasicBoardTransaction::setAccepted(const bool accepted) {
        this->accepted = accepted;
    }

    StateCode_t BasicBoardTransaction::getStateCode() const {
        return stateCode;
    }

    void BasicBoardTransaction::setStateCode(const StateCode_t code) {
        stateCode = code;
    }

    TransactionNumber_t BasicBoardTransaction::getTransactionNumber() const {
        return num;
    }

    void BasicBoardTransaction::setTransactionNumber(const TransactionNumber_t value) {
        num = value;
    }

    std::vector<BoardOp::Operation*>& BasicBoardTransaction::getBoardOps() {
        return boardOps;
    }

    const std::vector<BoardOp::Operation*>& BasicBoardTransaction::getBoardOps() const {
        return boardOps;
    }

    /*
     * MutableBoardTransaction
     */

    MutableBoardTransaction::MutableBoardTransaction(BoardTransaction &tr) :
        impl(&tr) {
    }

    BoardTransaction& MutableBoardTransaction::getBoardTransaction() const {
        return *impl;
    }

    const Coords& MutableBoardTransaction::getFrom() const {
        return impl->getFrom();
    }

    const Coords& MutableBoardTransaction::getTo() const {
        return impl->getTo();
    }

    bool MutableBoardTransaction::isAccepted() const {
        return impl->isAccepted();
    }

    void MutableBoardTransaction::setAccepted(const bool accepted) {
        impl->setAccepted(accepted);
    }

    StateCode_t MutableBoardTransaction::getStateCode() const {
        return impl->getStateCode();
    }

    void MutableBoardTransaction::setStateCode(const StateCode_t code) {
        impl->setStateCode(code);
    }

    TransactionNumber_t MutableBoardTransaction::getTransactionNumber() const {
        return impl->getTransactionNumber();
    }

    void MutableBoardTransaction::setTransactionNumber(const TransactionNumber_t value) {
        impl->setTransactionNumber(value);
    }

    std::vector<BoardOp::Operation*>& MutableBoardTransaction::getBoardOps() {
        return impl->getBoardOps();
    }

    const std::vector<BoardOp::Operation*>& MutableBoardTransaction::getBoardOps() const {
        return impl->getBoardOps();
    }

}
