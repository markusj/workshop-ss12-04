/*
 * BoardTransaction.h
 *
 *  Created on: 12.06.2012
 *      Author: markus
 */



#ifndef BOARDTRANSACTION_H_
#define BOARDTRANSACTION_H_

#include <vector>
#include "boardgame.h"
#include "Coords.h"
#include "Board.h"

namespace boardgame {

    class BoardTransaction {
            friend class MutableBoardTransaction;

        public:
            virtual ~BoardTransaction() { };

            virtual const Coords& getFrom() const = 0;
            virtual const Coords& getTo() const = 0;

            virtual bool isAccepted() const = 0;
            virtual StateCode_t getStateCode() const = 0;
            virtual TransactionNumber_t getTransactionNumber() const = 0;

            virtual const std::vector<BoardOp::Operation*>& getBoardOps() const = 0;

        protected:
            virtual void setAccepted(const bool accepted) = 0;
            virtual void setStateCode(const StateCode_t code) = 0;
            virtual void setTransactionNumber(const TransactionNumber_t value) = 0;

            virtual std::vector<BoardOp::Operation*>& getBoardOps() = 0;
    };

    class BasicBoardTransaction: public BoardTransaction {

        public:
            BasicBoardTransaction(const Coords from, const Coords to);
            BasicBoardTransaction(std::vector<BoardOp::Operation*>& ops);
            virtual ~BasicBoardTransaction();

            BasicBoardTransaction(const BasicBoardTransaction &src);
            BasicBoardTransaction& operator=(const BasicBoardTransaction &src);

            virtual const Coords& getFrom() const;
            virtual const Coords& getTo() const;

            virtual bool isAccepted() const;
            virtual StateCode_t getStateCode() const;
            virtual TransactionNumber_t getTransactionNumber() const;

            virtual const std::vector<BoardOp::Operation*>& getBoardOps() const;

        protected:
            virtual void setAccepted(const bool accepted);
            virtual void setStateCode(const StateCode_t code);
            virtual void setTransactionNumber(const TransactionNumber_t value);

            virtual std::vector<BoardOp::Operation*>& getBoardOps();


            Coords from;
            Coords to;

            bool accepted;
            StateCode_t stateCode;
            TransactionNumber_t num;
            std::vector<BoardOp::Operation*> boardOps;
    };

    class MutableBoardTransaction : public BoardTransaction {
            friend class Board;

        protected:
            MutableBoardTransaction(BoardTransaction &tr);

        public:
            virtual ~MutableBoardTransaction();

            BoardTransaction& getBoardTransaction() const;

            virtual const Coords& getFrom() const;
            virtual const Coords& getTo() const;

            virtual bool isAccepted() const;
            virtual void setAccepted(const bool accepted);

            virtual StateCode_t getStateCode() const;
            virtual void setStateCode(const StateCode_t code);

            virtual TransactionNumber_t getTransactionNumber() const;
            virtual void setTransactionNumber(const TransactionNumber_t value);

            virtual std::vector<BoardOp::Operation*>& getBoardOps();
            virtual const std::vector<BoardOp::Operation*>& getBoardOps() const;

        private:
            BoardTransaction *impl;
    };

}

#endif /* BOARDTRANSACTION_H_ */
