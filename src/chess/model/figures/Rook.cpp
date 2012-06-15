/*
 * Rook.cpp
 *
 *  Created on: 15 Jun 2012
 *      Author: Koios
 */




#include <cassert>
#include <cmath>
#include <cstddef>

#include "AllChessFigures.hpp"
#include "boardgame/Board.h"




namespace chess
{
	namespace figure
	{
		Rook::Rook(boardgame::Player* b)
			: OrdinaryChessFigure(b)
			, m_canDoCastling(true)
		{}
		Rook::Rook(Rook const& p, NoCastling)
			: OrdinaryChessFigure( p.getPlayer() )
			, m_canDoCastling(false)
		{}
		Rook::~Rook()
		{}

		bool Rook::apply(boardgame::MutableBoardTransaction& move) const
		{
			bool canMove = this->apply_internal(move);

			if(canMove && this->canDoCastling())
			{
				// neat line xD
				Rook newMe(*this, NoCastling());
				move.getBoardOps().push_back( new boardgame::BoardOp::Replace(move.getTo(), &newMe) );
			}

			return canMove;
		}

		bool Rook::castling(boardgame::MutableBoardTransaction& move, boardgame::Coords const& target) const
		{
			if( false == this->canMoveTo(target) )
			{// someone is in-between
				//move.setStateCode(INVALID);	// King sets this
				return false;
			}

			// TODO is capture-by-castling allowed?
			if( false == this->moveCapture(move) )
			{
				return false;
			}

			// neat line xD
			Rook newMe( *this, NoCastling());
			move.getBoardOps().push_back( new boardgame::BoardOp::Replace(target, &newMe) );

			return true;
		}

		bool Rook::canMoveTo(boardgame::Coords const& to) const
		{
			int ownX = static_cast<int>(getPosition().getX());
			int ownY = static_cast<int>(getPosition().getY());
			int toX = static_cast<int>(to.getX());
			int toY = static_cast<int>(to.getY());

			if(ownX != toX && ownY != toY)
			{
				return false;
			}

			CoordsIncr incr = { (ownX-toX)/std::abs(ownX-toX),
								(ownY-toY)/std::abs(ownY-toY) };
			return canMoveTo_incr(to, incr);
		}
	}
}
