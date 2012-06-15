/*
 * Pawn.cpp
 *
 *  Created on: 15.06.2012
 *      Author: DyP
 */




#include <cassert>
#include <cmath>
#include <cstddef>

#include "AllChessFigures.hpp"
#include "boardgame/BoardgameStatusCodes.h"
#include "boardgame/Board.h"




namespace chess
{
	namespace figure
	{
		Pawn::Pawn(boardgame::Player* b)
			: ChessFigure(b)
			, canJump(true)
			, jumpedAt(0)
			, movingDir(0)
		{}
		Pawn::Pawn(Pawn const& p, boardgame::TransactionNumber_t j)
			: ChessFigure( p.getPlayer() )
			, canJump(false)
			, jumpedAt(j)
			, movingDir(p.movingDir)
		{}
		Pawn::~Pawn()
		{}

		void Pawn::setPosition(boardgame::Coords const& c)
		{
			if(0 == movingDir)
			{
				     if(                                      1 == c.getY() ) { movingDir = +1; }
				else if( getBoard()->getDimensions().getY() - 1 == c.getY() ) { movingDir = -1; }
				else { assert(false & !"cannot determine in which direction this Pawn may be moving"); }
			}

			AbstractFigure::setPosition(c);
		}

		bool Pawn::apply(boardgame::MutableBoardTransaction& move) const
		{
			// emulate OrdinaryChessFigure::apply_internal
		        boardgame::Coords const& to = move.getTo();

				if( false == this->canMoveTo(to) )
				{
					move.setStateCode(boardgame::bsc_invalid);
					return false;
				}
				bool canMove = this->moveCapture(move);

			// special case: capture en-passant
				if( to.getX() != getPosition().getX() && NULL == getBoard()->get(to) )
				{// moving diagonally while the target field is empty => identify as capture en-passant
				    boardgame::BoardOp::Operation* pReplace = new boardgame::BoardOp::Replace(to, NULL);
					move.getBoardOps().push_back(pReplace);
				}


			if(canMove && this->canJump)
			{
				// neat line xD
				Pawn newMe( *this, move.getTransactionNumber() );
				move.getBoardOps().push_back( new boardgame::BoardOp::Replace(move.getTo(), &newMe) );
			}

			return canMove;
		}

		bool Pawn::hasJustJumped() const
		{
			return false;
		}

		bool Pawn::canMoveTo(boardgame::Coords const& to) const
		{
			int ownX = static_cast<int>(getPosition().getX());
			int ownY = static_cast<int>(getPosition().getY());
			int toX = static_cast<int>(to.getX());
			int toY = static_cast<int>(to.getY());

			if( movingDir != (toY-ownY)/std::abs(toY-ownY) )
			{// moving in the wrong direction (row)
				return false;
			}

			if( 1 == std::abs(toY - ownY) )
			{// no jump
				if( 1 == std::abs(toX - ownX) )
				{// tries to capture
					if( getBoard()->get(to)->getPlayer() != this->getPlayer() ) { return true; }	// normal capture

					// capture en-passant
					CoordsIncr incr = { toX-ownX, 0 };
					Figure const* const figAtEnPassantPos = getBoard()->get(to + incr);
					if(0 == figAtEnPassantPos) { return false; }

					Pawn const* const pawnAtEnPassantPos = dynamic_cast < Pawn const* > (figAtEnPassantPos);
					if(0 == pawnAtEnPassantPos) { return false; }

					if(pawnAtEnPassantPos->getPlayer() == this->getPlayer()) { return false; }
					if( false == pawnAtEnPassantPos->hasJustJumped() ) { return false; }
				}else
				{// normal move
					// nothing to check
				}
			}else if( 2 == std::abs(toY - ownY) )
			{// jump
				if(!canJump) { return false; }
			}
			//- no capture

			return ( 0 == getBoard()->get(to) );	// if the field is free, the move is allowed
		}

		bool Pawn::threatens(const boardgame::Coords &to) const {
		    // TODO: To be implemented
		}
	}
}
